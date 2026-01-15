// src/EFIGUI/Styles/TreeNodeStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for TreeNode and CollapsingHeader components
struct TreeNodeStyle {
    // Background colors
    ImU32 bgColor = IM_COL32(0, 0, 0, 0);  // Transparent
    ImU32 bgHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 bgActiveColor = IM_COL32(0, 245, 255, 20);

    // Text colors
    ImU32 textColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 textHoverColor = CyberpunkTheme::Colors::AccentCyan;

    // Arrow/Expand icon colors
    ImU32 arrowColor = CyberpunkTheme::Colors::TextSecondary;
    ImU32 arrowHoverColor = CyberpunkTheme::Colors::AccentCyan;

    // Connector line (optional visual for tree hierarchy)
    ImU32 connectorColor = CyberpunkTheme::Colors::Border;
    bool showConnectorLines = false;

    // Glow on expand
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;
    float glowIntensity = 0.3f;

    // Dimensions
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;
    float padding = 4.0f;
    float indentWidth = 20.0f;
    float arrowSize = 12.0f;

    // Animation
    float animSpeed = 0.15f;
};

} // namespace EFIGUI
