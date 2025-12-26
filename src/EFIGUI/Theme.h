#pragma once
#include "imgui.h"

namespace EFIGUI
{
    // =============================================
    // Configurable Icons
    // =============================================
    // Users can override these default icons before calling EFIGUI functions
    // Example: EFIGUI::Icons::Close = "\xef\x80\x8d"; // Font Awesome xmark

    namespace Icons
    {
        // Window control icons
        inline const char* Close       = "X";           // Window close button
        inline const char* Minimize    = "-";           // Window minimize button
        inline const char* Collapse    = "<<";          // Navbar collapse button
        inline const char* Expand      = ">>";          // Navbar expand button

        // Navigation icons
        inline const char* ChevronLeft  = "<";
        inline const char* ChevronRight = ">";
        inline const char* Bars         = "=";          // Menu/hamburger icon
        inline const char* Home         = "H";          // Home icon
        inline const char* Cog          = "*";          // Settings/gear icon
        inline const char* User         = "U";          // User/profile icon

        // Common UI icons
        inline const char* Check       = "v";           // Checkmark
        inline const char* Warning     = "!";           // Warning/exclamation
        inline const char* Info        = "i";           // Information
        inline const char* Error       = "x";           // Error/cross
        inline const char* Eye         = "o";           // Eye/visibility
        inline const char* Shield      = "S";           // Shield/protection
        inline const char* Bolt        = "B";           // Lightning bolt/performance
    }

    namespace Theme
    {
        // =============================================
        // Color Palette - Cyberpunk + Glassmorphism
        // =============================================

        // Background colors
        constexpr ImU32 BackgroundDark      = IM_COL32(10, 10, 15, 255);      // #0a0a0f
        constexpr ImU32 BackgroundPanel     = IM_COL32(20, 20, 35, 200);      // Semi-transparent panel
        constexpr ImU32 BackgroundNavbar    = IM_COL32(15, 15, 25, 240);      // Left navbar
        constexpr ImU32 BackgroundContent   = IM_COL32(25, 25, 45, 180);      // Content area (glass)

        // Accent colors (Neon)
        constexpr ImU32 AccentCyan          = IM_COL32(0, 245, 255, 255);     // #00f5ff
        constexpr ImU32 AccentPurple        = IM_COL32(184, 41, 255, 255);    // #b829ff
        constexpr ImU32 AccentPink          = IM_COL32(255, 41, 117, 255);    // #ff2975
        constexpr ImU32 AccentGreen         = IM_COL32(57, 255, 20, 255);     // #39ff14

        // Accent with alpha (for glow effects)
        constexpr ImU32 AccentCyanGlow      = IM_COL32(0, 245, 255, 80);
        constexpr ImU32 AccentPurpleGlow    = IM_COL32(184, 41, 255, 80);

        // Text colors
        constexpr ImU32 TextPrimary         = IM_COL32(255, 255, 255, 255);   // White
        constexpr ImU32 TextSecondary       = IM_COL32(180, 180, 200, 255);   // Light gray
        constexpr ImU32 TextMuted           = IM_COL32(100, 100, 120, 255);   // Muted
        constexpr ImU32 TextAccent          = IM_COL32(0, 245, 255, 255);     // Cyan

        // Border colors
        constexpr ImU32 BorderDefault       = IM_COL32(60, 60, 80, 255);
        constexpr ImU32 BorderHover         = IM_COL32(0, 245, 255, 180);
        constexpr ImU32 BorderActive        = IM_COL32(0, 245, 255, 255);

        // Button colors
        constexpr ImU32 ButtonDefault       = IM_COL32(40, 40, 60, 200);
        constexpr ImU32 ButtonHover         = IM_COL32(50, 50, 80, 220);
        constexpr ImU32 ButtonActive        = IM_COL32(0, 180, 200, 255);

        // Title bar gradient
        constexpr ImU32 TitleBarLeft        = IM_COL32(30, 30, 50, 255);
        constexpr ImU32 TitleBarRight       = IM_COL32(50, 30, 70, 255);

        // Status colors
        constexpr ImU32 StatusSuccess       = IM_COL32(57, 255, 20, 255);     // Green
        constexpr ImU32 StatusWarning       = IM_COL32(255, 200, 0, 255);     // Yellow
        constexpr ImU32 StatusError         = IM_COL32(255, 60, 60, 255);     // Red
        constexpr ImU32 StatusInfo          = IM_COL32(0, 200, 255, 255);     // Cyan

