// =============================================
// EFIGUI Components - Buttons
// GlowButton, IconButton, DangerButton, ColoredButton, CooldownButton
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Helper Functions
    // =============================================

    // Draw cooldown overlay with edge glow and flowing particles
    static void DrawCooldownOverlay(
        ImDrawList* draw,
        ImVec2 pos,
        ImVec2 size,
        float progress,
        float rounding,
        ImU32 glowColor,
        float sweepPos)
    {
        // Resolve values from ButtonTheme
        const auto& t = Theme::Button();
        const int cooldownOverlayAlpha = t.cooldownOverlayAlpha;
        const int edgeGlowLayers = t.edgeGlowLayers;
        const float edgeGlowBaseWidth = t.edgeGlowBaseWidth;
        const float edgeGlowExpandStep = t.edgeGlowExpandStep;
        const float edgeGlowBaseAlpha = t.edgeGlowBaseAlpha;
        const float particleRadiusSmall = t.particleRadiusSmall;
        const float particleRadiusLarge = t.particleRadiusLarge;
        const int particleAlphaBright = t.particleAlphaBright;
        const int particleAlphaDim = t.particleAlphaDim;

        float cooldownWidth = size.x * progress;

        // Dark overlay for cooldown portion (on the LEFT side)
        draw->AddRectFilled(
            pos,
            ImVec2(pos.x + cooldownWidth, pos.y + size.y),
            IM_COL32(0, 0, 0, cooldownOverlayAlpha),
            rounding,
            cooldownWidth >= size.x - rounding ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersLeft
        );

        // Extract RGB from glow color for edge effect
        auto glowRGB = Theme::ExtractRGB(glowColor);

        // Flowing light effect on the right edge of cooldown (the boundary)
        float edgeX = pos.x + cooldownWidth;
        if (edgeX > pos.x + rounding && edgeX < pos.x + size.x - rounding)
        {
            // Vertical gradient line with glow
            for (int i = edgeGlowLayers - 1; i >= 0; i--)
            {
                float glowWidth = edgeGlowBaseWidth + (float)i * edgeGlowExpandStep;
                float alpha = edgeGlowBaseAlpha * (1.0f - (float)i / edgeGlowLayers);
                draw->AddRectFilled(
                    ImVec2(edgeX, pos.y),
                    ImVec2(edgeX + glowWidth, pos.y + size.y),
                    IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, (int)(alpha * 255)),
                    0
                );
            }

            // Flowing light particles along the edge
            float particleY = pos.y + size.y * sweepPos;
            draw->AddCircleFilled(ImVec2(edgeX + 1, particleY), particleRadiusSmall, IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, particleAlphaBright));
            draw->AddCircleFilled(ImVec2(edgeX + 1, particleY), particleRadiusLarge, IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, particleAlphaDim));
        }
    }

    // =============================================
    // Buttons
    // =============================================

    bool GlowButton(const char* label, const GlowButtonConfig& config)
    {
        // Delegate to ButtonConfig version for unified implementation
        ButtonConfig bc;
        bc.size = config.size;
        bc.glowColor = config.glowColor;
        bc.forceHover = config.forceHover;
        bc.layer = config.layer;
        bc.bgAlpha = config.bgAlpha;
        return GlowButton(label, bc);
    }

    bool GlowButton(const char* label, const ButtonConfig& config)
    {
        // Use ButtonConfig with full customization support
        const auto& t = Theme::Button();

        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        // Resolve size from config or calculate from text
        ImVec2 size = config.size;
        float paddingX = config.padding.has_value() ? config.padding->horizontal() : t.padding.horizontal();
        float height = config.size.y > 0 ? config.size.y : t.height;
        if (size.x <= 0) size.x = ImGui::CalcTextSize(label).x + paddingX;
        if (size.y <= 0) size.y = height;

        // Apply min width if specified
        float minWidth = config.minWidth.value_or(t.minWidth);
        if (minWidth > 0 && size.x < minWidth) size.x = minWidth;

        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::InvisibleButton(label, size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();
        bool active = ImGui::IsItemActive();

        bool effectiveHover = hovered || config.forceHover;
        Animation::UpdateWidgetState(state, effectiveHover, active, false);

        if (clicked)
        {
            state.clickAnim = 1.0f;
        }

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Resolve colors
        ImU32 effectiveGlowColor = config.glowColor.value_or(t.glowColor);
        float rounding = config.rounding.value_or(t.rounding);
        int glowLayers = config.glowLayers.value_or(t.glowLayers);
        float glowExpand = config.glowExpand.value_or(t.glowExpand);
        float glowIntensity = config.glowIntensity.value_or(
            config.forceHover ? t.glowIntensityBase : (state.hoverAnim * t.glowIntensityBase + state.activeAnim * t.glowIntensityActive)
        );

        // Glow effect
        Draw::GlowLayers(pos, size, effectiveGlowColor, glowIntensity, glowLayers, glowExpand, rounding, config.layer);

        // Glassmorphism background
        float effectiveHoverAnim = config.forceHover ? 1.0f : state.hoverAnim;
        Draw::GlassmorphismBg(pos, size, rounding, effectiveHoverAnim, active, config.bgAlpha);

        // Marquee border
        float sweepPos = Animation::Sweep(0.12f);
        Draw::MarqueeBorder(pos, size, effectiveGlowColor, sweepPos, 0.22f, rounding, 1.5f, effectiveHoverAnim, config.layer);

        // Text
        ImU32 textColor = config.textColor.value_or(t.textColor);
        ImVec2 textSize = ImGui::CalcTextSize(label);
        ImVec2 textPos = ImVec2(
            pos.x + (size.x - textSize.x) * 0.5f,
            pos.y + (size.y - textSize.y) * 0.5f
        );
        draw->AddText(textPos, textColor, label);

        return clicked;
    }

    bool GlowButton(const char* label, ImVec2 size, std::optional<ImU32> glowColor, bool forceHover, std::optional<Layer> layer, std::optional<uint8_t> bgAlpha)
    {
        // Delegate to ButtonConfig version for unified implementation
        ButtonConfig config;
        config.size = size;
        config.glowColor = glowColor;
        config.forceHover = forceHover;
        config.layer = layer;
        config.bgAlpha = bgAlpha;
        return GlowButton(label, config);
    }

    bool IconButton(const char* icon, ImVec2 size, std::optional<ImU32> color, std::optional<uint8_t> bgAlpha, const char* uniqueId)
    {
        // Use uniqueId if provided, otherwise fall back to icon for ID
        const char* idStr = uniqueId ? uniqueId : icon;
        ImGuiID id = ImGui::GetID(idStr);
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::InvisibleButton(idStr, size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();

        Animation::UpdateWidgetState(state, hovered, false, false);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Get effective color
        ImU32 effectiveColor = color.value_or(Theme::TextSecondary());
        uint8_t effectiveBgAlpha = bgAlpha.value_or(Theme::DefaultBgAlpha());

        // Background on hover
        if (state.hoverAnim > 0.01f)
        {
            ImU32 bgColor = IM_COL32(255, 255, 255, (int)(state.hoverAnim * effectiveBgAlpha));
            draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, Theme::FrameRounding());
        }

        // Icon
        ImU32 iconColor = Animation::LerpColorU32(effectiveColor, Theme::AccentCyan(), state.hoverAnim);
        Draw::IconCentered(pos, ImVec2(pos.x + size.x, pos.y + size.y), icon, iconColor);

        return clicked;
    }

    bool DangerButton(const char* label, ImVec2 size, std::optional<Layer> layer, std::optional<uint8_t> bgAlpha)
    {
        // DangerButton always shows hover effect (forceHover = true)
        return GlowButton(label, size, Theme::StatusError(), true, layer, bgAlpha);
    }

    bool DangerButton(const char* label, const ButtonConfig& config)
    {
        // DangerButton with ButtonConfig - always forceHover and uses StatusError glow
        ButtonConfig dangerConfig = config;
        dangerConfig.forceHover = true;
        if (!dangerConfig.glowColor.has_value())
        {
            dangerConfig.glowColor = Theme::StatusError();
        }
        return GlowButton(label, dangerConfig);
    }

    bool ColoredButton(const char* label, ImVec2 size, ImU32 borderColor, std::optional<uint8_t> bgAlpha, std::optional<Layer> layer)
    {
        // Delegate to ButtonConfig version for unified implementation
        ButtonConfig config;
        config.size = size;
        config.bgAlpha = bgAlpha;
        config.layer = layer;
        return ColoredButton(label, borderColor, config);
    }

    bool ColoredButton(const char* label, ImU32 borderColor, const ButtonConfig& config)
    {
        // ColoredButton with full customization via ButtonConfig
        const auto& t = Theme::Button();

        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        // Resolve size from config
        ImVec2 size = config.size;
        float paddingX = config.padding.has_value() ? config.padding->horizontal() : t.padding.horizontal();
        if (size.x <= 0) size.x = ImGui::CalcTextSize(label).x + paddingX;
        if (size.y <= 0) size.y = t.height;

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
        float rounding = config.rounding.value_or(t.rounding);

        // Extract RGB from border color
        auto borderRGB = Theme::ExtractRGB(borderColor);

        // Get effective background alpha
        uint8_t baseBgAlpha = config.bgAlpha.value_or(Theme::DefaultButtonBgAlpha());
        uint8_t hoverBgAlpha = config.bgAlpha.value_or(Theme::DefaultButtonHoverAlpha());

        // Alpha multiplier for disabled state
        float alphaMult = isDisabled ? 0.35f : 1.0f;

        // Subtle glow effect on hover only (not when disabled)
        if (!isDisabled)
        {
            int glowLayers = config.glowLayers.value_or(t.coloredButtonGlowLayers);
            float glowExpand = config.glowExpand.value_or(t.coloredButtonGlowExpand);
            Draw::GlowLayers(pos, size, borderColor, state.hoverAnim * 0.8f, glowLayers, glowExpand, rounding, config.layer);
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
        ImU32 textColor = isDisabled ? Theme::TextMuted() : config.textColor.value_or(t.textColor);
        draw->AddText(textPos, textColor, label);

        return clicked && !isDisabled;
    }

    bool CooldownButton(const char* label, const CooldownButtonConfig& config)
    {
        return CooldownButton(label, config.size, config.glowColor, config.cooldownProgress, config.layer, config.bgAlpha);
    }

    bool CooldownButton(const char* label, ImVec2 size, ImU32 glowColor, float cooldownProgress, std::optional<Layer> layer, std::optional<uint8_t> bgAlpha)
    {
        // Resolve values from ButtonTheme
        const auto& t = Theme::Button();
        const float defaultPaddingX = t.paddingX;
        const float defaultHeight = t.height;
        const float cooldownThreshold = t.cooldownThreshold;

        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        if (size.x <= 0) size.x = ImGui::CalcTextSize(label).x + defaultPaddingX;
        if (size.y <= 0) size.y = defaultHeight;

        ImVec2 pos = ImGui::GetCursorScreenPos();

        // Button is disabled during cooldown
        bool isOnCooldown = cooldownProgress > cooldownThreshold;
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
        float rounding = Theme::ButtonRounding();

        // Glow effect (outer glow when hovered, NOT on cooldown)
        if (!isOnCooldown)
        {
            Draw::GlowLayers(pos, size, glowColor, state.hoverAnim, Theme::ButtonGlowLayers(), Theme::ButtonGlowExpand(), rounding, layer);
        }

        // Glassmorphism background (skip when on cooldown for simpler look)
        if (!isOnCooldown)
        {
            Draw::GlassmorphismBg(pos, size, rounding, state.hoverAnim, active, bgAlpha);
        }
        else
        {
            // Fallback: solid background when on cooldown
            ImU32 cooldownBg = Theme::ButtonDefault();
            if (bgAlpha.has_value())
            {
                cooldownBg = Theme::ApplyAlpha(cooldownBg, bgAlpha.value());
            }
            draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), cooldownBg, rounding);
        }

        // Cooldown overlay with edge glow and particles
        if (isOnCooldown)
        {
            float sweepPos = Animation::Sweep(0.5f);
            DrawCooldownOverlay(draw, pos, size, cooldownProgress, rounding, glowColor, sweepPos);
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
        ImU32 textColor = isOnCooldown ? Theme::TextMuted() : Theme::TextPrimary();
        draw->AddText(textPos, textColor, label);

        return clicked && !isOnCooldown;
    }
}
