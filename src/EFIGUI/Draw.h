#pragma once
#include "imgui.h"
#include "Layer.h"
#include <vector>
#include <optional>

namespace EFIGUI
{
    // =============================================
    // Draw Constants
    // =============================================
    // Centralized constants for drawing functions.
    // Avoids magic numbers and enables easy tuning.

    namespace DrawConstants
    {
        // RectGlow constants
        constexpr float GlowMinIntensity = 0.01f;       // Threshold below which glow is not drawn
        constexpr int GlowLayerCount = 4;               // Number of glow layers
        constexpr float GlowAlphaMultiplier = 0.2f;     // Base alpha multiplier for glow
        constexpr float GlowLineThickness = 2.0f;       // Line thickness for glow borders

        // NeonBorder constants
        constexpr float NeonGlowIntensity = 0.5f;       // Glow intensity for neon effect
        constexpr float NeonGlowRadius = 4.0f;          // Glow radius for neon effect

        // GlowLayers constants
        constexpr float GlowLayersAlphaMultiplier = 0.1f;   // Alpha multiplier for rect glow layers
        constexpr float GlowLayersCircleAlphaMultiplier = 0.15f; // Alpha multiplier for circle glow layers

        // MarqueeBorder constants
        constexpr int MarqueeNumSegments = 80;          // Total number of path segments
        constexpr int MarqueeCornerSegments = 8;        // Segments per corner arc
        constexpr int MarqueeMinEdgeSegments = 2;       // Minimum segments for straight edges
        constexpr int MarqueeMinAlpha = 25;             // Minimum alpha for border visibility
        constexpr float MarqueeHoverThreshold = 0.1f;   // Threshold for hover animation
        constexpr float Pi = 3.14159265f;               // Pi constant for angle calculations

        // GlassmorphismBg constants
        constexpr int GlassBlurAlpha = 200;             // Alpha for blurred background
        constexpr ImU32 GlassBaseLayer = IM_COL32(20, 20, 35, 200);  // Opaque base to prevent transparency stacking
        constexpr ImU32 GlassOverlayActive = IM_COL32(30, 30, 50, 255);
        constexpr ImU32 GlassOverlayDefault = IM_COL32(20, 20, 35, 250);
        constexpr ImU32 GlassOverlayHover = IM_COL32(30, 30, 55, 252);
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
        bool GlassmorphismBg(
            ImVec2 pos,
            ImVec2 size,
            float rounding = 8.0f,
            float hoverAnim = 0.0f,
            bool isActive = false
        );
    }
}
