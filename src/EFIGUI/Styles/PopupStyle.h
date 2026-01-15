// src/EFIGUI/Styles/PopupStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for Popup/Modal components
struct PopupStyle {
    // Colors
    ImU32 bgColor = CyberpunkTheme::Colors::Surface;
    ImU32 borderColor = CyberpunkTheme::Colors::Border;
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;
    ImU32 overlayColor = IM_COL32(0, 0, 0, 150);  // Modal backdrop

    // Dimensions
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;
    float padding = 12.0f;
    uint8_t bgAlpha = 220;

    // Effects
    bool showGlow = true;
    float glowIntensity = 0.3f;

    // Animation
    float animSpeed = 0.15f;
};

} // namespace EFIGUI
