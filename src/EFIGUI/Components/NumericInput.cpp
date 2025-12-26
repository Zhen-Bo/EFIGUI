// =============================================
// EFIGUI Components - NumericInput
// Standalone editable numeric value display box
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // NumericInput Layout Constants
    // =============================================

    namespace NumericInputLayout
    {
        constexpr float DefaultWidth = 55.0f;
        constexpr float DefaultHeight = 22.0f;
        constexpr float Rounding = 6.0f;
        constexpr float Inset = 2.0f;
        constexpr float PaddingX = 6.0f;
        constexpr float GlowExpandSize = 4.0f;
        constexpr float HoverThreshold = 0.1f;
        constexpr float GlowThreshold = 0.05f;
        constexpr float EditingGlowIntensity = 0.7f;
    }

    namespace NumericInputColors
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
    // NumericInput Buffer Management
    // =============================================

    struct NumericInputData
    {
        std::string buffer;
        int lastUpdateFrame = 0;
    };

    static std::unordered_map<ImGuiID, NumericInputData>& GetNumericInputBuffers()
    {
        static std::unordered_map<ImGuiID, NumericInputData> s_buffers;
        return s_buffers;
    }

    // =============================================
    // NumericInput Helper Functions
    // =============================================

    static std::string FormatNumericValue(float value, int precision)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << value;
        return oss.str();
    }

    static bool ApplyNumericInputValue(ImGuiID id, float* value, float min, float max, int precision)
    {
        auto& buffers = GetNumericInputBuffers();
        auto it = buffers.find(id);
        if (it == buffers.end()) return false;

        bool changed = false;
        try
        {
            float newVal = std::stof(it->second.buffer);
            newVal = std::clamp(newVal, min, max);
            if (value && newVal != *value)
            {
                *value = newVal;
                changed = true;
            }
        }
        catch (...) { /* Invalid input, ignore */ }

        // Sync buffer back to current value
        it->second.buffer = FormatNumericValue(value ? *value : 0.0f, precision);
        it->second.lastUpdateFrame = ImGui::GetFrameCount();
        return changed;
    }

    static void SyncNumericInputBuffer(ImGuiID id, float value, int precision, bool isEditing)
    {
        auto& buffers = GetNumericInputBuffers();
        std::string expectedValue = FormatNumericValue(value, precision);
        int currentFrame = ImGui::GetFrameCount();

        auto it = buffers.find(id);
        if (it == buffers.end())
        {
            buffers[id] = { expectedValue, currentFrame };
        }
        else
        {
            it->second.lastUpdateFrame = currentFrame;
            // Only sync if not editing (don't overwrite user input)
            if (!isEditing && it->second.buffer != expectedValue)
            {
                it->second.buffer = expectedValue;
            }
        }
    }

    // =============================================
    // NumericInput Drawing
    // =============================================

    static void DrawNumericInputBackground(ImDrawList* draw, ImVec2 boxMin, ImVec2 boxMax,
                                           float rounding, const Animation::WidgetState& state,
                                           bool isEditing, std::optional<Layer> layer)
    {
        using namespace NumericInputLayout;
        using namespace NumericInputColors;

        float width = boxMax.x - boxMin.x;
        float height = boxMax.y - boxMin.y;

        // Draw glow effect first (before background)
        if (state.hoverAnim > GlowThreshold || isEditing)
        {
            float glowIntensity = isEditing ? EditingGlowIntensity : state.hoverAnim * 0.5f;
            ImVec2 boxSize = ImVec2(width, height);
            Draw::GlowLayers(boxMin, boxSize, Theme::AccentCyan, glowIntensity,
                              Theme::SliderGlowLayers, Theme::SliderInputGlowExpand, rounding, layer);
        }

        // Bezel (outer frame)
        draw->AddRectFilled(boxMin, boxMax, BezelColor, rounding);

        // Inner screen area
        ImVec2 screenMin = ImVec2(boxMin.x + Inset, boxMin.y + Inset);
        ImVec2 screenMax = ImVec2(boxMax.x - Inset, boxMax.y - Inset);

        // Screen background
        ImU32 screenBg = isEditing ? ScreenBgEditing :
                         Animation::LerpColorU32(ScreenBgDefault, ScreenBgHover, state.hoverAnim);
        draw->AddRectFilled(screenMin, screenMax, screenBg, rounding - Inset);

        // Inner border/bevel effect
        draw->AddLine(ImVec2(screenMin.x + rounding, screenMin.y),
                      ImVec2(screenMax.x - rounding, screenMin.y),
                      InnerBorderTop, 1.0f);
        draw->AddLine(ImVec2(screenMin.x + rounding, screenMax.y),
                      ImVec2(screenMax.x - rounding, screenMax.y),
                      InnerBorderBottom, 1.0f);

        // Outer border
        ImU32 borderColor = isEditing ? Theme::AccentCyan :
                           Animation::LerpColorU32(OuterBorderDefault, OuterBorderHover, state.hoverAnim);
        draw->AddRect(boxMin, boxMax, borderColor, rounding, 0, 1.0f);
    }

    // =============================================
    // Public NumericInput Function
    // =============================================

    bool NumericInput(const char* label, float* value, const NumericInputConfig& config, std::optional<Layer> layer)
    {
        using namespace NumericInputLayout;
        using namespace NumericInputColors;

        if (!value) return false;

        ImGuiID id = ImGui::GetID(label);
        auto& buffers = GetNumericInputBuffers();

        // Calculate box position and size
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float width = config.width > 0 ? config.width : DefaultWidth;
        float height = DefaultHeight;
        ImVec2 boxMin = pos;
        ImVec2 boxMax = ImVec2(pos.x + width, pos.y + height);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Build InputText ID to check if we're editing
        std::string inputIdStr = std::string("##") + label + "_input";
        ImGuiID inputTextId = ImGui::GetID(inputIdStr.c_str());
        bool isEditing = (ImGui::GetActiveID() == inputTextId);

        // Sync buffer with current value
        SyncNumericInputBuffer(id, *value, config.precision, isEditing);

        // Get animation state - use mouse position for hover detection (no InvisibleButton)
        Animation::WidgetState& state = Animation::GetState(id);
        ImVec2 mousePos = ImGui::GetMousePos();
        bool hovered = (mousePos.x >= boxMin.x && mousePos.x <= boxMax.x &&
                        mousePos.y >= boxMin.y && mousePos.y <= boxMax.y);
        Animation::UpdateWidgetState(state, hovered || isEditing, isEditing, false);

        // Draw background
        DrawNumericInputBackground(draw, boxMin, boxMax, Rounding, state, isEditing, layer);

        // Render InputText (always rendered - handles its own click/focus)
        ImGui::SetCursorScreenPos(boxMin);

        // Calculate vertical padding for centered text
        float textHeight = ImGui::GetFontSize();
        float verticalPadding = (height - textHeight) * 0.5f;

        // Style: transparent background, colored text when editing
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Text, isEditing ? Theme::ToVec4(Theme::AccentCyan) :
                              Theme::ToVec4(Animation::LerpColorU32(TextDefault, Theme::TextPrimary, state.hoverAnim)));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Rounding);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(PaddingX, verticalPadding));

        ImGui::SetNextItemWidth(width);

        // Prepare buffer for InputText
        char inputBuf[32];
        strncpy(inputBuf, buffers[id].buffer.c_str(), sizeof(inputBuf) - 1);
        inputBuf[sizeof(inputBuf) - 1] = '\0';

        bool enterPressed = ImGui::InputText(inputIdStr.c_str(), inputBuf, sizeof(inputBuf),
            ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);

        // Update buffer from InputText
        buffers[id].buffer = inputBuf;

        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(5);

        bool changed = false;

        // Apply value on Enter or when focus is lost after editing
        if (enterPressed || ImGui::IsItemDeactivatedAfterEdit())
        {
            changed = ApplyNumericInputValue(id, value, config.min, config.max, config.precision);
        }

        // Advance cursor
        ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + height + ImGui::GetStyle().ItemSpacing.y));

        return changed;
    }

    // =============================================
    // Cleanup Functions
    // =============================================

    void PruneNumericInputBuffers(int maxIdleFrames)
    {
        auto& buffers = GetNumericInputBuffers();
        int currentFrame = ImGui::GetFrameCount();

        for (auto it = buffers.begin(); it != buffers.end(); )
        {
            if (currentFrame - it->second.lastUpdateFrame > maxIdleFrames)
            {
                it = buffers.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}
