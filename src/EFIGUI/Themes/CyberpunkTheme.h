// src/EFIGUI/Themes/CyberpunkTheme.h
#pragma once
#include "imgui.h"
#include <cstdint>

namespace EFIGUI {

/// Cyberpunk Glassmorphism Theme - The default EFIGUI theme
struct CyberpunkTheme {
    /// Color palette
    struct Colors {
        // Accent colors
        static constexpr ImU32 AccentCyan = IM_COL32(0, 245, 255, 255);
        static constexpr ImU32 AccentPurple = IM_COL32(180, 0, 255, 255);
        static constexpr ImU32 AccentCyanGlow = IM_COL32(0, 245, 255, 100);
        static constexpr ImU32 AccentPurpleGlow = IM_COL32(180, 0, 255, 100);

        // Status colors
        static constexpr ImU32 Success = IM_COL32(0, 255, 136, 255);
        static constexpr ImU32 Warning = IM_COL32(255, 200, 0, 255);
        static constexpr ImU32 Error = IM_COL32(255, 80, 80, 255);
        static constexpr ImU32 Info = IM_COL32(0, 200, 255, 255);

        // Surface colors
        static constexpr ImU32 Background = IM_COL32(15, 15, 25, 255);
        static constexpr ImU32 Surface = IM_COL32(25, 28, 38, 200);
        static constexpr ImU32 SurfaceHover = IM_COL32(35, 40, 55, 220);
        static constexpr ImU32 SurfaceActive = IM_COL32(45, 50, 70, 240);

        // Text colors
        static constexpr ImU32 TextPrimary = IM_COL32(255, 255, 255, 255);
        static constexpr ImU32 TextSecondary = IM_COL32(180, 180, 190, 255);
        static constexpr ImU32 TextMuted = IM_COL32(120, 120, 140, 255);

        // Border colors
        static constexpr ImU32 Border = IM_COL32(60, 65, 80, 255);
        static constexpr ImU32 BorderHover = IM_COL32(80, 90, 110, 255);
    };

    /// Dimension constants
    struct Dimensions {
        static constexpr float Rounding = 6.0f;
        static constexpr float BorderWidth = 1.0f;
        static constexpr float Spacing = 8.0f;
        static constexpr float GlowIntensity = 0.5f;
        static constexpr int GlowLayers = 5;
    };
};

} // namespace EFIGUI
