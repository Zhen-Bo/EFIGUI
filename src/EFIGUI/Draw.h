#pragma once
#include "imgui.h"
#include "Layer.h"
#include <vector>
#include <optional>

namespace EFIGUI
{
    // =============================================
    // Draw Local Constants (non-configurable)
    // =============================================
    // Mathematical constants that don't belong in Theme.
    // All other draw constants are now in DrawTheme (ThemeConfig.h).

    namespace DrawLocal
    {
        constexpr float Pi = 3.14159265f;               // Pi constant for angle calculations
    }

    namespace Draw
    {
        // =============================================
        // Basic Glow Effects
        // =============================================

        // Draw a rectangle with outer glow effect
        void RectGlow(ImVec2 min, ImVec2 max, ImU32 color, float intensity = 0.5f, float radius = 8.0f);

        // Draw horizontal gradient rectangle
        void RectGradientH(ImVec2 min, ImVec2 max, ImU32 left, ImU32 right, float rounding = 0.0f);

        // Draw vertical gradient rectangle
        void RectGradientV(ImVec2 min, ImVec2 max, ImU32 top, ImU32 bottom, float rounding = 0.0f);

        // Draw a neon-style glowing border
        void NeonBorder(ImVec2 min, ImVec2 max, ImU32 color, float thickness = 2.0f, float rounding = 8.0f);

        // Draw centered icon/text within a rect
        void IconCentered(ImVec2 min, ImVec2 max, const char* icon, ImU32 color);

        // =============================================
        // Advanced Glow Effects
        // =============================================

        // Draw multiple expanding glow layers (rectangle)
        // layer: Target layer for deferred drawing (nullopt = use LayerConfig default)
        void GlowLayers(
            ImVec2 pos,
            ImVec2 size,
            ImU32 color,
            float intensity = 0.6f,
            int layerCount = 5,
            float expandBase = 3.0f,
            float rounding = 8.0f,
            std::optional<Layer> layer = std::nullopt
        );

        // Draw multiple expanding glow layers (circle)
        // layer: Target layer for deferred drawing (nullopt = use LayerConfig default)
        void GlowLayersCircle(
            ImVec2 center,
            float baseRadius,
            ImU32 color,
            float intensity = 0.6f,
            int layerCount = 5,
            float expandBase = 3.0f,
            std::optional<Layer> layer = std::nullopt
        );

        // =============================================
        // Animated Effects
        // =============================================

        // Draw an animated marquee border (flowing glow around perimeter)
        // sweepPos: 0.0 to 1.0 position of the bright spot along perimeter
        // sweepLengthFrac: fraction of perimeter that is lit up
        // hoverAnim: 0.0 = static border, 1.0 = full animation
        // layer: Target layer for deferred drawing (nullopt = use LayerConfig default)
        void MarqueeBorder(
            ImVec2 pos,
            ImVec2 size,
            ImU32 color,
            float sweepPos,
            float sweepLengthFrac = 0.25f,
            float rounding = 8.0f,
            float lineThickness = 2.0f,
            float hoverAnim = 1.0f,
            std::optional<Layer> layer = std::nullopt
        );

        // =============================================
        // Glassmorphism Effect
        // =============================================

        // Draw a glassmorphism background using the blur backend
        // Returns true if blur was applied, false if fallback solid color was used
        // Requires a blur backend to be initialized for the blur effect
        // bgAlpha: overlay alpha (omit = use GlassOverlay* defaults, 0-255 = custom alpha)
        bool GlassmorphismBg(
            ImVec2 pos,
            ImVec2 size,
            float rounding = 8.0f,
            float hoverAnim = 0.0f,
            bool isActive = false,
            std::optional<uint8_t> bgAlpha = std::nullopt
        );
    }
}
