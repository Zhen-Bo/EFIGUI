// =============================================
// EFIGUI Components - Buttons
// GlowButton, IconButton, DangerButton, ColoredButton, CooldownButton
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Buttons
    // =============================================

    bool GlowButton(const char* label, ImVec2 size, std::optional<ImU32> glowColor, bool forceHover, std::optional<Layer> layer)
    {
        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        if (size.x <= 0) size.x = ImGui::CalcTextSize(label).x + 32.0f;
        if (size.y <= 0) size.y = 36.0f;

        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::InvisibleButton(label, size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();
        bool active = ImGui::IsItemActive();

        // If forceHover is true, treat as always hovered for animation purposes
        bool effectiveHover = hovered || forceHover;
        Animation::UpdateWidgetState(state, effectiveHover, active, false);

        if (clicked)
        {
            state.clickAnim = 1.0f;
        }

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Get effective glow color
        ImU32 effectiveGlowColor = glowColor.value_or(Theme::AccentCyan);

        // Glow effect (outer glow when hovered or forceHover)
        float glowIntensity = forceHover ? 0.6f : (state.hoverAnim * 0.6f + state.activeAnim * 0.4f);
        Draw::GlowLayers(pos, size, effectiveGlowColor, glowIntensity, Theme::ButtonGlowLayers, Theme::ButtonGlowExpand, Theme::ButtonRounding, layer);

        // Glassmorphism background
        float effectiveHoverAnim = forceHover ? 1.0f : state.hoverAnim;
        Draw::GlassmorphismBg(pos, size, Theme::ButtonRounding, effectiveHoverAnim, active);

        // Marquee border
        float sweepPos = Animation::Sweep(0.12f);
        Draw::MarqueeBorder(pos, size, effectiveGlowColor, sweepPos, 0.22f, Theme::ButtonRounding, 1.5f, effectiveHoverAnim, layer);

        // Text
        ImVec2 textSize = ImGui::CalcTextSize(label);
        ImVec2 textPos = ImVec2(
            pos.x + (size.x - textSize.x) * 0.5f,
            pos.y + (size.y - textSize.y) * 0.5f
        );
        draw->AddText(textPos, Theme::TextPrimary, label);

        return clicked;
    }

    bool IconButton(const char* icon, ImVec2 size, std::optional<ImU32> color, std::optional<uint8_t> bgAlpha)
    {
        ImGuiID id = ImGui::GetID(icon);
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::InvisibleButton(icon, size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();

        Animation::UpdateWidgetState(state, hovered, false, false);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Get effective color
        ImU32 effectiveColor = color.value_or(Theme::TextSecondary);
        uint8_t effectiveBgAlpha = bgAlpha.value_or(Theme::DefaultBgAlpha);

        // Background on hover
        if (state.hoverAnim > 0.01f)
        {
            ImU32 bgColor = IM_COL32(255, 255, 255, (int)(state.hoverAnim * effectiveBgAlpha));
            draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, Theme::FrameRounding);
        }

        // Icon
        ImU32 iconColor = Animation::LerpColorU32(effectiveColor, Theme::AccentCyan, state.hoverAnim);
        Draw::IconCentered(pos, ImVec2(pos.x + size.x, pos.y + size.y), icon, iconColor);

        return clicked;
    }

    bool DangerButton(const char* label, ImVec2 size, std::optional<Layer> layer)
    {
        // DangerButton always shows hover effect (forceHover = true)
        return GlowButton(label, size, Theme::StatusError, true, layer);
    }

    bool ColoredButton(const char* label, ImVec2 size, ImU32 borderColor, std::optional<uint8_t> bgAlpha, std::optional<Layer> layer)
    {
        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        if (size.x <= 0) size.x = ImGui::CalcTextSize(label).x + 32.0f;
        if (size.y <= 0) size.y = 36.0f;

        ImVec2 pos = ImGui::GetCursorScreenPos();

        // Check if we're in a disabled state
        ImGuiContext& ctx = *ImGui::GetCurrentContext();
        bool isDisabled = (ctx.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;

        ImGui::InvisibleButton(label, size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();
        bool active = ImGui::IsItemActive();

        Animation::UpdateWidgetState(state, hovered && !isDisabled, active, false);

        ImDrawList* draw = ImGui::GetWindowDrawList();
        float rounding = Theme::ButtonRounding;

        // Extract RGB from border color
        auto borderRGB = Theme::ExtractRGB(borderColor);

        // Get effective background alpha (if provided, override default)
        uint8_t baseBgAlpha = bgAlpha.value_or(Theme::DefaultButtonBgAlpha);
        uint8_t hoverBgAlpha = bgAlpha.value_or(Theme::DefaultButtonHoverAlpha);

        // Alpha multiplier for disabled state
        float alphaMult = isDisabled ? 0.35f : 1.0f;

        // Subtle glow effect on hover only (not when disabled)
        if (!isDisabled)
        {
            Draw::GlowLayers(pos, size, borderColor, state.hoverAnim * 0.8f, Theme::ColoredButtonGlowLayers, Theme::ColoredButtonGlowExpand, rounding, layer);
        }

        // Background - slightly tinted with border color
        ImU32 bgBase = IM_COL32(borderRGB.r / 8 + 20, borderRGB.g / 8 + 20, borderRGB.b / 8 + 30, (int)(baseBgAlpha * alphaMult));
        ImU32 bgHover = IM_COL32(borderRGB.r / 6 + 30, borderRGB.g / 6 + 30, borderRGB.b / 6 + 45, (int)(hoverBgAlpha * alphaMult));
        ImU32 bgColor = isDisabled ? bgBase : Animation::LerpColorU32(bgBase, bgHover, state.hoverAnim);
        if (active && !isDisabled)
        {
            bgColor = IM_COL32(borderRGB.r / 4 + 20, borderRGB.g / 4 + 20, borderRGB.b / 4 + 35, 240);
        }
        draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, rounding);

        // Always show colored border (no marquee animation)
        int borderAlpha = isDisabled ? 80 : (180 + (int)(state.hoverAnim * 75));
        ImU32 finalBorderColor = IM_COL32(
            (int)(borderRGB.r * alphaMult),
            (int)(borderRGB.g * alphaMult),
            (int)(borderRGB.b * alphaMult),
            borderAlpha
        );
        draw->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), finalBorderColor, rounding, 0, 1.5f);

        // Text - dimmer when disabled
        ImVec2 textSize = ImGui::CalcTextSize(label);
        ImVec2 textPos = ImVec2(
            pos.x + (size.x - textSize.x) * 0.5f,
            pos.y + (size.y - textSize.y) * 0.5f
        );
        ImU32 textColor = isDisabled ? Theme::TextMuted : Theme::TextPrimary;
        draw->AddText(textPos, textColor, label);

        return clicked && !isDisabled;
    }

    bool CooldownButton(const char* label, ImVec2 size, ImU32 glowColor, float cooldownProgress, std::optional<Layer> layer)
    {
        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        if (size.x <= 0) size.x = ImGui::CalcTextSize(label).x + 32.0f;
        if (size.y <= 0) size.y = 36.0f;

        ImVec2 pos = ImGui::GetCursorScreenPos();

        // Button is disabled during cooldown
        bool isOnCooldown = cooldownProgress > 0.01f;
        if (isOnCooldown) ImGui::BeginDisabled();

        ImGui::InvisibleButton(label, size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();
        bool active = ImGui::IsItemActive();

        if (isOnCooldown) ImGui::EndDisabled();

        Animation::UpdateWidgetState(state, hovered && !isOnCooldown, active, false);

        if (clicked && !isOnCooldown)
        {
            state.clickAnim = 1.0f;
        }

        ImDrawList* draw = ImGui::GetWindowDrawList();
        float rounding = Theme::ButtonRounding;

        // Glow effect (outer glow when hovered, NOT on cooldown)
        if (!isOnCooldown)
        {
            Draw::GlowLayers(pos, size, glowColor, state.hoverAnim, Theme::ButtonGlowLayers, Theme::ButtonGlowExpand, rounding, layer);
        }

        // Glassmorphism background (skip when on cooldown for simpler look)
        if (!isOnCooldown)
        {
            Draw::GlassmorphismBg(pos, size, rounding, state.hoverAnim, active);
        }
        else
        {
            // Fallback: solid background when on cooldown
            draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), Theme::ButtonDefault, rounding);
        }

        // Cooldown overlay - fills from LEFT to RIGHT as cooldown progresses
        if (isOnCooldown)
        {
            float cooldownWidth = size.x * cooldownProgress;

            // Dark overlay for cooldown portion (on the LEFT side)
            draw->AddRectFilled(
                pos,
                ImVec2(pos.x + cooldownWidth, pos.y + size.y),
                IM_COL32(0, 0, 0, 180),
                rounding,
                cooldownWidth >= size.x - rounding ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersLeft
            );

            // Extract RGB from glow color for edge effect
            auto glowRGB = Theme::ExtractRGB(glowColor);

            // Flowing light effect on the right edge of cooldown (the boundary)
            float sweepPos = Animation::Sweep(0.5f);
            float edgeX = pos.x + cooldownWidth;
            if (edgeX > pos.x + rounding && edgeX < pos.x + size.x - rounding)
            {
                // Vertical gradient line with glow
                for (int i = 3; i >= 0; i--)
                {
                    float glowWidth = 2.0f + (float)i * 3.0f;
                    float alpha = 0.4f * (1.0f - (float)i / 4.0f);
                    draw->AddRectFilled(
                        ImVec2(edgeX, pos.y),
                        ImVec2(edgeX + glowWidth, pos.y + size.y),
                        IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, (int)(alpha * 255)),
                        0
                    );
                }

                // Flowing light particles along the edge
                float particleY = pos.y + size.y * sweepPos;
                draw->AddCircleFilled(ImVec2(edgeX + 1, particleY), 3.0f, IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, 220));
                draw->AddCircleFilled(ImVec2(edgeX + 1, particleY), 6.0f, IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, 100));
            }
        }

        // Marquee border (only when not on cooldown)
        if (!isOnCooldown)
        {
            float sweepPos = Animation::Sweep(0.12f);
            Draw::MarqueeBorder(pos, size, glowColor, sweepPos, 0.22f, rounding, 1.5f, state.hoverAnim, layer);
        }

        // Text
        ImVec2 textSize = ImGui::CalcTextSize(label);
        ImVec2 textPos = ImVec2(
            pos.x + (size.x - textSize.x) * 0.5f,
            pos.y + (size.y - textSize.y) * 0.5f
        );
        ImU32 textColor = isOnCooldown ? Theme::TextMuted : Theme::TextPrimary;
        draw->AddText(textPos, textColor, label);

        return clicked && !isOnCooldown;
    }
}
