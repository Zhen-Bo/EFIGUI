// =============================================
// EFIGUI Components - Slider
// Modern slider with editable value display box
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Slider Layout Constants
    // =============================================

    namespace SliderLayout
    {
        constexpr float Height = 24.0f;
        constexpr float TrackHeight = 6.0f;
        constexpr float KnobRadius = 8.0f;
        constexpr float ValueInputWidth = 55.0f;
        constexpr float InputGap = 8.0f;
        constexpr float LabelGap = 12.0f;
        constexpr float MinSliderWidth = 80.0f;
        constexpr float InputRounding = 6.0f;
        constexpr float InputInset = 2.0f;
        constexpr float InputPaddingX = 6.0f;
        constexpr float GlowExpandSize = 4.0f;
        constexpr float HoverThreshold = 0.1f;
        constexpr float GlowThreshold = 0.05f;
        constexpr float ActiveGlowIntensity = 0.8f;
        constexpr float EditingGlowIntensity = 0.7f;
    }

    namespace SliderColors
    {
        constexpr ImU32 BezelColor = IM_COL32(25, 25, 35, 255);
        constexpr ImU32 ScreenBgDefault = IM_COL32(15, 15, 25, 255);
        constexpr ImU32 ScreenBgHover = IM_COL32(20, 25, 35, 255);
        constexpr ImU32 ScreenBgEditing = IM_COL32(20, 30, 40, 255);
        constexpr ImU32 InnerBorderTop = IM_COL32(10, 10, 15, 255);
        constexpr ImU32 InnerBorderBottom = IM_COL32(50, 50, 70, 100);
        constexpr ImU32 OuterBorderDefault = IM_COL32(60, 60, 80, 255);
        constexpr ImU32 OuterBorderHover = IM_COL32(80, 100, 120, 255);
        constexpr ImU32 TextDefault = IM_COL32(180, 200, 210, 255);
    }

    // =============================================
    // Slider Internal State
    // =============================================

    static std::unordered_map<ImGuiID, std::string> s_sliderInputBuffers;
    static ImGuiID s_activeSliderInputId = 0;
    static int s_sliderEditStartFrame = 0;

    // =============================================
    // Slider Helper Functions
    // =============================================

    // Format a float value to string with given precision
    static std::string FormatSliderValue(float value, int precision)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value;
        return oss.str();
    }

    // Parse format string to extract precision (e.g., "%.2f" -> 2)
    static int ParseFormatPrecision(const char* format)
    {
        if (!format) return 1;

        const char* p = format;
        while (*p)
        {
            if (*p == '.' && *(p + 1) >= '0' && *(p + 1) <= '9')
            {
                return *(p + 1) - '0';
            }
            p++;
        }
        return 1;
    }

    // Apply slider input value and reset editing state
    static bool ApplySliderInputValue(ImGuiID id, float* value, float min, float max, int precision, bool& changed)
    {
        try
        {
            float newVal = std::stof(s_sliderInputBuffers[id]);
            newVal = std::clamp(newVal, min, max);
            if (value && newVal != *value)
            {
                *value = newVal;
                changed = true;
            }
        }
        catch (...) { /* Invalid input, ignore */ }

        s_sliderInputBuffers[id] = FormatSliderValue(value ? *value : 0.0f, precision);
        s_activeSliderInputId = 0;
        return changed;
    }

    // Ensure input buffer is initialized and synced
    static void SyncSliderInputBuffer(ImGuiID id, float* value, int precision, bool isEditing)
    {
        std::string expectedValue = FormatSliderValue(value ? *value : 0.0f, precision);

        if (s_sliderInputBuffers.find(id) == s_sliderInputBuffers.end())
        {
            s_sliderInputBuffers[id] = expectedValue;
        }
        else if (!isEditing && value && s_sliderInputBuffers[id] != expectedValue)
        {
            s_sliderInputBuffers[id] = expectedValue;
        }
    }

    // Handle slider drag interaction and update value
    static bool HandleSliderDrag(float* value, float min, float max, float sliderStartX, float sliderWidth,
                                  ImGuiID id, int precision, bool sliderActive)
    {
        if (!sliderActive || !value) return false;

        float mouseX = ImGui::GetMousePos().x;
        float sliderEndX = sliderStartX + sliderWidth;
        float clampedMouseX = std::clamp(mouseX, sliderStartX, sliderEndX);

        float normalizedValue = (clampedMouseX - sliderStartX) / sliderWidth;
        float newValue = min + normalizedValue * (max - min);

        if (newValue != *value)
        {
            *value = newValue;
            s_sliderInputBuffers[id] = FormatSliderValue(*value, precision);
            return true;
        }
        return false;
    }

    // Draw the slider track (background + filled portion + glow)
    static void DrawSliderTrack(ImDrawList* draw, float sliderStartX, float trackY, float sliderWidth,
                                 float fillWidth, float trackHeight, const Animation::WidgetState& state, bool sliderActive)
    {
        // Background track
        draw->AddRectFilled(
            ImVec2(sliderStartX, trackY),
            ImVec2(sliderStartX + sliderWidth, trackY + trackHeight),
            Theme::ButtonDefault,
            trackHeight * 0.5f
        );

        // Filled portion with gradient
        if (fillWidth > 0)
        {
            Draw::RectGradientH(
                ImVec2(sliderStartX, trackY),
                ImVec2(sliderStartX + fillWidth, trackY + trackHeight),
                Theme::AccentPurple,
                Theme::AccentCyan,
                trackHeight * 0.5f
            );

            // Glow effect on hover/active
            if (state.hoverAnim > SliderLayout::HoverThreshold || sliderActive)
            {
                float glowIntensity = sliderActive ? SliderLayout::ActiveGlowIntensity : state.hoverAnim * 0.5f;
                Draw::RectGlow(
                    ImVec2(sliderStartX, trackY),
                    ImVec2(sliderStartX + fillWidth, trackY + trackHeight),
                    Theme::AccentCyanGlow,
                    glowIntensity,
                    SliderLayout::GlowExpandSize
                );
            }
        }
    }

    // Draw the slider knob with glow effect
    static void DrawSliderKnob(ImDrawList* draw, float knobX, float knobY, float knobRadius,
                                const Animation::WidgetState& state, bool sliderActive, std::optional<Layer> layer)
    {
        // Knob glow effect
        if (state.hoverAnim > SliderLayout::HoverThreshold || sliderActive)
        {
            float glowIntensity = sliderActive ? SliderLayout::ActiveGlowIntensity : state.hoverAnim * 0.5f;
            Draw::GlowLayersCircle(ImVec2(knobX, knobY), knobRadius + 2.0f, Theme::AccentCyan,
                                    glowIntensity, Theme::SliderGlowLayers, Theme::SliderKnobGlowExpand, layer);
        }

        ImU32 knobColor = sliderActive ? Theme::AccentCyan : Theme::TextPrimary;
        draw->AddCircleFilled(ImVec2(knobX, knobY), knobRadius, knobColor);
    }

    // Draw the value display box background and border
    static void DrawValueBoxBackground(ImDrawList* draw, ImVec2 inputBoxMin, ImVec2 inputBoxMax,
                                        float inputRounding, const Animation::WidgetState& displayState,
                                        bool isEditing, std::optional<Layer> layer)
    {
        float valueInputWidth = inputBoxMax.x - inputBoxMin.x;
        float valueInputHeight = inputBoxMax.y - inputBoxMin.y;

        // Draw glow effect FIRST (before background)
        if (displayState.hoverAnim > SliderLayout::GlowThreshold || isEditing)
        {
            float glowIntensity = isEditing ? SliderLayout::EditingGlowIntensity : displayState.hoverAnim * 0.5f;
            ImVec2 boxSize = ImVec2(valueInputWidth, valueInputHeight);
            Draw::GlowLayers(inputBoxMin, boxSize, Theme::AccentCyan, glowIntensity,
                              Theme::SliderGlowLayers, Theme::SliderInputGlowExpand, inputRounding, layer);
        }

        // Bezel (outer frame)
        draw->AddRectFilled(inputBoxMin, inputBoxMax, SliderColors::BezelColor, inputRounding);

        // Inner screen area
        ImVec2 screenMin = ImVec2(inputBoxMin.x + SliderLayout::InputInset, inputBoxMin.y + SliderLayout::InputInset);
        ImVec2 screenMax = ImVec2(inputBoxMax.x - SliderLayout::InputInset, inputBoxMax.y - SliderLayout::InputInset);

        // Screen background
        ImU32 screenBg = isEditing ? SliderColors::ScreenBgEditing :
                         Animation::LerpColorU32(SliderColors::ScreenBgDefault, SliderColors::ScreenBgHover, displayState.hoverAnim);
        draw->AddRectFilled(screenMin, screenMax, screenBg, inputRounding - SliderLayout::InputInset);

        // Inner border/bevel effect
        draw->AddLine(ImVec2(screenMin.x + inputRounding, screenMin.y),
                      ImVec2(screenMax.x - inputRounding, screenMin.y),
                      SliderColors::InnerBorderTop, 1.0f);
        draw->AddLine(ImVec2(screenMin.x + inputRounding, screenMax.y),
                      ImVec2(screenMax.x - inputRounding, screenMax.y),
                      SliderColors::InnerBorderBottom, 1.0f);

        // Outer border
        ImU32 borderColor = isEditing ? Theme::AccentCyan :
                           Animation::LerpColorU32(SliderColors::OuterBorderDefault, SliderColors::OuterBorderHover, displayState.hoverAnim);
        draw->AddRect(inputBoxMin, inputBoxMax, borderColor, inputRounding, 0, 1.0f);
    }

    // Handle the editable input text when in editing mode
    static bool HandleValueInputEditing(ImGuiID id, float* value, float min, float max, int precision,
                                         const char* label, ImVec2 inputBoxMin, float valueInputWidth,
                                         float valueInputHeight, float inputRounding, bool& changed)
    {
        bool isFirstEditFrame = (s_sliderEditStartFrame == ImGui::GetFrameCount());
        if (isFirstEditFrame)
        {
            ImGui::SetKeyboardFocusHere();
        }

        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Text, Theme::ToVec4(Theme::AccentCyan));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, inputRounding);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(SliderLayout::InputPaddingX, (valueInputHeight - ImGui::GetFontSize()) * 0.5f));

        ImGui::SetNextItemWidth(valueInputWidth);

        char inputBuf[32];
        strncpy(inputBuf, s_sliderInputBuffers[id].c_str(), sizeof(inputBuf) - 1);
        inputBuf[sizeof(inputBuf) - 1] = '\0';

        std::string inputId = std::string("##") + label + "_input";
        if (ImGui::InputText(inputId.c_str(), inputBuf, sizeof(inputBuf),
            ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            s_sliderInputBuffers[id] = inputBuf;
            ApplySliderInputValue(id, value, min, max, precision, changed);
        }
        else
        {
            s_sliderInputBuffers[id] = inputBuf;
        }

        // Track if still editing
        if (ImGui::IsItemActive())
        {
            s_activeSliderInputId = id;
        }
        else if (isFirstEditFrame)
        {
            s_activeSliderInputId = id;
        }
        else
        {
            ApplySliderInputValue(id, value, min, max, precision, changed);
        }

        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(5);

        return changed;
    }

    // Draw the value text when in display mode (not editing)
    static void DrawValueDisplay(ImDrawList* draw, ImGuiID id, ImVec2 inputBoxMin,
                                  float valueInputWidth, float valueInputHeight,
                                  const Animation::WidgetState& displayState)
    {
        const std::string& displayValue = s_sliderInputBuffers[id];
        ImVec2 valTextSize = ImGui::CalcTextSize(displayValue.c_str());
        ImVec2 textPos = ImVec2(
            inputBoxMin.x + (valueInputWidth - valTextSize.x) * 0.5f,
            inputBoxMin.y + (valueInputHeight - valTextSize.y) * 0.5f
        );

        ImU32 textColor = Animation::LerpColorU32(SliderColors::TextDefault, Theme::TextPrimary, displayState.hoverAnim);
        draw->AddText(textPos, textColor, displayValue.c_str());
    }

    // =============================================
    // Public Slider Functions
    // =============================================

    bool ModernSliderFloat(const char* label, float* value, float min, float max, const char* format, std::optional<Layer> layer)
    {
        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Parse format and calculate precision
        int precision = ParseFormatPrecision(format);

        // Calculate label dimensions
        bool labelHidden = (label[0] == '#' && label[1] == '#');
        float labelWidth = 0.0f;
        ImVec2 textSize = ImVec2(0, 0);
        if (!labelHidden)
        {
            textSize = ImGui::CalcTextSize(label);
            labelWidth = textSize.x + SliderLayout::LabelGap;
        }

        // Calculate layout dimensions
        float knobPadding = SliderLayout::KnobRadius + 2.0f;
        float valueInputHeight = SliderLayout::Height - 2.0f;
        float sliderWidth = ImGui::GetContentRegionAvail().x - labelWidth - SliderLayout::ValueInputWidth - knobPadding * 2 - SliderLayout::InputGap;
        sliderWidth = std::max(sliderWidth, SliderLayout::MinSliderWidth);

        bool changed = false;
        bool isEditingThisSlider = (s_activeSliderInputId == id);

        // Draw label
        if (!labelHidden)
        {
            draw->AddText(ImVec2(pos.x, pos.y + (SliderLayout::Height - textSize.y) * 0.5f), Theme::TextPrimary, label);
        }

        // Slider track area calculations
        float sliderStartX = pos.x + labelWidth + knobPadding;
        float trackY = pos.y + (SliderLayout::Height - SliderLayout::TrackHeight) * 0.5f;

        // Create invisible button for slider interaction
        ImGui::SetCursorScreenPos(ImVec2(sliderStartX - knobPadding, pos.y));
        ImGui::InvisibleButton((std::string(label) + "_slider").c_str(), ImVec2(sliderWidth + knobPadding * 2, SliderLayout::Height));
        bool sliderHovered = ImGui::IsItemHovered();
        bool sliderActive = ImGui::IsItemActive();

        Animation::UpdateWidgetState(state, sliderHovered, sliderActive, false);

        // Handle slider drag
        if (HandleSliderDrag(value, min, max, sliderStartX, sliderWidth, id, precision, sliderActive))
        {
            changed = true;
        }

        // Clamp value
        if (value)
        {
            *value = std::clamp(*value, min, max);
        }

        // Calculate fill width
        float normalizedCurrent = value ? std::clamp((*value - min) / (max - min), 0.0f, 1.0f) : 0.0f;
        float fillWidth = sliderWidth * normalizedCurrent;

        // Draw track
        DrawSliderTrack(draw, sliderStartX, trackY, sliderWidth, fillWidth, SliderLayout::TrackHeight, state, sliderActive);

        // Draw knob
        float knobX = sliderStartX + fillWidth;
        float knobY = pos.y + SliderLayout::Height * 0.5f;
        DrawSliderKnob(draw, knobX, knobY, SliderLayout::KnobRadius, state, sliderActive, layer);

        // Value input box calculations
        float inputX = sliderStartX + sliderWidth + knobPadding + SliderLayout::InputGap;
        float inputY = pos.y + (SliderLayout::Height - valueInputHeight) * 0.5f;
        ImVec2 inputBoxMin = ImVec2(inputX, inputY);
        ImVec2 inputBoxMax = ImVec2(inputX + SliderLayout::ValueInputWidth, inputY + valueInputHeight);

        // Sync input buffer
        SyncSliderInputBuffer(id, value, precision, isEditingThisSlider);

        // Create invisible button for display box interaction
        ImGui::SetCursorScreenPos(inputBoxMin);
        ImGui::InvisibleButton((std::string(label) + "_display").c_str(), ImVec2(SliderLayout::ValueInputWidth, valueInputHeight));
        bool displayHovered = ImGui::IsItemHovered();
        bool displayClicked = ImGui::IsItemClicked();

        // Get animation state for display box
        ImGuiID displayId = ImGui::GetID((std::string(label) + "_display").c_str());
        Animation::WidgetState& displayState = Animation::GetState(displayId);
        Animation::UpdateWidgetState(displayState, displayHovered, isEditingThisSlider, false);

        // Draw value box background
        DrawValueBoxBackground(draw, inputBoxMin, inputBoxMax, SliderLayout::InputRounding, displayState, isEditingThisSlider, layer);

        // Handle input/display
        ImGui::SetCursorScreenPos(inputBoxMin);

        if (isEditingThisSlider)
        {
            HandleValueInputEditing(id, value, min, max, precision, label, inputBoxMin,
                                     SliderLayout::ValueInputWidth, valueInputHeight, SliderLayout::InputRounding, changed);
        }
        else
        {
            DrawValueDisplay(draw, id, inputBoxMin, SliderLayout::ValueInputWidth, valueInputHeight, displayState);

            if (displayClicked)
            {
                s_activeSliderInputId = id;
                s_sliderEditStartFrame = ImGui::GetFrameCount() + 1;
            }
        }

        // Restore cursor for next element
        ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + SliderLayout::Height + ImGui::GetStyle().ItemSpacing.y));

        return changed;
    }

    bool ModernSliderInt(const char* label, int* value, int min, int max, const char* format, std::optional<Layer> layer)
    {
        int originalValue = value ? *value : 0;

        // Clamp input value first
        if (value)
        {
            *value = std::clamp(*value, min, max);
        }

        float floatValue = value ? (float)*value : 0.0f;
        bool changed = ModernSliderFloat(label, &floatValue, (float)min, (float)max, "%.0f", layer);

        if (value)
        {
            int newValue = std::clamp((int)(floatValue + 0.5f), min, max);
            *value = newValue;
            changed = (newValue != originalValue);
        }
        return changed;
    }
}
