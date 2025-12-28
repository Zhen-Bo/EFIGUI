// =============================================
// EFIGUI Components - Navigation
// Sidebar navigation items, section headers, collapse button
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Navigation
    // =============================================

    bool NavItem(const char* icon, const char* label, bool selected, float width, bool collapsed, std::optional<ImU32> accentColor)
    {
        using namespace NavConstants;

        // Use theme default if width is 0
        float effectiveWidth = (width <= 0) ? Theme::NavbarWidth() : width;

        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImVec2(effectiveWidth - Theme::NavItemPadding() * 2, Theme::NavItemHeight());

        ImGui::InvisibleButton(label, size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();

        Animation::UpdateWidgetState(state, hovered, false, selected);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Get effective accent color (use custom or theme default)
        ImU32 effectiveAccent = accentColor.value_or(Theme::AccentCyan());
        auto accentRGB = Theme::ExtractRGB(effectiveAccent);
        ImU32 effectiveAccentGlow = IM_COL32(accentRGB.r, accentRGB.g, accentRGB.b, AccentGlowAlpha);

        // Background
        float bgAlpha = state.hoverAnim * 0.3f + state.selectedAnim * 0.4f;
        if (bgAlpha > 0.01f)
        {
            ImU32 bgColor = IM_COL32(accentRGB.r, accentRGB.g, accentRGB.b, (int)(bgAlpha * BgAlphaMultiplier));
            draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, Theme::NavItemRounding());
        }

        // Left accent bar (when selected)
        if (state.selectedAnim > 0.01f)
        {
            float barHeight = size.y * AccentBarHeightRatio * state.selectedAnim;
            float barY = pos.y + (size.y - barHeight) * 0.5f;

            draw->AddRectFilled(
                ImVec2(pos.x, barY),
                ImVec2(pos.x + AccentBarWidth, barY + barHeight),
                effectiveAccent,
                AccentBarWidth * 0.5f
            );

            // Glow
            Draw::RectGlow(
                ImVec2(pos.x, barY),
                ImVec2(pos.x + AccentBarWidth, barY + barHeight),
                effectiveAccentGlow,
                state.selectedAnim,
                AccentBarGlowRadius
            );
        }

        // Icon - center it when collapsed, scale up when collapsed
        float baseFontSize = ImGui::GetFontSize();
        float iconSize = collapsed ? baseFontSize * CollapsedIconScale : baseFontSize;
        float iconX = collapsed ? pos.x + (size.x - iconSize) * 0.5f : pos.x + IconPadding;
        float iconY = pos.y + (size.y - iconSize) * 0.5f;

        // Use max to prevent flash when transitioning between hover and selected states
        float hoverContrib = state.hoverAnim * 0.5f;
        float iconLerpFactor = state.selectedAnim > hoverContrib ? state.selectedAnim : hoverContrib;
        ImU32 iconColor = Animation::LerpColorU32(
            Theme::TextSecondary(),
            effectiveAccent,
            iconLerpFactor
        );

        if (collapsed)
        {
            // Draw larger icon when collapsed
            draw->AddText(ImGui::GetFont(), iconSize, ImVec2(iconX, iconY), iconColor, icon);
        }
        else
        {
            draw->AddText(ImVec2(iconX, iconY), iconColor, icon);
        }

        // Label (only if not collapsed)
        if (!collapsed)
        {
            float labelX = pos.x + IconPadding + ImGui::GetFontSize() + LabelSpacing;

            // Use max to prevent flash when transitioning between hover and selected states
            float labelHoverContrib = state.hoverAnim * 0.5f;
            float labelLerpFactor = state.selectedAnim > labelHoverContrib ? state.selectedAnim : labelHoverContrib;
            ImU32 labelColor = Animation::LerpColorU32(
                Theme::TextSecondary(),
                Theme::TextPrimary(),
                labelLerpFactor
            );
            draw->AddText(ImVec2(labelX, iconY), labelColor, label);
        }

        // Tooltip when collapsed
        if (collapsed && hovered)
        {
            ImGui::SetTooltip("%s", label);
        }

        return clicked;
    }

    bool NavCollapseButton(bool collapsed, float width)
    {
        using namespace NavConstants;

        const char* icon = collapsed ? "\xef\x81\x94" : "\xef\x81\x93";  // chevron-right : chevron-left
        const char* label = collapsed ? "Expand" : "Collapse";

        ImGuiID id = ImGui::GetID("##NavCollapseBtn");
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImVec2(width - Theme::NavItemPadding() * 2, Theme::NavItemHeight());

        ImGui::InvisibleButton("##NavCollapseBtn", size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();

        Animation::UpdateWidgetState(state, hovered, false, false);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Background on hover
        if (state.hoverAnim > 0.01f)
        {
            ImU32 bgColor = IM_COL32(255, 255, 255, (int)(state.hoverAnim * HoverBgAlpha));
            draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, Theme::NavItemRounding());
        }

        // Icon - center it
        float iconX = pos.x + (size.x - ImGui::GetFontSize()) * 0.5f;
        float iconY = pos.y + (size.y - ImGui::GetFontSize()) * 0.5f;
        ImU32 iconColor = Animation::LerpColorU32(
            Theme::TextMuted(),
            Theme::TextPrimary(),
            state.hoverAnim
        );
        draw->AddText(ImVec2(iconX, iconY), iconColor, icon);

        // Tooltip
        if (hovered)
        {
            ImGui::SetTooltip("%s", label);
        }

        return clicked;
    }

    void NavSectionHeader(const char* label)
    {
        using namespace NavConstants;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        draw->AddText(
            ImVec2(pos.x + SectionPaddingX, pos.y + SectionPaddingY),
            Theme::TextMuted(),
            label
        );

        ImGui::Dummy(ImVec2(0, SectionHeight));
    }
}
