// src/EFIGUI/Styles/VSliderStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for VSlider (vertical slider) components
struct VSliderStyle {
    // Track colors
    ImU32 trackColor = CyberpunkTheme::Colors::Surface;
    ImU32 trackBorderColor = CyberpunkTheme::Colors::Border;
    ImU32 trackBorderHoverColor = CyberpunkTheme::Colors::AccentCyan;

    // Fill colors (gradient from bottom to current value)
    ImU32 fillColorBottom = CyberpunkTheme::Colors::AccentPurple;
    ImU32 fillColorTop = CyberpunkTheme::Colors::AccentCyan;

    // Knob colors
    ImU32 knobColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 knobActiveColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 knobBorderColor = CyberpunkTheme::Colors::Border;

    // Glow
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;
    ImU32 fillGlowColor = CyberpunkTheme::Colors::AccentCyanGlow;

    // Text
    ImU32 textColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 labelColor = CyberpunkTheme::Colors::TextSecondary;

    // Dimensions
    float trackWidth = 24.0f;
    float trackRounding = 12.0f;
    float knobRadius = 8.0f;
    float knobGlowExpand = 4.0f;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;
    float padding = 4.0f;

    // Animation
    float animSpeed = 0.15f;
    int glowLayers = CyberpunkTheme::Dimensions::GlowLayers;
    float glowIntensity = CyberpunkTheme::Dimensions::GlowIntensity;
};

} // namespace EFIGUI
