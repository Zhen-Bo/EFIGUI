// =============================================
// EFIGUI Components - Toggle
// Modern toggle switch with optional description
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Toggle Local Constants (non-configurable)
    // =============================================

    namespace ToggleLocal
    {
        // Currently empty - all constants moved to ToggleTheme
        // This namespace exists for future non-configurable values
    }

    // =============================================
    // Toggle / Checkbox (Config version - recommended)
    // =============================================

    bool ModernToggle(const char* label, bool* value, const ToggleConfig& config)
    {
        using namespace ToggleLocal;

        // Resolve config values with Theme defaults
        const auto& t = Theme::Toggle();
        const float toggleWidth = Resolve(config.width, t.width);
        const float toggleHeight = Resolve(config.height, t.height);
        const float knobSize = Resolve(config.knobSize, t.knobSize);
        const float labelGap = Resolve(config.labelGap, t.labelGap);
        const float animSpeed = Resolve(config.animSpeed, t.animSpeed);
        const float glowThreshold = t.glowThreshold;  // Not in ToggleConfig, always from theme
        const float glowRadius = Resolve(config.glowRadius, t.glowRadius);
        const float disabledAlpha = t.disabledAlpha;  // Not in ToggleConfig, always from theme

        // Resolve colors with Theme defaults
        const ImU32 trackOnColor = config.trackOnColor.value_or(Theme::AccentCyan());
        const ImU32 trackOffColor = config.trackOffColor.value_or(Theme::ButtonDefault());
        const ImU32 knobBaseColor = config.knobColor.value_or(Theme::TextPrimary());
        const ImU32 glowColor = config.glowColor.value_or(Theme::AccentCyanGlow());

        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 textSize = ImGui::CalcTextSize(label);

        // Check if we're in a disabled state (from ImGui context or config)
        ImGuiContext& g = *ImGui::GetCurrentContext();
        bool isDisabled = config.disabled || (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;

        // Total area
        float totalWidth = toggleWidth + labelGap + textSize.x;
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
        state.slideAnim = Animation::Lerp(state.slideAnim, targetSlide, animSpeed);

        Animation::UpdateWidgetState(state, hovered && !isDisabled, false, isOn);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Apply disabled alpha multiplier
        float alphaMultiplier = isDisabled ? disabledAlpha : 1.0f;

        // Track background
        ImU32 trackColorBase = Animation::LerpColorU32(
            trackOffColor,
            trackOnColor,
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
        if (!isDisabled && state.slideAnim > glowThreshold)
        {
            Draw::RectGlow(
                pos,
                ImVec2(pos.x + toggleWidth, pos.y + toggleHeight),
                glowColor,
                state.slideAnim * 0.5f,
                glowRadius
            );
        }

        // Knob
        float knobPadding = (toggleHeight - knobSize) * 0.5f;
        float knobMinX = pos.x + knobPadding;
        float knobMaxX = pos.x + toggleWidth - knobSize - knobPadding;
        float knobX = knobMinX + (knobMaxX - knobMinX) * state.slideAnim;
        float knobY = pos.y + knobPadding;

        // Knob color with disabled alpha
        auto knobRGB = Theme::ExtractRGB(knobBaseColor);
        int knobAlpha = (int)(255 * alphaMultiplier);
        ImU32 knobColor = IM_COL32(knobRGB.r, knobRGB.g, knobRGB.b, knobAlpha);

        draw->AddCircleFilled(
            ImVec2(knobX + knobSize * 0.5f, knobY + knobSize * 0.5f),
            knobSize * 0.5f,
            knobColor
        );

        // Label with disabled alpha
        ImU32 textColor = isDisabled ? Theme::TextMuted() : Theme::TextPrimary();
        draw->AddText(
            ImVec2(pos.x + toggleWidth + labelGap, pos.y + (toggleHeight - textSize.y) * 0.5f),
            textColor,
            label
        );

        return clicked && !isDisabled;
    }

    bool ModernToggleWithDesc(const char* label, const char* description, bool* value, const ToggleConfig& config)
    {
        // Resolve description-specific values from theme
        const auto& t = Theme::Toggle();
        const float descOffsetX = t.descOffsetX;
        const float descSpacingY = t.descSpacingY;

        bool result = ModernToggle(label, value, config);

        // Description below with text wrapping
        float availableWidth = ImGui::GetContentRegionAvail().x - descOffsetX;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + descOffsetX);

        // Use TextWrapped for automatic line breaking
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + availableWidth);
        ImGui::PushStyleColor(ImGuiCol_Text, Theme::ToVec4(Theme::TextMuted()));
        ImGui::TextWrapped("%s", description);
        ImGui::PopStyleColor();
        ImGui::PopTextWrapPos();

        ImGui::Dummy(ImVec2(0, descSpacingY));

        return result;
    }

    // =============================================
    // Toggle / Checkbox (legacy API - for backward compatibility)
    // =============================================

    bool ModernToggle(const char* label, bool* value)
    {
        ToggleConfig config;
        return ModernToggle(label, value, config);
    }

    bool ModernToggleWithDesc(const char* label, const char* description, bool* value)
    {
        ToggleConfig config;
        return ModernToggleWithDesc(label, description, value, config);
    }
}
