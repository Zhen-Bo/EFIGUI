#include "Components.h"
#include "Draw.h"
#include "EFIGUI.h"
#include "imgui_internal.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <optional>

namespace EFIGUI
{
    // =============================================
    // Internal State
    // =============================================

    static bool s_isDraggingTitleBar = false;
    static ImVec2 s_dragOffset = ImVec2(0, 0);

    // =============================================
    // Custom Title Bar
    // =============================================

    bool BeginCustomWindow(const char* name, bool* p_open, ImGuiWindowFlags flags)
    {
        // Add flags for no title bar (we draw our own)
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowSizeConstraints(ImVec2(Theme::WindowMinWidth, Theme::WindowMinHeight), ImVec2(FLT_MAX, FLT_MAX));

        if (!ImGui::Begin(name, p_open, flags))
        {
            ImGui::End();
            return false;
        }

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        float titleHeight = Theme::TitleBarHeight;

        // Title bar background (gradient)
        ImVec2 titleMin = windowPos;
        ImVec2 titleMax = ImVec2(windowPos.x + windowSize.x, windowPos.y + titleHeight);
        Draw::RectGradientH(titleMin, titleMax, Theme::TitleBarLeft, Theme::TitleBarRight, Theme::WindowRounding);

        // Only round top corners
        draw->AddRectFilled(
            ImVec2(titleMin.x, titleMin.y + Theme::WindowRounding),
            ImVec2(titleMax.x, titleMax.y),
            Theme::TitleBarLeft
        );

        // Neon accent line at bottom of title
        draw->AddLine(
            ImVec2(titleMin.x, titleMax.y),
            ImVec2(titleMax.x, titleMax.y),
            Theme::AccentCyan,
            1.0f
        );

        // Title text
        ImVec2 textPos = ImVec2(windowPos.x + 16.0f, windowPos.y + (titleHeight - ImGui::GetFontSize()) * 0.5f);
        draw->AddText(textPos, Theme::TextPrimary, name);

        // Close button
        float btnSize = 24.0f;
        float btnY = windowPos.y + (titleHeight - btnSize) * 0.5f;
        float closeX = windowPos.x + windowSize.x - btnSize - 12.0f;

        ImGui::SetCursorScreenPos(ImVec2(closeX, btnY));
        ImGui::PushID("##close");

        bool closeHovered = ImGui::IsMouseHoveringRect(
            ImVec2(closeX, btnY),
            ImVec2(closeX + btnSize, btnY + btnSize)
        );

        ImU32 closeColor = closeHovered ? Theme::StatusError : Theme::TextSecondary;
        if (closeHovered && ImGui::IsMouseClicked(0))
        {
            if (p_open) *p_open = false;
        }

        Draw::IconCentered(
            ImVec2(closeX, btnY),
            ImVec2(closeX + btnSize, btnY + btnSize),
            Icons::Close,
            closeColor
        );

        ImGui::PopID();

        // Minimize button
        float minX = closeX - btnSize - 8.0f;
        ImGui::PushID("##minimize");

        bool minHovered = ImGui::IsMouseHoveringRect(
            ImVec2(minX, btnY),
            ImVec2(minX + btnSize, btnY + btnSize)
        );

        ImU32 minColor = minHovered ? Theme::AccentCyan : Theme::TextSecondary;
        Draw::IconCentered(
            ImVec2(minX, btnY),
            ImVec2(minX + btnSize, btnY + btnSize),
            Icons::Minimize,
            minColor
        );

        ImGui::PopID();

        // Title bar dragging
        bool inTitleBar = ImGui::IsMouseHoveringRect(titleMin, ImVec2(minX - 8.0f, titleMax.y));

        if (inTitleBar && ImGui::IsMouseClicked(0))
        {
            s_isDraggingTitleBar = true;
            s_dragOffset = ImVec2(
                ImGui::GetMousePos().x - windowPos.x,
                ImGui::GetMousePos().y - windowPos.y
            );
        }

        if (s_isDraggingTitleBar)
        {
            if (ImGui::IsMouseDown(0))
            {
                ImVec2 newPos = ImVec2(
                    ImGui::GetMousePos().x - s_dragOffset.x,
                    ImGui::GetMousePos().y - s_dragOffset.y
                );
                ImGui::SetWindowPos(newPos);
            }
            else
            {
                s_isDraggingTitleBar = false;
            }
        }

        // Set cursor below title bar for content
        ImGui::SetCursorPosY(titleHeight);

        return true;
    }

    void EndCustomWindow()
    {
        ImGui::End();
    }

    // =============================================
    // Borderless Window
    // =============================================

    bool BeginBorderlessWindow(const char* name, bool* p_open, ImGuiWindowFlags flags, std::optional<uint8_t> overlayAlpha)
    {
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoCollapse;
        flags |= ImGuiWindowFlags_NoBackground;
        flags |= ImGuiWindowFlags_NoResize;  // We handle resize manually (no top edge)

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, Theme::WindowRounding);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

