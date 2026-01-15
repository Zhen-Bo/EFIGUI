// src/EFIGUI/Styles/PlotStyle.h
#pragma once
#include "imgui.h"
#include "../Themes/CyberpunkTheme.h"

namespace EFIGUI {

/// Style configuration for Plot components (PlotLines, PlotHistogram)
struct PlotStyle {
    // Frame colors
    ImU32 frameColor = CyberpunkTheme::Colors::Surface;
    ImU32 frameBorderColor = CyberpunkTheme::Colors::Border;
    ImU32 frameBorderHoverColor = CyberpunkTheme::Colors::AccentCyan;

    // Grid (optional)
    ImU32 gridColor = IM_COL32(60, 65, 80, 100);
    float gridLineWidth = 1.0f;
    int gridLinesX = 4;
    int gridLinesY = 3;
    bool showGrid = true;

    // Line plot colors
    ImU32 lineColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 lineGlowColor = CyberpunkTheme::Colors::AccentCyanGlow;
    float lineThickness = 2.0f;

    // Histogram colors
    ImU32 histogramColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 histogramHoverColor = CyberpunkTheme::Colors::AccentPurple;
    float histogramGap = 1.0f;

    // Point markers (optional)
    ImU32 pointColor = CyberpunkTheme::Colors::TextPrimary;
    float pointRadius = 0.0f;
    bool showPoints = false;

    // Highlight
    ImU32 highlightColor = CyberpunkTheme::Colors::AccentPurple;
    ImU32 highlightGlowColor = CyberpunkTheme::Colors::AccentPurpleGlow;

    // Text
    ImU32 overlayTextColor = CyberpunkTheme::Colors::TextPrimary;
    ImU32 labelColor = CyberpunkTheme::Colors::TextSecondary;

    // Dimensions
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;
    float padding = 4.0f;

    // Animation
    float animSpeed = 0.15f;
    int glowLayers = 3;
    float glowIntensity = 0.3f;
};

} // namespace EFIGUI
