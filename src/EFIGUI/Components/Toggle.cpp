// =============================================
// EFIGUI Components - Toggle
// Modern toggle switch with optional description
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Toggle / Checkbox
    // =============================================

    bool ModernToggle(const char* label, bool* value)
    {
        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        float toggleWidth = Theme::ToggleWidth;
        float toggleHeight = Theme::ToggleHeight;
        float knobSize = Theme::ToggleKnobSize;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 textSize = ImGui::CalcTextSize(label);

        // Check if we're in a disabled state
        ImGuiContext& g = *ImGui::GetCurrentContext();
        bool isDisabled = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;

        // Total area
        float totalWidth = toggleWidth + Theme::ToggleLabelGap + textSize.x;
        ImGui::InvisibleButton(label, ImVec2(totalWidth, toggleHeight));

        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();

        if (clicked && value && !isDisabled)
        {
            *value = !*value;
        }

        bool isOn = value ? *value : false;

        // Update slide animation
        float targetSlide = isOn ? 1.0f : 0.0f;
        state.slideAnim = Animation::Lerp(state.slideAnim, targetSlide, 12.0f);

        Animation::UpdateWidgetState(state, hovered && !isDisabled, false, isOn);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Apply disabled alpha multiplier
        float alphaMultiplier = isDisabled ? 0.4f : 1.0f;

        // Track background
        ImU32 trackColorBase = Animation::LerpColorU32(
            Theme::ButtonDefault,
            Theme::AccentCyan,
            state.slideAnim
        );

        // Apply alpha for disabled state
        auto trackRGB = Theme::ExtractRGB(trackColorBase);
        int ta = (int)(((trackColorBase >> 24) & 0xFF) * alphaMultiplier);
        ImU32 trackColor = IM_COL32(trackRGB.r, trackRGB.g, trackRGB.b, ta);

        draw->AddRectFilled(
            pos,
            ImVec2(pos.x + toggleWidth, pos.y + toggleHeight),
            trackColor,
            toggleHeight * 0.5f
        );

        // Glow when on (skip when disabled)
        if (!isDisabled && state.slideAnim > 0.1f)
        {
            Draw::RectGlow(
                pos,
                ImVec2(pos.x + toggleWidth, pos.y + toggleHeight),
                Theme::AccentCyanGlow,
                state.slideAnim * 0.5f,
                4.0f
            );
        }

        // Knob
        float knobPadding = (toggleHeight - knobSize) * 0.5f;
        float knobMinX = pos.x + knobPadding;
        float knobMaxX = pos.x + toggleWidth - knobSize - knobPadding;
        float knobX = knobMinX + (knobMaxX - knobMinX) * state.slideAnim;
        float knobY = pos.y + knobPadding;

        // Knob color with disabled alpha
        int knobAlpha = (int)(255 * alphaMultiplier);
        ImU32 knobColor = IM_COL32(255, 255, 255, knobAlpha);

        draw->AddCircleFilled(
            ImVec2(knobX + knobSize * 0.5f, knobY + knobSize * 0.5f),
            knobSize * 0.5f,
            knobColor
        );

        // Label with disabled alpha
        ImU32 textColor = isDisabled ? Theme::TextMuted : Theme::TextPrimary;
        draw->AddText(
            ImVec2(pos.x + toggleWidth + Theme::ToggleLabelGap, pos.y + (toggleHeight - textSize.y) * 0.5f),
            textColor,
            label
        );

        return clicked && !isDisabled;
    }

    bool ModernToggleWithDesc(const char* label, const char* description, bool* value)
    {
        bool result = ModernToggle(label, value);

        // Description below with text wrapping
        float descOffsetX = 60.0f;
        float availableWidth = ImGui::GetContentRegionAvail().x - descOffsetX;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + descOffsetX);

        // Use TextWrapped for automatic line breaking
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + availableWidth);
        ImGui::PushStyleColor(ImGuiCol_Text, Theme::ToVec4(Theme::TextMuted));
        ImGui::TextWrapped("%s", description);
        ImGui::PopStyleColor();
        ImGui::PopTextWrapPos();

        ImGui::Dummy(ImVec2(0, 4.0f));

        return result;
    }
}
