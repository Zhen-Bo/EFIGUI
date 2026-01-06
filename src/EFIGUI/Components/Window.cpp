// =============================================
// EFIGUI Components - Window
// Custom Title Bar, Borderless Window, Navbar Header
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Helper Functions
    // =============================================

    // Draw title bar gradient background
    static void DrawTitleBarBackground(
        ImDrawList* draw,
        ImVec2 windowPos,
        ImVec2 windowSize,
        float titleHeight)
    {
        ImVec2 titleMin = windowPos;
        ImVec2 titleMax = ImVec2(windowPos.x + windowSize.x, windowPos.y + titleHeight);

        // Gradient background
        Draw::RectGradientH(titleMin, titleMax, Theme::TitleBarLeft(), Theme::TitleBarRight(), Theme::WindowRounding());

        // Only round top corners - fill the rest
        draw->AddRectFilled(
            ImVec2(titleMin.x, titleMin.y + Theme::WindowRounding()),
            ImVec2(titleMax.x, titleMax.y),
            Theme::TitleBarLeft()
        );

        // Neon accent line at bottom of title
        draw->AddLine(
            ImVec2(titleMin.x, titleMax.y),
            ImVec2(titleMax.x, titleMax.y),
            Theme::AccentCyan(),
            1.0f
        );
    }

    // Draw window control buttons (close, minimize) and handle their interaction
    // Returns the X position of the leftmost button (for drag area calculation)
    static float DrawWindowControls(
        ImDrawList* draw,
        ImVec2 windowPos,
        ImVec2 windowSize,
        float titleHeight,
        bool* p_open)
    {
        // Resolve values from WindowTheme
        const auto& t = Theme::Window();
        const float titleButtonSize = t.titleButtonSize;
        const float titleButtonMargin = t.titleButtonMargin;
        const float titleButtonSpacing = t.titleButtonSpacing;

        float btnY = windowPos.y + (titleHeight - titleButtonSize) * 0.5f;
        float closeX = windowPos.x + windowSize.x - titleButtonSize - titleButtonMargin;

        // Close button
        ImGui::SetCursorScreenPos(ImVec2(closeX, btnY));
        ImGui::PushID("##close");

        bool closeHovered = ImGui::IsMouseHoveringRect(
            ImVec2(closeX, btnY),
            ImVec2(closeX + titleButtonSize, btnY + titleButtonSize)
        );

        ImU32 closeColor = closeHovered ? Theme::StatusError() : Theme::TextSecondary();
        if (closeHovered && ImGui::IsMouseClicked(0))
        {
            if (p_open) *p_open = false;
        }

        Draw::IconCentered(
            ImVec2(closeX, btnY),
            ImVec2(closeX + titleButtonSize, btnY + titleButtonSize),
            Icons::Close,
            closeColor
        );

        ImGui::PopID();

        // Minimize button
        float minX = closeX - titleButtonSize - titleButtonSpacing;
        ImGui::PushID("##minimize");

        bool minHovered = ImGui::IsMouseHoveringRect(
            ImVec2(minX, btnY),
            ImVec2(minX + titleButtonSize, btnY + titleButtonSize)
        );

        ImU32 minColor = minHovered ? Theme::AccentCyan() : Theme::TextSecondary();
        Draw::IconCentered(
            ImVec2(minX, btnY),
            ImVec2(minX + titleButtonSize, btnY + titleButtonSize),
            Icons::Minimize,
            minColor
        );

        ImGui::PopID();

        return minX;
    }

    // Handle title bar dragging logic
    static void HandleTitleBarDrag(
        ImVec2 windowPos,
        ImVec2 titleMin,
        ImVec2 titleMax)
    {
        bool inTitleBar = ImGui::IsMouseHoveringRect(titleMin, titleMax);

        bool& isDragging = GetIsDraggingTitleBar();
        ImVec2& dragOffset = GetDragOffset();

        if (inTitleBar && ImGui::IsMouseClicked(0))
        {
            isDragging = true;
            dragOffset = ImVec2(
                ImGui::GetMousePos().x - windowPos.x,
                ImGui::GetMousePos().y - windowPos.y
            );
        }

        if (isDragging)
        {
            if (ImGui::IsMouseDown(0))
            {
                ImVec2 newPos = ImVec2(
                    ImGui::GetMousePos().x - dragOffset.x,
                    ImGui::GetMousePos().y - dragOffset.y
                );
                ImGui::SetWindowPos(newPos);
            }
            else
            {
                isDragging = false;
            }
        }
    }

    // =============================================
    // Custom Title Bar
    // =============================================

    bool BeginCustomWindow(const char* name, bool* p_open, ImGuiWindowFlags flags)
    {
        // Resolve values from WindowTheme
        const auto& t = Theme::Window();
        const float titleTextPadding = t.titleTextPadding;
        const float titleButtonSpacing = t.titleButtonSpacing;

        // Add flags for no title bar (we draw our own)
        flags |= ImGuiWindowFlags_NoTitleBar;
        flags |= ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowSizeConstraints(ImVec2(Theme::WindowMinWidth(), Theme::WindowMinHeight()), ImVec2(FLT_MAX, FLT_MAX));

        if (!ImGui::Begin(name, p_open, flags))
        {
            ImGui::End();
            return false;
        }

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        float titleHeight = Theme::TitleBarHeight();

        // Draw title bar background
        DrawTitleBarBackground(draw, windowPos, windowSize, titleHeight);

        // Title text
        ImVec2 textPos = ImVec2(windowPos.x + titleTextPadding, windowPos.y + (titleHeight - ImGui::GetFontSize()) * 0.5f);
        draw->AddText(textPos, Theme::TextPrimary(), name);

        // Draw window controls and get leftmost button position
        float leftmostBtnX = DrawWindowControls(draw, windowPos, windowSize, titleHeight, p_open);

        // Handle title bar dragging (drag area ends before buttons)
        ImVec2 titleMin = windowPos;
        ImVec2 titleMax = ImVec2(leftmostBtnX - titleButtonSpacing, windowPos.y + titleHeight);
        HandleTitleBarDrag(windowPos, titleMin, titleMax);

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
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, Theme::WindowRounding());
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
        uint8_t effectiveOverlayAlpha = overlayAlpha.value_or(Theme::DefaultOverlayAlpha());

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
                Theme::WindowRounding()
            );

            // Dark overlay for better contrast and cyberpunk look
            draw->AddRectFilled(
                windowPos,
                ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
                IM_COL32(10, 10, 20, effectiveOverlayAlpha),
                Theme::WindowRounding()
            );
        }
        else
        {
            // Fallback: solid dark background
            draw->AddRectFilled(
                windowPos,
                ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
                Theme::BackgroundDark(),
                Theme::WindowRounding()
            );
        }

        // Subtle border
        draw->AddRect(
            windowPos,
            ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
            Theme::BorderDefault(),
            Theme::WindowRounding(),
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

    bool NavbarHeader(const char* title, const NavbarHeaderConfig& config)
    {
        return NavbarHeader(title, config.iconExpanded, config.iconCollapsed,
                           config.collapsed, config.width, config.closeClicked, config.glowColor);
    }

    bool NavbarHeader(const char* title, const char* iconExpanded, const char* iconCollapsed,
                      bool collapsed, float width, bool* closeClicked, std::optional<ImU32> glowColor)
    {
        // Resolve values from WindowTheme
        const auto& t = Theme::Window();
        const float glowRadiusBase = t.glowRadiusBase;
        const float glowRadiusExpand = t.glowRadiusExpand;
        const int glowAnimAlpha = t.glowAnimAlpha;
        const float contentSpacingY = t.contentSpacingY;
        const float navbarIconPadding = t.navbarIconPadding;
        const float navbarButtonSize = t.navbarButtonSize;
        const float navbarButtonPadding = t.navbarButtonPadding;

        ImGuiID id = ImGui::GetID("##NavbarHeader");
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float headerHeight = Theme::TitleBarHeight();
        ImVec2 size = ImVec2(width - Theme::NavItemPadding() * 2, headerHeight);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Get effective glow color
        ImU32 effectiveGlowColor = glowColor.value_or(Theme::AccentCyan());
        auto glowRGB = Theme::ExtractRGB(effectiveGlowColor);

        // Header background (gradient)
        Draw::RectGradientH(pos, ImVec2(pos.x + size.x, pos.y + size.y),
                           Theme::TitleBarLeft(), Theme::TitleBarRight(), Theme::NavItemRounding());

        // Bottom accent line
        draw->AddLine(
            ImVec2(pos.x, pos.y + size.y),
            ImVec2(pos.x + size.x, pos.y + size.y),
            effectiveGlowColor,
            1.0f
        );

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
                int glowAlpha = (int)(Theme::DefaultGlowBaseAlpha() + glowAnim * Theme::DefaultGlowMaxAlpha());
                iconColor = IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, glowAlpha);
            }

            // Draw glow behind icon
            if (!headerHovered)
            {
                float glowRadius = glowRadiusBase + glowAnim * glowRadiusExpand;
                ImU32 glowColorWithAlpha = IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, (int)(glowAnim * glowAnimAlpha));
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
            ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + headerHeight + contentSpacingY));

            return headerClicked;
        }
        else
        {
            // Expanded: show icon + title + << button
            float iconX = pos.x + navbarIconPadding;

            // Brand icon
            draw->AddText(ImVec2(iconX, contentY), effectiveGlowColor, iconExpanded);

            // Title text
            float textX = iconX + ImGui::GetFontSize() + navbarIconPadding;
            draw->AddText(ImVec2(textX, contentY), Theme::TextPrimary(), title);

            // Collapse button on far right
            float collapseX = pos.x + size.x - navbarButtonSize - navbarButtonPadding;
            float btnY = pos.y + (headerHeight - navbarButtonSize) * 0.5f;

            ImGui::SetCursorScreenPos(ImVec2(collapseX, btnY));
            ImGui::InvisibleButton("##collapse", ImVec2(navbarButtonSize, navbarButtonSize));
            bool collapseHovered = ImGui::IsItemHovered();
            bool collapseClicked = ImGui::IsItemClicked();

            ImU32 collapseColor = collapseHovered ? effectiveGlowColor : Theme::TextSecondary();
            Draw::IconCentered(
                ImVec2(collapseX, btnY),
                ImVec2(collapseX + navbarButtonSize, btnY + navbarButtonSize),
                Icons::Collapse,
                collapseColor
            );

            // Advance cursor
            ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + headerHeight + contentSpacingY));

            return collapseClicked;
        }
    }
}