        if (!ImGui::Begin(name, p_open, flags))
        {
            ImGui::PopStyleColor();
            ImGui::PopStyleVar(2);
            ImGui::End();
            return false;
        }

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);

        // Draw window background with rounded corners
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();

        // Determine overlay alpha (nullopt = use Theme default)
        uint8_t effectiveOverlayAlpha = overlayAlpha.value_or(Theme::DefaultOverlayAlpha);

        // Try to use blurred background for glassmorphism effect
        void* blurredBg = GetBlurResult();
        if (blurredBg)
        {
            // Calculate UV coordinates for window region
            ImGuiIO& io = ImGui::GetIO();
            float screenW = io.DisplaySize.x;
            float screenH = io.DisplaySize.y;

            ImVec2 uv0(windowPos.x / screenW, windowPos.y / screenH);
            ImVec2 uv1((windowPos.x + windowSize.x) / screenW, (windowPos.y + windowSize.y) / screenH);

            // Draw the blurred background
            draw->AddImageRounded(
                (ImTextureID)blurredBg,
                windowPos,
                ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
                uv0, uv1,
                IM_COL32(255, 255, 255, 255),
                Theme::WindowRounding
            );

            // Dark overlay for better contrast and cyberpunk look
            draw->AddRectFilled(
                windowPos,
                ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
                IM_COL32(10, 10, 20, effectiveOverlayAlpha),
                Theme::WindowRounding
            );
        }
        else
        {
            // Fallback: solid dark background
            draw->AddRectFilled(
                windowPos,
                ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
                Theme::BackgroundDark,
                Theme::WindowRounding
            );
        }

        // Subtle border
        draw->AddRect(
            windowPos,
            ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
            Theme::BorderDefault,
            Theme::WindowRounding,
            0,
            1.0f
        );

        return true;
    }

    void EndBorderlessWindow()
    {
        ImGui::End();
    }

    // =============================================
    // Navbar Header
    // =============================================

    bool NavbarHeader(const char* title, const char* iconExpanded, const char* iconCollapsed,
                      bool collapsed, float width, bool* closeClicked, std::optional<ImU32> glowColor)
    {
        ImGuiID id = ImGui::GetID("##NavbarHeader");
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float headerHeight = Theme::TitleBarHeight;
        ImVec2 size = ImVec2(width - Theme::NavItemPadding * 2, headerHeight);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Get effective glow color
        ImU32 effectiveGlowColor = glowColor.value_or(Theme::AccentCyan);
        auto glowRGB = Theme::ExtractRGB(effectiveGlowColor);

        // Header background (gradient)
        Draw::RectGradientH(pos, ImVec2(pos.x + size.x, pos.y + size.y),
                           Theme::TitleBarLeft, Theme::TitleBarRight, Theme::NavItemRounding);

        // Bottom accent line
        draw->AddLine(
            ImVec2(pos.x, pos.y + size.y),
            ImVec2(pos.x + size.x, pos.y + size.y),
            effectiveGlowColor,
            1.0f
        );

        float btnSize = 20.0f;
        float contentY = pos.y + (headerHeight - ImGui::GetFontSize()) * 0.5f;

        if (collapsed)
        {
            // Collapsed: show glowing icon (clickable to expand)
            float iconSize = ImGui::GetFontSize();
            float iconX = pos.x + (size.x - iconSize) * 0.5f;
            float iconY = pos.y + (headerHeight - iconSize) * 0.5f;

            // Make the entire header clickable
            ImGui::SetCursorScreenPos(pos);
            ImGui::InvisibleButton("##expandHeader", size);
            bool headerHovered = ImGui::IsItemHovered();
            bool headerClicked = ImGui::IsItemClicked();

            // Glowing icon with slow pulse (0.5 Hz = 2 seconds per cycle)
            float glowAnim = Animation::Breathe(0.5f);
            ImU32 iconColor;
            if (headerHovered)
            {
                // Bright color when hovered
                iconColor = effectiveGlowColor;
            }
            else
            {
                // Pulsing glow effect
                int glowAlpha = (int)(Theme::DefaultGlowBaseAlpha + glowAnim * Theme::DefaultGlowMaxAlpha);
                iconColor = IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, glowAlpha);
            }

            // Draw glow behind icon
            if (!headerHovered)
            {
                float glowRadius = 4.0f + glowAnim * 2.0f;
                ImU32 glowColorWithAlpha = IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, (int)(glowAnim * 60));
                draw->AddCircleFilled(
                    ImVec2(iconX + iconSize * 0.5f, iconY + iconSize * 0.5f),
                    glowRadius + iconSize * 0.5f,
                    glowColorWithAlpha
                );
            }

            draw->AddText(ImVec2(iconX, iconY), iconColor, iconCollapsed);

            // Tooltip hint
            if (headerHovered)
            {
                ImGui::SetTooltip("Click to expand");
            }

            // Advance cursor
            ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + headerHeight + 8.0f));

            return headerClicked;
        }
        else
        {
            // Expanded: show icon + title + << button
            float iconX = pos.x + 8.0f;

            // Brand icon
            draw->AddText(ImVec2(iconX, contentY), effectiveGlowColor, iconExpanded);

            // Title text
            float textX = iconX + ImGui::GetFontSize() + 8.0f;
            draw->AddText(ImVec2(textX, contentY), Theme::TextPrimary, title);

            // Collapse button on far right
            float collapseX = pos.x + size.x - btnSize - 4.0f;
            float btnY = pos.y + (headerHeight - btnSize) * 0.5f;

            ImGui::SetCursorScreenPos(ImVec2(collapseX, btnY));
            ImGui::InvisibleButton("##collapse", ImVec2(btnSize, btnSize));
            bool collapseHovered = ImGui::IsItemHovered();
            bool collapseClicked = ImGui::IsItemClicked();

            ImU32 collapseColor = collapseHovered ? effectiveGlowColor : Theme::TextSecondary;
            Draw::IconCentered(
                ImVec2(collapseX, btnY),
                ImVec2(collapseX + btnSize, btnY + btnSize),
                Icons::Collapse,
                collapseColor
            );

            // Advance cursor
            ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + headerHeight + 8.0f));

            return collapseClicked;
        }
    }

    // =============================================
    // Glass Panel
    // =============================================

    void BeginGlassPanel(const char* id, ImVec2 size, bool border, std::optional<uint8_t> bgAlpha, std::optional<ImU32> glowColor)
    {
        ImGui::PushID(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();

        if (size.x <= 0) size.x = ImGui::GetContentRegionAvail().x;
        if (size.y <= 0) size.y = ImGui::GetContentRegionAvail().y;

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Determine alpha to use (nullopt = use Theme default)
        uint8_t alpha = bgAlpha.value_or((Theme::BackgroundContent >> 24) & 0xFF);

        // Extract RGB from Theme color and apply custom alpha
        ImU32 bgColor = (Theme::BackgroundContent & 0x00FFFFFF) | ((ImU32)alpha << 24);

        // Simple solid background (blur effect is on main window only)
        draw->AddRectFilled(
            pos,
            ImVec2(pos.x + size.x, pos.y + size.y),
            bgColor,
            Theme::FrameRounding
        );

        if (border)
        {
            // Subtle border
            draw->AddRect(
                pos,
                ImVec2(pos.x + size.x, pos.y + size.y),
                Theme::BorderDefault,
                Theme::FrameRounding,
                0,
                1.0f
            );

            // Glow effect on top edge - use custom or theme color
            ImU32 effectiveGlowColor = glowColor.value_or(Theme::AccentCyan);
            float glowIntensity = Animation::Breathe(1.0f) * 0.3f + 0.2f;
            auto glowRGB = Theme::ExtractRGB(effectiveGlowColor);
            ImU32 glowColorWithAlpha = IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, (int)(glowIntensity * 80));
            draw->AddLine(
                ImVec2(pos.x + Theme::FrameRounding, pos.y),
                ImVec2(pos.x + size.x - Theme::FrameRounding, pos.y),
                glowColorWithAlpha,
                2.0f
            );
        }

        ImGui::BeginChild(id, size, false, ImGuiWindowFlags_NoBackground);

        // Apply internal padding by creating an inner child with margins
        float padding = Theme::ContentPadding;
        ImVec2 innerSize = ImVec2(size.x - padding * 2, size.y - padding * 2);
        ImGui::SetCursorPos(ImVec2(padding, padding));
        ImGui::BeginChild("##GlassPanelInner", innerSize, false, ImGuiWindowFlags_NoBackground);
    }

    void EndGlassPanel()
    {
        ImGui::EndChild();  // End inner padded child
        ImGui::EndChild();  // End outer glass panel child
        ImGui::PopID();
    }

    // =============================================
    // Navigation
    // =============================================

    bool NavItem(const char* icon, const char* label, bool selected, float width, bool collapsed, std::optional<ImU32> accentColor)
    {
        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImVec2(width - Theme::NavItemPadding * 2, Theme::NavItemHeight);

        ImGui::InvisibleButton(label, size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();

        Animation::UpdateWidgetState(state, hovered, false, selected);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Get effective accent color (use custom or theme default)
        ImU32 effectiveAccent = accentColor.value_or(Theme::AccentCyan);
        auto accentRGB = Theme::ExtractRGB(effectiveAccent);
        ImU32 effectiveAccentGlow = IM_COL32(accentRGB.r, accentRGB.g, accentRGB.b, 80);

        // Background
        float bgAlpha = state.hoverAnim * 0.3f + state.selectedAnim * 0.4f;
        if (bgAlpha > 0.01f)
        {
            ImU32 bgColor = IM_COL32(accentRGB.r, accentRGB.g, accentRGB.b, (int)(bgAlpha * 60));
            draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, Theme::NavItemRounding);
        }

        // Left accent bar (when selected)
        if (state.selectedAnim > 0.01f)
        {
            float barWidth = 3.0f;
            float barHeight = size.y * 0.6f * state.selectedAnim;
            float barY = pos.y + (size.y - barHeight) * 0.5f;

            draw->AddRectFilled(
                ImVec2(pos.x, barY),
                ImVec2(pos.x + barWidth, barY + barHeight),
                effectiveAccent,
                barWidth * 0.5f
            );

            // Glow
            Draw::RectGlow(
                ImVec2(pos.x, barY),
                ImVec2(pos.x + barWidth, barY + barHeight),
                effectiveAccentGlow,
                state.selectedAnim,
                4.0f
            );
        }

        // Icon - center it when collapsed, scale up 1.25x when collapsed
        float baseFontSize = ImGui::GetFontSize();
        float iconSize = collapsed ? baseFontSize * 1.25f : baseFontSize;
        float iconX = collapsed ? pos.x + (size.x - iconSize) * 0.5f : pos.x + 12.0f;
        float iconY = pos.y + (size.y - iconSize) * 0.5f;

        // Use max to prevent flash when transitioning between hover and selected states
        float hoverContrib = state.hoverAnim * 0.5f;
        float iconLerpFactor = state.selectedAnim > hoverContrib ? state.selectedAnim : hoverContrib;
        ImU32 iconColor = Animation::LerpColorU32(
            Theme::TextSecondary,
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
            float labelX = pos.x + 12.0f + ImGui::GetFontSize() + 12.0f;

            // Use max to prevent flash when transitioning between hover and selected states
            float labelHoverContrib = state.hoverAnim * 0.5f;
            float labelLerpFactor = state.selectedAnim > labelHoverContrib ? state.selectedAnim : labelHoverContrib;
            ImU32 labelColor = Animation::LerpColorU32(
                Theme::TextSecondary,
                Theme::TextPrimary,
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
        const char* icon = collapsed ? "\xef\x81\x94" : "\xef\x81\x93";  // chevron-right : chevron-left
        const char* label = collapsed ? "Expand" : "Collapse";

        ImGuiID id = ImGui::GetID("##NavCollapseBtn");
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 size = ImVec2(width - Theme::NavItemPadding * 2, Theme::NavItemHeight);

        ImGui::InvisibleButton("##NavCollapseBtn", size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();

        Animation::UpdateWidgetState(state, hovered, false, false);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Background on hover
        if (state.hoverAnim > 0.01f)
        {
            ImU32 bgColor = IM_COL32(255, 255, 255, (int)(state.hoverAnim * 30));
            draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, Theme::NavItemRounding);
        }

        // Icon - center it
        float iconX = pos.x + (size.x - ImGui::GetFontSize()) * 0.5f;
        float iconY = pos.y + (size.y - ImGui::GetFontSize()) * 0.5f;
        ImU32 iconColor = Animation::LerpColorU32(
            Theme::TextMuted,
            Theme::TextPrimary,
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
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        draw->AddText(
            ImVec2(pos.x + 12.0f, pos.y + 4.0f),
            Theme::TextMuted,
            label
        );

        ImGui::Dummy(ImVec2(0, 24.0f));
    }

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

    // =============================================
    // Input
    // =============================================

    bool ModernInputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::ToVec4(Theme::ButtonDefault));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Theme::ToVec4(Theme::ButtonHover));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Theme::ToVec4(Theme::ButtonHover));
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::ToVec4(Theme::BorderDefault));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Theme::FrameRounding);

        bool result = ImGui::InputText(label, buf, buf_size, flags);

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);

        return result;
    }

    bool ModernInputTextMultiline(const char* label, char* buf, size_t buf_size, ImVec2 size, ImGuiInputTextFlags flags)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::ToVec4(Theme::ButtonDefault));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Theme::ToVec4(Theme::ButtonHover));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Theme::ToVec4(Theme::ButtonHover));

        bool result = ImGui::InputTextMultiline(label, buf, buf_size, size, flags);

        ImGui::PopStyleColor(3);

        return result;
    }

    // Simple callback for std::string auto-resize only
    static int StringInputTextCallback(ImGuiInputTextCallbackData* data)
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            std::string* str = (std::string*)data->UserData;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);
            data->Buf = (char*)str->c_str();
        }
        return 0;
    }

    bool ModernInputText(const char* label, std::string* str, ImGuiInputTextFlags flags)
    {
        if (str->capacity() < 256)
            str->reserve(256);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::ToVec4(Theme::ButtonDefault));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Theme::ToVec4(Theme::ButtonHover));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Theme::ToVec4(Theme::ButtonHover));
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::ToVec4(Theme::BorderDefault));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Theme::FrameRounding);

        flags |= ImGuiInputTextFlags_CallbackResize;
        bool result = ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, StringInputTextCallback, (void*)str);

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);

        return result;
    }

    bool ModernInputTextMultiline(const char* label, std::string* str, ImVec2 size, ImGuiInputTextFlags flags)
    {
        if (str->capacity() < 256)
            str->reserve(256);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::ToVec4(Theme::ButtonDefault));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Theme::ToVec4(Theme::ButtonHover));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Theme::ToVec4(Theme::ButtonHover));

        flags |= ImGuiInputTextFlags_CallbackResize;
        bool result = ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, StringInputTextCallback, (void*)str);

        ImGui::PopStyleColor(3);

        return result;
    }

    // =============================================
    // Slider
    // =============================================

    // Static storage for slider input editing state
    static std::unordered_map<ImGuiID, std::string> s_sliderInputBuffers;
    static ImGuiID s_activeSliderInputId = 0;

    bool ModernSliderFloat(const char* label, float* value, float min, float max, const char* format, std::optional<Layer> layer)
    {
        ImGuiID id = ImGui::GetID(label);
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float sliderHeight = 24.0f;
        float trackHeight = 6.0f;
        float knobRadius = 8.0f;

        // Check if label is hidden (starts with ##)
        bool labelHidden = (label[0] == '#' && label[1] == '#');
        float labelWidth = 0.0f;
        ImVec2 textSize = ImVec2(0, 0);

        if (!labelHidden)
        {
            textSize = ImGui::CalcTextSize(label);
            labelWidth = textSize.x + 12.0f;
        }

        // Parse format string to extract precision
        int precision = 1;
        if (format)
        {
            const char* p = format;
            while (*p)
            {
                if (*p == '.' && *(p + 1) >= '0' && *(p + 1) <= '9')
                {
                    precision = *(p + 1) - '0';
                    break;
                }
                p++;
            }
        }

        // Reserve space
        float valueInputWidth = 55.0f;
        float valueInputHeight = sliderHeight - 2.0f;
        float knobPadding = knobRadius + 2.0f;
        float inputGap = 8.0f;
        float sliderWidth = ImGui::GetContentRegionAvail().x - labelWidth - valueInputWidth - knobPadding * 2 - inputGap;
        if (sliderWidth < 80.0f) sliderWidth = 80.0f;

        bool changed = false;
        bool isEditingThisSlider = (s_activeSliderInputId == id);

        // --- Draw label first ---
        ImDrawList* draw = ImGui::GetWindowDrawList();
        if (!labelHidden)
        {
            draw->AddText(ImVec2(pos.x, pos.y + (sliderHeight - textSize.y) * 0.5f), Theme::TextPrimary, label);
        }

        // --- Slider track area ---
        float sliderStartX = pos.x + labelWidth + knobPadding;
        float trackY = pos.y + (sliderHeight - trackHeight) * 0.5f;

        // Create invisible button for slider interaction
        ImGui::SetCursorScreenPos(ImVec2(sliderStartX - knobPadding, pos.y));
        ImGui::InvisibleButton((std::string(label) + "_slider").c_str(), ImVec2(sliderWidth + knobPadding * 2, sliderHeight));
        bool sliderHovered = ImGui::IsItemHovered();
        bool sliderActive = ImGui::IsItemActive();

        Animation::UpdateWidgetState(state, sliderHovered, sliderActive, false);

        // Handle slider drag
        if (sliderActive && value)
        {
            float mouseX = ImGui::GetMousePos().x;
            float sliderEndX = sliderStartX + sliderWidth;

            float clampedMouseX = mouseX;
            if (clampedMouseX < sliderStartX) clampedMouseX = sliderStartX;
            if (clampedMouseX > sliderEndX) clampedMouseX = sliderEndX;

            float normalizedValue = (clampedMouseX - sliderStartX) / sliderWidth;
            float newValue = min + normalizedValue * (max - min);

            if (newValue != *value)
            {
                *value = newValue;
                changed = true;

                // Update input buffer while dragging
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(precision) << *value;
                s_sliderInputBuffers[id] = oss.str();
            }
        }

        // Ensure value is always clamped
        if (value)
        {
            if (*value < min) *value = min;
            if (*value > max) *value = max;
        }

        // --- Draw slider track ---
        draw->AddRectFilled(
            ImVec2(sliderStartX, trackY),
            ImVec2(sliderStartX + sliderWidth, trackY + trackHeight),
            Theme::ButtonDefault,
            trackHeight * 0.5f
        );

        // Calculate fill
        float normalizedCurrent = value ? (*value - min) / (max - min) : 0.0f;
        if (normalizedCurrent < 0.0f) normalizedCurrent = 0.0f;
        if (normalizedCurrent > 1.0f) normalizedCurrent = 1.0f;
        float fillWidth = sliderWidth * normalizedCurrent;

        // Track fill with gradient
        if (fillWidth > 0)
        {
            Draw::RectGradientH(
                ImVec2(sliderStartX, trackY),
                ImVec2(sliderStartX + fillWidth, trackY + trackHeight),
                Theme::AccentPurple,
                Theme::AccentCyan,
                trackHeight * 0.5f
            );

            // Glow effect
            if (state.hoverAnim > 0.1f || sliderActive)
            {
                float glowIntensity = sliderActive ? 0.8f : state.hoverAnim * 0.5f;
                Draw::RectGlow(
                    ImVec2(sliderStartX, trackY),
                    ImVec2(sliderStartX + fillWidth, trackY + trackHeight),
                    Theme::AccentCyanGlow,
                    glowIntensity,
                    4.0f
                );
            }
        }

        // --- Draw knob ---
        float knobX = sliderStartX + fillWidth;
        float knobY = pos.y + sliderHeight * 0.5f;

        // Knob glow effect
        if (state.hoverAnim > 0.1f || sliderActive)
        {
            float glowIntensity = sliderActive ? 0.8f : state.hoverAnim * 0.5f;
            Draw::GlowLayersCircle(ImVec2(knobX, knobY), knobRadius + 2.0f, Theme::AccentCyan, glowIntensity, Theme::SliderGlowLayers, Theme::SliderKnobGlowExpand, layer);
        }

        ImU32 knobColor = sliderActive ? Theme::AccentCyan : Theme::TextPrimary;
        draw->AddCircleFilled(ImVec2(knobX, knobY), knobRadius, knobColor);

        // --- Modern Display Box for Value ---
        float inputX = sliderStartX + sliderWidth + knobPadding + inputGap;
        float inputY = pos.y + (sliderHeight - valueInputHeight) * 0.5f;
        float inputRounding = 6.0f;

        // Initialize buffer if needed
        if (s_sliderInputBuffers.find(id) == s_sliderInputBuffers.end())
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(precision) << (value ? *value : 0.0f);
            s_sliderInputBuffers[id] = oss.str();
        }

        // Sync buffer when value changes externally (not editing)
        if (!isEditingThisSlider && value)
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(precision) << *value;
            std::string expectedValue = oss.str();
            if (s_sliderInputBuffers[id] != expectedValue)
            {
                s_sliderInputBuffers[id] = expectedValue;
            }
        }

        ImVec2 inputBoxMin = ImVec2(inputX, inputY);
        ImVec2 inputBoxMax = ImVec2(inputX + valueInputWidth, inputY + valueInputHeight);

        // Check hover/active state for the display box
        ImGui::SetCursorScreenPos(inputBoxMin);
        ImGui::InvisibleButton((std::string(label) + "_display").c_str(), ImVec2(valueInputWidth, valueInputHeight));
        bool displayHovered = ImGui::IsItemHovered();
        bool displayClicked = ImGui::IsItemClicked();

        // Get animation state for this display box
        ImGuiID displayId = ImGui::GetID((std::string(label) + "_display").c_str());
        Animation::WidgetState& displayState = Animation::GetState(displayId);
        Animation::UpdateWidgetState(displayState, displayHovered, isEditingThisSlider, false);

        // Draw glow effect FIRST (before background)
        if (displayState.hoverAnim > 0.05f || isEditingThisSlider)
        {
            float glowIntensity = isEditingThisSlider ? 0.7f : displayState.hoverAnim * 0.5f;
            ImVec2 boxSize = ImVec2(valueInputWidth, valueInputHeight);
            Draw::GlowLayers(inputBoxMin, boxSize, Theme::AccentCyan, glowIntensity, Theme::SliderGlowLayers, Theme::SliderInputGlowExpand, inputRounding, layer);
        }

        // Draw the display box background
        ImU32 bezelColor = IM_COL32(25, 25, 35, 255);
        draw->AddRectFilled(inputBoxMin, inputBoxMax, bezelColor, inputRounding);

        // Inner screen area (slightly inset)
        float inset = 2.0f;
        ImVec2 screenMin = ImVec2(inputBoxMin.x + inset, inputBoxMin.y + inset);
        ImVec2 screenMax = ImVec2(inputBoxMax.x - inset, inputBoxMax.y - inset);

        // Screen background
        ImU32 screenBg = isEditingThisSlider ? IM_COL32(20, 30, 40, 255) :
                         Animation::LerpColorU32(
                             IM_COL32(15, 15, 25, 255),
                             IM_COL32(20, 25, 35, 255),
                             displayState.hoverAnim
                         );
        draw->AddRectFilled(screenMin, screenMax, screenBg, inputRounding - inset);

        // Subtle inner border/bevel effect
        ImU32 innerBorderTop = IM_COL32(10, 10, 15, 255);
        ImU32 innerBorderBottom = IM_COL32(50, 50, 70, 100);
        draw->AddLine(ImVec2(screenMin.x + inputRounding, screenMin.y),
                      ImVec2(screenMax.x - inputRounding, screenMin.y),
                      innerBorderTop, 1.0f);
        draw->AddLine(ImVec2(screenMin.x + inputRounding, screenMax.y),
                      ImVec2(screenMax.x - inputRounding, screenMax.y),
                      innerBorderBottom, 1.0f);

        // Outer border
        ImU32 borderColor = isEditingThisSlider ? Theme::AccentCyan :
                           Animation::LerpColorU32(
                               IM_COL32(60, 60, 80, 255),
                               IM_COL32(80, 100, 120, 255),
                               displayState.hoverAnim
                           );
        draw->AddRect(inputBoxMin, inputBoxMax, borderColor, inputRounding, 0, 1.0f);

        // Now handle the actual input/display
        ImGui::SetCursorScreenPos(inputBoxMin);

        if (isEditingThisSlider)
        {
            // Editing mode
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Text, Theme::ToVec4(Theme::AccentCyan));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, inputRounding);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, (valueInputHeight - ImGui::GetFontSize()) * 0.5f));

            ImGui::SetNextItemWidth(valueInputWidth);

            char inputBuf[32];
            strncpy(inputBuf, s_sliderInputBuffers[id].c_str(), sizeof(inputBuf) - 1);
            inputBuf[sizeof(inputBuf) - 1] = '\0';

            std::string inputId = std::string("##") + label + "_input";
            if (ImGui::InputText(inputId.c_str(), inputBuf, sizeof(inputBuf),
                ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
            {
                // Enter pressed - apply value
                try
                {
                    float newVal = std::stof(inputBuf);
                    if (newVal < min) newVal = min;
                    if (newVal > max) newVal = max;
                    if (value && newVal != *value)
                    {
                        *value = newVal;
                        changed = true;
                    }
                }
                catch (...) { /* Invalid input, ignore */ }

                // Update buffer to formatted value
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(precision) << (value ? *value : 0.0f);
                s_sliderInputBuffers[id] = oss.str();
                s_activeSliderInputId = 0;
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
            else if (!ImGui::IsItemActive())
            {
                // Lost focus - apply value
                try
                {
                    float newVal = std::stof(s_sliderInputBuffers[id]);
                    if (newVal < min) newVal = min;
                    if (newVal > max) newVal = max;
                    if (value && newVal != *value)
                    {
                        *value = newVal;
                        changed = true;
                    }
                }
                catch (...) { /* Invalid input, ignore */ }

                std::ostringstream oss;
                oss << std::fixed << std::setprecision(precision) << (value ? *value : 0.0f);
                s_sliderInputBuffers[id] = oss.str();
                s_activeSliderInputId = 0;
            }

            ImGui::PopStyleVar(3);
            ImGui::PopStyleColor(5);
        }
        else
        {
            // Display mode - draw the value as centered text
            const std::string& displayValue = s_sliderInputBuffers[id];
            ImVec2 valTextSize = ImGui::CalcTextSize(displayValue.c_str());
            ImVec2 textPos = ImVec2(
                inputBoxMin.x + (valueInputWidth - valTextSize.x) * 0.5f,
                inputBoxMin.y + (valueInputHeight - valTextSize.y) * 0.5f
            );

            // Text color
            ImU32 textColor = Animation::LerpColorU32(
                IM_COL32(180, 200, 210, 255),
                Theme::TextPrimary,
                displayState.hoverAnim
            );

            draw->AddText(textPos, textColor, displayValue.c_str());

            // Start editing when clicked
            if (displayClicked)
            {
                s_activeSliderInputId = id;
                ImGui::SetKeyboardFocusHere();
            }
        }

        // Restore cursor for next element
        ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + sliderHeight + ImGui::GetStyle().ItemSpacing.y));

        return changed;
    }

    bool ModernSliderInt(const char* label, int* value, int min, int max, const char* format, std::optional<Layer> layer)
    {
        int originalValue = value ? *value : 0;

        // Clamp input value first
        if (value)
        {
            if (*value < min) *value = min;
            if (*value > max) *value = max;
        }

        float floatValue = value ? (float)*value : 0.0f;
        bool changed = ModernSliderFloat(label, &floatValue, (float)min, (float)max, "%.0f", layer);

        if (value)
        {
            // Round and clamp
            int newValue = (int)(floatValue + 0.5f);
            if (newValue < min) newValue = min;
            if (newValue > max) newValue = max;
            *value = newValue;

            // Check if actually changed
            changed = (newValue != originalValue);
        }
        return changed;
    }

    // =============================================
    // Combo / Dropdown
    // =============================================

    bool ModernCombo(const char* label, int* current_item, const char* const items[], int items_count, std::optional<uint8_t> popupBgAlpha)
    {
        // Determine alpha to use (nullopt = use Theme default)
        uint8_t alpha = popupBgAlpha.value_or((Theme::BackgroundPanel >> 24) & 0xFF);
        ImU32 popupBgColor = (Theme::BackgroundPanel & 0x00FFFFFF) | ((ImU32)alpha << 24);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::ToVec4(Theme::ButtonDefault));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Theme::ToVec4(Theme::ButtonHover));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Theme::ToVec4(Theme::ButtonHover));
        ImGui::PushStyleColor(ImGuiCol_PopupBg, Theme::ToVec4(popupBgColor));
        ImGui::PushStyleColor(ImGuiCol_Header, Theme::ToVec4(Theme::AccentCyan));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(0, 245, 255, 80));
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::ToVec4(Theme::BorderDefault));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Theme::FrameRounding);
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, Theme::FrameRounding);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

        bool result = ImGui::Combo(label, current_item, items, items_count);

        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(7);

        return result;
    }

    // =============================================
    // Progress / Status
    // =============================================

    void ModernProgressBar(float fraction, ImVec2 size, const char* overlay, std::optional<Layer> layer)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        if (size.x <= 0) size.x = ImGui::GetContentRegionAvail().x;
        if (size.y <= 0) size.y = 8.0f;

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Background
        draw->AddRectFilled(
            pos,
            ImVec2(pos.x + size.x, pos.y + size.y),
            Theme::ButtonDefault,
            size.y * 0.5f
        );

        // Fill
        float fillWidth = size.x * std::clamp(fraction, 0.0f, 1.0f);
        if (fillWidth > 0)
        {
            Draw::RectGradientH(
                pos,
                ImVec2(pos.x + fillWidth, pos.y + size.y),
                Theme::AccentPurple,
                Theme::AccentCyan,
                size.y * 0.5f
            );

            // Glow effect using layer system
            ImVec2 fillSize = ImVec2(fillWidth, size.y);
            Draw::GlowLayers(pos, fillSize, Theme::AccentCyan, 0.5f, Theme::ProgressGlowLayers, Theme::ProgressGlowExpand, size.y * 0.5f, layer);
        }

        // Overlay text
        if (overlay)
        {
            ImVec2 textSize = ImGui::CalcTextSize(overlay);
            draw->AddText(
                ImVec2(pos.x + (size.x - textSize.x) * 0.5f, pos.y + size.y + 4.0f),
                Theme::TextSecondary,
                overlay
            );
        }

        ImGui::Dummy(ImVec2(size.x, size.y + (overlay ? 20.0f : 0.0f)));
    }

    void StatusIndicator(const char* label, ImU32 color, bool pulse, std::optional<float> dotSize)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        float effectiveDotSize = dotSize.value_or(Theme::DefaultDotSize);
        float alpha = pulse ? (Animation::Pulse(2.0f) * 0.5f + 0.5f) : 1.0f;

        auto colorRGB = Theme::ExtractRGB(color);
        ImU32 dotColor = IM_COL32(colorRGB.r, colorRGB.g, colorRGB.b, (int)(alpha * 255));

        draw->AddCircleFilled(
            ImVec2(pos.x + effectiveDotSize * 0.5f, pos.y + ImGui::GetFontSize() * 0.5f),
            effectiveDotSize * 0.5f,
            dotColor
        );

        draw->AddText(
            ImVec2(pos.x + effectiveDotSize + 8.0f, pos.y),
            Theme::TextPrimary,
            label
        );

        ImGui::Dummy(ImVec2(ImGui::CalcTextSize(label).x + effectiveDotSize + 8.0f, ImGui::GetFontSize()));
    }

    // =============================================
    // Cards / Sections
    // =============================================

    bool FeatureCard(const char* icon, const char* name, const char* description, bool* enabled, std::optional<uint8_t> bgAlpha)
    {
        ImGuiID id = ImGui::GetID(name);
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();

        // Calculate layout dimensions
        float iconSize = 32.0f;
        float iconX = pos.x + 12.0f;
        float textX = iconX + iconSize + 12.0f;
        float toggleWidth = Theme::ToggleWidth;
        float togglePadding = 12.0f;
        float cardWidth = ImGui::GetContentRegionAvail().x;

        // Calculate available width for description text
        float availableTextWidth = cardWidth - (textX - pos.x) - toggleWidth - togglePadding;

        // Check if description needs wrapping
        ImVec2 descSize = ImGui::CalcTextSize(description);
        bool needsWrap = descSize.x > availableTextWidth;

        // Calculate card height based on whether text wraps
        float baseHeight = 56.0f;
        float cardHeight = baseHeight;

        if (needsWrap && availableTextWidth > 50.0f)
        {
            // Calculate wrapped text height
            ImVec2 wrappedSize = ImGui::CalcTextSize(description, nullptr, false, availableTextWidth);
            float extraHeight = wrappedSize.y - ImGui::GetFontSize();
            if (extraHeight > 0)
            {
                cardHeight = baseHeight + extraHeight;
            }
        }

        ImVec2 size = ImVec2(cardWidth, cardHeight);

        ImGui::InvisibleButton(name, size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();

        if (clicked && enabled)
        {
            *enabled = !*enabled;
        }

        bool isOn = enabled ? *enabled : false;
        Animation::UpdateWidgetState(state, hovered, false, isOn);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Background with optional alpha override
        ImU32 bgBase = Theme::ButtonDefault;
        ImU32 bgHoverTarget = Theme::ButtonHover;

        // Apply custom alpha if specified (nullopt = use Theme default)
        if (bgAlpha.has_value())
        {
            bgBase = (bgBase & 0x00FFFFFF) | ((ImU32)bgAlpha.value() << 24);
            bgHoverTarget = (bgHoverTarget & 0x00FFFFFF) | ((ImU32)bgAlpha.value() << 24);
        }

        ImU32 bgColor = Animation::LerpColorU32(
            bgBase,
            bgHoverTarget,
            state.hoverAnim
        );
        draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, Theme::FrameRounding);

        // Icon
        float iconY = pos.y + (size.y - iconSize) * 0.5f;
        ImU32 iconColor = isOn ? Theme::AccentCyan : Theme::TextMuted;
        draw->AddText(ImVec2(iconX + 6.0f, iconY + 6.0f), iconColor, icon);

        // Name
        draw->AddText(ImVec2(textX, pos.y + 10.0f), Theme::TextPrimary, name);

        // Description - with automatic wrapping if needed
        if (needsWrap && availableTextWidth > 50.0f)
        {
            const char* textStart = description;
            const char* textEnd = description + strlen(description);
            float lineY = pos.y + 28.0f;
            float lineHeight = ImGui::GetFontSize();

            while (textStart < textEnd)
            {
                const char* lineEnd = ImGui::GetFont()->CalcWordWrapPositionA(
                    1.0f, textStart, textEnd, availableTextWidth);

                if (lineEnd == textStart)
                    lineEnd = textStart + 1;

                draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
                    ImVec2(textX, lineY), Theme::TextMuted, textStart, lineEnd);

                lineY += lineHeight;
                textStart = lineEnd;

                while (textStart < textEnd && *textStart == ' ')
                    textStart++;
            }
        }
        else
        {
            draw->AddText(ImVec2(textX, pos.y + 28.0f), Theme::TextMuted, description);
        }

        // Toggle indicator on right (vertically centered)
        float toggleX = pos.x + size.x - 48.0f;
        float toggleY = pos.y + (size.y - 24.0f) * 0.5f;

        // Simple toggle visual
        state.slideAnim = Animation::Lerp(state.slideAnim, isOn ? 1.0f : 0.0f, 12.0f);

        ImU32 toggleColor = Animation::LerpColorU32(
            Theme::ButtonDefault,
            Theme::AccentCyan,
            state.slideAnim
        );
        draw->AddRectFilled(
            ImVec2(toggleX, toggleY),
            ImVec2(toggleX + 40.0f, toggleY + 24.0f),
            toggleColor,
            12.0f
        );

        float knobX = toggleX + 3.0f + state.slideAnim * 16.0f;
        draw->AddCircleFilled(
            ImVec2(knobX + 9.0f, toggleY + 12.0f),
            9.0f,
            Theme::TextPrimary
        );

        return clicked;
    }

    bool SectionHeader(const char* label, bool* collapsed, std::optional<ImU32> accentColor)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        ImU32 effectiveAccentColor = accentColor.value_or(Theme::TextAccent);

        draw->AddText(
            ImVec2(pos.x, pos.y),
            effectiveAccentColor,
            label
        );

        // Line under header
        float lineY = pos.y + ImGui::GetFontSize() + 4.0f;
        draw->AddLine(
            ImVec2(pos.x, lineY),
            ImVec2(pos.x + ImGui::GetContentRegionAvail().x, lineY),
            Theme::BorderDefault
        );

        ImGui::Dummy(ImVec2(0, ImGui::GetFontSize() + 12.0f));

        return collapsed ? !*collapsed : true;
    }

    // =============================================
    // Tooltip
    // =============================================

    void ModernTooltip(const char* text)
    {
        ImGui::PushStyleColor(ImGuiCol_PopupBg, Theme::ToVec4(Theme::BackgroundPanel));
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::ToVec4(Theme::BorderDefault));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 6));
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, Theme::FrameRounding);

        ImGui::SetTooltip("%s", text);

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(2);
    }

    void HelpMarker(const char* text)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered())
        {
            ModernTooltip(text);
        }
    }

    // =============================================
    // Spacing / Layout
    // =============================================

    void Spacing(float height)
    {
        ImGui::Dummy(ImVec2(0, height));
    }

    void Separator(std::optional<ImU32> color)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float width = ImGui::GetContentRegionAvail().x;

        ImU32 effectiveColor = color.value_or(Theme::BorderDefault);

        ImGui::GetWindowDrawList()->AddLine(
            ImVec2(pos.x, pos.y + 4.0f),
            ImVec2(pos.x + width, pos.y + 4.0f),
            effectiveColor
        );

        ImGui::Dummy(ImVec2(0, 8.0f));
    }

    void SameLine(float offset, float spacing)
    {
        ImGui::SameLine(offset, spacing);
    }
}