        // =============================================
        // ImVec4 versions (for ImGui style colors)
        // =============================================

        inline ImVec4 ToVec4(ImU32 color) {
            return ImGui::ColorConvertU32ToFloat4(color);
        }

        // =============================================
        // Dimensions
        // =============================================

        constexpr float WindowRounding      = 12.0f;
        constexpr float FrameRounding       = 8.0f;
        constexpr float ButtonRounding      = 8.0f;
        constexpr float NavItemRounding     = 10.0f;

        constexpr float TitleBarHeight      = 36.0f;
        constexpr float NavbarWidth         = 200.0f;
        constexpr float NavbarWidthCollapsed = 65.0f;
        constexpr float NavItemHeight       = 40.0f;
        constexpr float NavItemPadding      = 8.0f;

        constexpr float ContentPadding      = 18.0f;
        constexpr float ItemSpacing         = 8.0f;

        constexpr float GlowIntensity       = 0.6f;
        constexpr float GlowRadius          = 8.0f;

        // Window constraints
        constexpr float WindowMinWidth      = 700.0f;
        constexpr float WindowMinHeight     = 550.0f;

        // Toggle dimensions
        constexpr float ToggleWidth         = 48.0f;
        constexpr float ToggleHeight        = 24.0f;
        constexpr float ToggleKnobSize      = 18.0f;
        constexpr float ToggleLabelGap      = 12.0f;

        // =============================================
        // Font sizes
        // =============================================

        constexpr float FontSizeDefault     = 16.0f;
        constexpr float FontSizeSmall       = 14.0f;
        constexpr float FontSizeLarge       = 18.0f;
        constexpr float FontSizeTitle       = 20.0f;
        constexpr float FontSizeIcon        = 18.0f;

        // =============================================
        // Animation
        // =============================================

        constexpr float AnimationSpeed      = 8.0f;
        constexpr float PulseFrequency      = 2.0f;

        // =============================================
        // Default Values (for optional parameters)
        // =============================================

        constexpr uint8_t DefaultOverlayAlpha       = 200;      // Borderless window overlay
        constexpr uint8_t DefaultBgAlpha            = 30;       // Icon button hover background
        constexpr uint8_t DefaultButtonBgAlpha      = 200;      // Colored button background
        constexpr uint8_t DefaultButtonHoverAlpha   = 220;      // Colored button hover background
        constexpr float   DefaultDotSize            = 8.0f;     // Status indicator dot size
        constexpr float   DefaultGlowMaxAlpha       = 75.0f;    // Glow animation max alpha delta
        constexpr float   DefaultGlowBaseAlpha      = 180.0f;   // Glow animation base alpha

        // =============================================
        // Color Utilities
        // =============================================

        // RGB component structure for color manipulation
        struct ColorRGB
        {
            uint8_t r, g, b;
        };

        // Extract RGB components from ImU32 color (ignores alpha)
        inline ColorRGB ExtractRGB(ImU32 color)
        {
            return {
                static_cast<uint8_t>((color >> 0) & 0xFF),
                static_cast<uint8_t>((color >> 8) & 0xFF),
                static_cast<uint8_t>((color >> 16) & 0xFF)
            };
        }

        // Create ImU32 color from RGB components and alpha
        inline ImU32 MakeColor(const ColorRGB& rgb, uint8_t alpha)
        {
            return IM_COL32(rgb.r, rgb.g, rgb.b, alpha);
        }

        // Apply new alpha to existing color (preserves RGB)
        inline ImU32 ApplyAlpha(ImU32 color, uint8_t alpha)
        {
            return (color & 0x00FFFFFF) | (static_cast<ImU32>(alpha) << 24);
        }

        // Extract alpha from ImU32 color
        inline uint8_t ExtractAlpha(ImU32 color)
        {
            return static_cast<uint8_t>((color >> 24) & 0xFF);
        }

        // =============================================
        // Theme initialization
        // =============================================

        // Apply EFIGUI theme to ImGui
        void Apply();

        // Apply custom colors (override defaults)
        void SetAccentColor(ImU32 primary, ImU32 secondary = 0);
    }
}
