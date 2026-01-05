// =============================================
// EFIGUI Components - Slider
// Modern slider with decoupled track and value input
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Slider Local Constants (non-configurable)
    // =============================================

    namespace SliderLocal
    {
        constexpr float LabelGap = 12.0f;
        constexpr float MinSliderWidth = 80.0f;
        constexpr float GlowExpandSize = 4.0f;
        constexpr float HoverThreshold = 0.1f;
        constexpr float ActiveGlowIntensity = 0.8f;
    }

    // =============================================
    // Slider Helper Functions
    // =============================================

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

    // =============================================
    // SliderTrack - Internal Component
    // =============================================
    // Handles the slider track, knob, and drag interaction.
    // Returns true if value was changed by dragging.

    static bool SliderTrack(const char* label, float* value, float min, float max,
                            float startX, float trackY, float sliderWidth,
                            const Animation::WidgetState& state, bool sliderActive,
                            float trackHeight, float knobRadius, ImU32 glowColor,
                            int glowLayers, float knobGlowExpand, std::optional<Layer> layer)
    {
        using namespace SliderLocal;

        if (!value) return false;

        ImDrawList* draw = ImGui::GetWindowDrawList();
        bool changed = false;

        // Clamp value
        *value = std::clamp(*value, min, max);

        // Calculate fill width
        float normalizedCurrent = std::clamp((*value - min) / (max - min), 0.0f, 1.0f);
        float fillWidth = sliderWidth * normalizedCurrent;

        // Handle drag interaction
        if (sliderActive)
        {
            float mouseX = ImGui::GetMousePos().x;
            float sliderEndX = startX + sliderWidth;
            float clampedMouseX = std::clamp(mouseX, startX, sliderEndX);

            float normalizedValue = (clampedMouseX - startX) / sliderWidth;
            float newValue = min + normalizedValue * (max - min);

            if (newValue != *value)
            {
                *value = newValue;
                changed = true;
                // Recalculate fill width after value change
                normalizedCurrent = std::clamp((*value - min) / (max - min), 0.0f, 1.0f);
                fillWidth = sliderWidth * normalizedCurrent;
            }
        }

        // Draw background track
        draw->AddRectFilled(
            ImVec2(startX, trackY),
            ImVec2(startX + sliderWidth, trackY + trackHeight),
            Theme::ButtonDefault(),
            trackHeight * 0.5f
        );

        // Draw filled portion with gradient
        if (fillWidth > 0)
        {
            Draw::RectGradientH(
                ImVec2(startX, trackY),
                ImVec2(startX + fillWidth, trackY + trackHeight),
                Theme::AccentPurple(),
                Theme::AccentCyan(),
                trackHeight * 0.5f
            );

            // Glow effect on hover/active
            if (state.hoverAnim > HoverThreshold || sliderActive)
            {
                float glowIntensity = sliderActive ? ActiveGlowIntensity : state.hoverAnim * 0.5f;
                Draw::RectGlow(
                    ImVec2(startX, trackY),
                    ImVec2(startX + fillWidth, trackY + trackHeight),
                    Theme::AccentCyanGlow(),
                    glowIntensity,
                    GlowExpandSize
                );
            }
        }

        // Draw knob
        float knobX = startX + fillWidth;
        float knobY = trackY + trackHeight * 0.5f;

        // Knob glow effect
        if (state.hoverAnim > HoverThreshold || sliderActive)
        {
            float glowIntensity = sliderActive ? ActiveGlowIntensity : state.hoverAnim * 0.5f;
            Draw::GlowLayersCircle(ImVec2(knobX, knobY), knobRadius + 2.0f, glowColor,
                                    glowIntensity, glowLayers, knobGlowExpand, layer);
        }

        ImU32 knobColor = sliderActive ? Theme::AccentCyan() : Theme::TextPrimary();
        draw->AddCircleFilled(ImVec2(knobX, knobY), knobRadius, knobColor);

        return changed;
    }

    // =============================================
    // Public Slider Functions (Config version - recommended)
    // =============================================

    bool ModernSliderFloat(const char* label, float* value, float min, float max,
                           const char* format, std::optional<ImU32> glowColor,
                           const SliderConfig& config)
    {
        using namespace SliderLocal;

        // Resolve config values with Theme defaults
        const auto& t = Theme::Slider();
        const float height = Resolve(config.height, t.height);
        const float trackHeight = Resolve(config.trackHeight, t.trackHeight);
        const float knobRadius = Resolve(config.knobRadius, t.knobRadius);
        const float inputWidth = Resolve(config.inputWidth, t.inputWidth);
        const float inputGap = Resolve(config.inputGap, t.inputGap);
        const int glowLayers = Resolve(config.glowLayers, t.glowLayers);
        const float knobGlowExpand = Resolve(config.knobGlowExpand, t.knobGlowExpand);
        const ImU32 effectiveGlowColor = glowColor.value_or(
            config.glowColor.value_or(Theme::AccentCyan()));

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
            labelWidth = textSize.x + LabelGap;
        }

        // Calculate layout dimensions
        float knobPadding = knobRadius + 2.0f;
        float valueInputHeight = height - 2.0f;
        float sliderWidth = ImGui::GetContentRegionAvail().x - labelWidth - inputWidth - knobPadding * 2 - inputGap;
        sliderWidth = std::max(sliderWidth, MinSliderWidth);

        bool changed = false;

        // Draw label
        if (!labelHidden)
        {
            draw->AddText(ImVec2(pos.x, pos.y + (height - textSize.y) * 0.5f), Theme::TextPrimary(), label);
        }

        // Slider track area calculations
        float sliderStartX = pos.x + labelWidth + knobPadding;
        float trackY = pos.y + (height - trackHeight) * 0.5f;

        // Create invisible button for slider interaction
        ImGui::SetCursorScreenPos(ImVec2(sliderStartX - knobPadding, pos.y));
        ImGui::InvisibleButton((std::string(label) + "_slider").c_str(), ImVec2(sliderWidth + knobPadding * 2, height));
        bool sliderHovered = ImGui::IsItemHovered();
        bool sliderActive = ImGui::IsItemActive();

        Animation::UpdateWidgetState(state, sliderHovered, sliderActive, false);

        // Draw and handle slider track
        if (SliderTrack(label, value, min, max, sliderStartX, trackY, sliderWidth, state, sliderActive,
                        trackHeight, knobRadius, effectiveGlowColor, glowLayers, knobGlowExpand, config.layer))
        {
            changed = true;
        }

        // Value input box - using NumericInput component (if enabled)
        if (config.showInput)
        {
            float inputX = sliderStartX + sliderWidth + knobPadding + inputGap;
            float inputY = pos.y + (height - valueInputHeight) * 0.5f;

            ImGui::SetCursorScreenPos(ImVec2(inputX, inputY));

            NumericInputConfig inputConfig;
            inputConfig.min = min;
            inputConfig.max = max;
            inputConfig.precision = precision;
            inputConfig.width = inputWidth;

            // Use unique ID for the input to avoid collision with slider
            std::string inputLabel = std::string("##") + label + "_value";
            if (NumericInput(inputLabel.c_str(), value, inputConfig, config.layer))
            {
                changed = true;
            }
        }

        // Restore cursor for next element
        ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + height + ImGui::GetStyle().ItemSpacing.y));

        return changed;
    }

    bool ModernSliderInt(const char* label, int* value, int min, int max,
                         const char* format, std::optional<ImU32> glowColor,
                         const SliderConfig& config)
    {
        int originalValue = value ? *value : 0;

        // Clamp input value first
        if (value)
        {
            *value = std::clamp(*value, min, max);
        }

        float floatValue = value ? (float)*value : 0.0f;
        bool changed = ModernSliderFloat(label, &floatValue, (float)min, (float)max, "%.0f", glowColor, config);

        if (value)
        {
            int newValue = std::clamp((int)(floatValue + 0.5f), min, max);
            *value = newValue;
            changed = (newValue != originalValue);
        }
        return changed;
    }

    // =============================================
    // Public Slider Functions (legacy API - for backward compatibility)
    // =============================================

    bool ModernSliderFloat(const char* label, float* value, float min, float max, const char* format, std::optional<Layer> layer)
    {
        SliderConfig config;
        config.layer = layer;
        return ModernSliderFloat(label, value, min, max, format, std::nullopt, config);
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

    // =============================================
    // Cleanup Functions
    // =============================================

    void PruneSliderInputBuffers(int maxIdleFrames)
    {
        // Delegate to NumericInput's cleanup
        PruneNumericInputBuffers(maxIdleFrames);
    }
}
