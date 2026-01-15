// src/EFIGUI/Styles/ColorEditStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for ColorEdit/ColorPicker components
struct ColorEditStyle {
    // Preview swatch
    ImU32 swatchBorderColor = CyberpunkTheme::Colors::Border;
    ImU32 swatchBorderHoverColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 swatchGlowColor = CyberpunkTheme::Colors::AccentCyanGlow;
    float swatchRounding = CyberpunkTheme::Dimensions::Rounding;
    float swatchBorderWidth = CyberpunkTheme::Dimensions::BorderWidth;

    // Picker popup
    ImU32 popupBackground = CyberpunkTheme::Colors::Surface;
    ImU32 popupBorderColor = CyberpunkTheme::Colors::AccentCyan;
    float popupRounding = CyberpunkTheme::Dimensions::Rounding;
    float popupPadding = 8.0f;

    // Alpha channel
    ImU32 alphaCheckerLight = IM_COL32(80, 80, 90, 255);
    ImU32 alphaCheckerDark = IM_COL32(40, 40, 50, 255);
    float alphaCheckerSize = 6.0f;

    // Hue bar
    ImU32 hueBorderColor = CyberpunkTheme::Colors::Border;
    float hueBorderWidth = 1.0f;

    // Text
    ImU32 textColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 labelColor = CyberpunkTheme::Colors::TextSecondary;

    // Dimensions
    float padding = 4.0f;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;

    // Animation
    float animSpeed = 0.15f;
    int glowLayers = CyberpunkTheme::Dimensions::GlowLayers;
    float glowIntensity = CyberpunkTheme::Dimensions::GlowIntensity;
};

} // namespace EFIGUI
