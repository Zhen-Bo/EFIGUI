#pragma once
#include "imgui.h"
#include <cstdint>

namespace EFIGUI
{
    // =============================================
    // Theme Configuration Structures
    // =============================================
    // Runtime-configurable theme settings.
    // All values have sensible defaults (Cyberpunk theme).
    // Users can modify individual values or use preset themes.

    // =============================================
    // Color Configuration
    // =============================================

    struct ThemeColors
    {
        // Background colors
        ImU32 backgroundDark      = IM_COL32(10, 10, 15, 255);      // Main window background
        ImU32 backgroundPanel     = IM_COL32(20, 20, 35, 200);      // Semi-transparent panel
        ImU32 backgroundNavbar    = IM_COL32(15, 15, 25, 240);      // Left navbar
        ImU32 backgroundContent   = IM_COL32(25, 25, 45, 180);      // Content area (glass)

        // Accent colors (Neon)
        ImU32 accentPrimary       = IM_COL32(0, 245, 255, 255);     // Cyan
        ImU32 accentSecondary     = IM_COL32(184, 41, 255, 255);    // Purple
        ImU32 accentTertiary      = IM_COL32(255, 41, 117, 255);    // Pink
        ImU32 accentSuccess       = IM_COL32(57, 255, 20, 255);     // Green

        // Accent with alpha (for glow effects) - derived from accent colors
        ImU32 accentPrimaryGlow   = IM_COL32(0, 245, 255, 80);
        ImU32 accentSecondaryGlow = IM_COL32(184, 41, 255, 80);

        // Text colors
        ImU32 textPrimary         = IM_COL32(255, 255, 255, 255);   // White
        ImU32 textSecondary       = IM_COL32(180, 180, 200, 255);   // Light gray
        ImU32 textMuted           = IM_COL32(100, 100, 120, 255);   // Muted
        ImU32 textAccent          = IM_COL32(0, 245, 255, 255);     // Cyan

        // Border colors
        ImU32 borderDefault       = IM_COL32(60, 60, 80, 255);
        ImU32 borderHover         = IM_COL32(0, 245, 255, 180);
        ImU32 borderActive        = IM_COL32(0, 245, 255, 255);

        // Button colors
        ImU32 buttonDefault       = IM_COL32(40, 40, 60, 200);
        ImU32 buttonHover         = IM_COL32(50, 50, 80, 220);
        ImU32 buttonActive        = IM_COL32(0, 180, 200, 255);

        // Title bar gradient
        ImU32 titleBarLeft        = IM_COL32(30, 30, 50, 255);
        ImU32 titleBarRight       = IM_COL32(50, 30, 70, 255);

        // Status colors
        ImU32 statusSuccess       = IM_COL32(57, 255, 20, 255);     // Green
        ImU32 statusWarning       = IM_COL32(255, 200, 0, 255);     // Yellow
        ImU32 statusError         = IM_COL32(255, 60, 60, 255);     // Red
        ImU32 statusInfo          = IM_COL32(0, 200, 255, 255);     // Cyan
    };

    // =============================================
    // Dimension Configuration
    // =============================================

    struct ThemeDimensions
    {
        // Rounding
        float windowRounding      = 12.0f;
        float frameRounding       = 8.0f;
        float buttonRounding      = 8.0f;
        float navItemRounding     = 10.0f;

        // Layout dimensions
        float titleBarHeight      = 36.0f;
        float navbarWidth         = 200.0f;
        float navbarWidthCollapsed = 65.0f;
        float navItemHeight       = 40.0f;
        float navItemPadding      = 8.0f;

        // Spacing
        float contentPadding      = 18.0f;
        float itemSpacing         = 8.0f;

        // Window constraints
        float windowMinWidth      = 700.0f;
        float windowMinHeight     = 550.0f;

        // Toggle dimensions
        float toggleWidth         = 48.0f;
        float toggleHeight        = 24.0f;
        float toggleKnobSize      = 18.0f;
        float toggleLabelGap      = 12.0f;

        // Font sizes
        float fontSizeDefault     = 16.0f;
        float fontSizeSmall       = 14.0f;
        float fontSizeLarge       = 18.0f;
        float fontSizeTitle       = 20.0f;
        float fontSizeIcon        = 18.0f;
    };

    // =============================================
    // Effect Configuration
    // =============================================

    struct ThemeEffects
    {
        // Glow parameters
        float glowIntensity       = 0.6f;
        float glowRadius          = 8.0f;

        // Button glow settings
        int   buttonGlowLayers    = 5;
        float buttonGlowExpand    = 2.0f;

        // Colored button glow (more subtle)
        int   coloredButtonGlowLayers = 4;
        float coloredButtonGlowExpand = 1.5f;

        // Slider glow settings
        int   sliderGlowLayers    = 4;
        float sliderKnobGlowExpand = 2.0f;
        float sliderInputGlowExpand = 1.5f;

        // Progress bar glow
        int   progressGlowLayers  = 4;
        float progressGlowExpand  = 1.5f;

        // Default alpha values
        uint8_t defaultOverlayAlpha     = 200;      // Borderless window overlay
        uint8_t defaultBgAlpha          = 30;       // Icon button hover background
        uint8_t defaultButtonBgAlpha    = 200;      // Colored button background
        uint8_t defaultButtonHoverAlpha = 220;      // Colored button hover
        float   defaultDotSize          = 8.0f;     // Status indicator dot size
        float   defaultGlowMaxAlpha     = 75.0f;    // Glow animation max alpha delta
        float   defaultGlowBaseAlpha    = 180.0f;   // Glow animation base alpha
    };

    // =============================================
    // Animation Configuration
    // =============================================

    struct ThemeAnimation
    {
        float defaultSpeed        = 8.0f;           // Default animation speed
        float pulseFrequency      = 2.0f;           // Pulse effect frequency
        float breatheFrequency    = 1.0f;           // Breathing glow frequency
        float sweepFrequency      = 0.8f;           // Marquee sweep frequency

        // Per-widget animation speeds (multiplier applied to defaultSpeed)
        float toggleSpeedMultiplier = 1.5f;         // Toggle animation (12.0 / 8.0)
        float clickDecaySpeed     = 3.0f;           // Click animation decay

        // Marquee border settings
        float marqueeSpeed        = 0.12f;          // Sweep position increment per frame
        float marqueeSweepLength  = 0.22f;          // Fraction of perimeter that glows
    };

    // =============================================
    // Complete Theme Configuration
    // =============================================

    struct ThemeConfig
    {
        ThemeColors colors;
        ThemeDimensions dimensions;
        ThemeEffects effects;
        ThemeAnimation animation;
    };

    // =============================================
    // Theme Presets (Extensible)
    // =============================================
    // Currently only Cyberpunk is implemented.
    // Designed for future expansion without modifying core architecture.

    enum class ThemePreset
    {
        Cyberpunk       // Default neon cyan/purple theme
        // Future: Dark, Light, Ocean, etc.
    };

    // =============================================
    // Convenience Enums
    // =============================================

    enum class RoundingStyle
    {
        Sharp,          // 0px - No rounding
        Subtle,         // 4px - Subtle rounding
        Rounded,        // 8px - Default rounded
        Pill            // 999px - Pill-shaped
    };

    enum class AnimSpeed
    {
        Instant,        // 0x multiplier (immediate)
        Fast,           // 2x multiplier
        Normal,         // 1x multiplier (default)
        Slow,           // 0.5x multiplier
        Relaxed         // 0.25x multiplier
    };
}
