#include "Draw.h"
#include "../Theme.h"
#include "Animation.h"
#include "Layer.h"
#include "EFIGUI.h"
#include <cmath>

namespace EFIGUI
{
    namespace Draw
    {
        // =============================================
        // Helper Functions
        // =============================================

        // Calculate path points along a rounded rectangle perimeter
        // Returns points in clockwise order starting from top-left corner
        static void CalculateRoundedRectPath(
            ImVec2 pos,
            ImVec2 size,
            float rounding,
            int numSegments,
            int cornerSegments,
            std::vector<ImVec2>& outPoints)
        {
            using namespace DrawLocal;
            const auto& d = Theme::Draw();

            float straightW = size.x - 2 * rounding;
            float straightH = size.y - 2 * rounding;
            float cornerArc = Pi * rounding * 0.5f;
            float perimeter = 2 * straightW + 2 * straightH + 4 * cornerArc;

            outPoints.clear();
            outPoints.reserve(numSegments + 1);

            // Top edge (left to right)
            int topSegments = (int)(numSegments * straightW / perimeter);
            if (topSegments < d.marqueeMinEdgeSegments) topSegments = d.marqueeMinEdgeSegments;
            for (int i = 0; i <= topSegments; i++)
            {
                float t = (float)i / topSegments;
                outPoints.push_back(ImVec2(pos.x + rounding + t * straightW, pos.y));
            }

            // Top-right corner
            for (int i = 1; i <= cornerSegments; i++)
            {
                float angle = -Pi * 0.5f + (float)i / cornerSegments * Pi * 0.5f;
                outPoints.push_back(ImVec2(
                    pos.x + size.x - rounding + cosf(angle) * rounding,
                    pos.y + rounding + sinf(angle) * rounding));
            }

            // Right edge (top to bottom)
            int rightSegments = (int)(numSegments * straightH / perimeter);
            if (rightSegments < d.marqueeMinEdgeSegments) rightSegments = d.marqueeMinEdgeSegments;
            for (int i = 1; i <= rightSegments; i++)
            {
                float t = (float)i / rightSegments;
                outPoints.push_back(ImVec2(pos.x + size.x, pos.y + rounding + t * straightH));
            }

            // Bottom-right corner
            for (int i = 1; i <= cornerSegments; i++)
            {
                float angle = (float)i / cornerSegments * Pi * 0.5f;
                outPoints.push_back(ImVec2(
                    pos.x + size.x - rounding + cosf(angle) * rounding,
                    pos.y + size.y - rounding + sinf(angle) * rounding));
            }

            // Bottom edge (right to left)
            for (int i = 1; i <= topSegments; i++)
            {
                float t = (float)i / topSegments;
                outPoints.push_back(ImVec2(pos.x + size.x - rounding - t * straightW, pos.y + size.y));
            }

            // Bottom-left corner
            for (int i = 1; i <= cornerSegments; i++)
            {
                float angle = Pi * 0.5f + (float)i / cornerSegments * Pi * 0.5f;
                outPoints.push_back(ImVec2(
                    pos.x + rounding + cosf(angle) * rounding,
                    pos.y + size.y - rounding + sinf(angle) * rounding));
            }

            // Left edge (bottom to top)
            for (int i = 1; i <= rightSegments; i++)
            {
                float t = (float)i / rightSegments;
                outPoints.push_back(ImVec2(pos.x, pos.y + size.y - rounding - t * straightH));
            }

            // Top-left corner
            for (int i = 1; i <= cornerSegments; i++)
            {
                float angle = Pi + (float)i / cornerSegments * Pi * 0.5f;
                outPoints.push_back(ImVec2(
                    pos.x + rounding + cosf(angle) * rounding,
                    pos.y + rounding + sinf(angle) * rounding));
            }
        }

        // Draw marquee segments with alpha falloff based on sweep position
        static void DrawMarqueeSegments(
            Layer targetLayer,
            const std::vector<ImVec2>& pathPoints,
            int r, int g, int b,
            float sweepPos,
            float sweepLengthFrac,
            float lineThickness,
            float hoverAnim)
        {
            const auto& d = Theme::Draw();

            int totalPoints = (int)pathPoints.size();
            for (int i = 0; i < totalPoints; i++)
            {
                float segPos = (float)i / totalPoints;

                // Calculate distance from sweep position (wrapping around)
                float dist1 = fabsf(segPos - sweepPos);
                float dist2 = fabsf(segPos - sweepPos + 1.0f);
                float dist3 = fabsf(segPos - sweepPos - 1.0f);
                float dist = dist1;
                if (dist2 < dist) dist = dist2;
                if (dist3 < dist) dist = dist3;

                // Convert distance to alpha with quadratic falloff
                float normalizedDist = dist / sweepLengthFrac;
                float alpha = 1.0f - normalizedDist;
                if (alpha < 0.0f) alpha = 0.0f;
                alpha = alpha * alpha;

                int finalAlpha = (int)(alpha * 255 * hoverAnim);
                if (finalAlpha < d.marqueeMinAlpha) finalAlpha = (int)(d.marqueeMinAlpha * hoverAnim);

                ImU32 segColor = IM_COL32(r, g, b, finalAlpha);

                int nextIdx = (i + 1) % totalPoints;
                Layers().AddLine(targetLayer, pathPoints[i], pathPoints[nextIdx], segColor, lineThickness);
            }
        }

        // =============================================
        // Basic Glow Effects
        // =============================================

        void RectGlow(ImVec2 min, ImVec2 max, ImU32 color, float intensity, float radius)
        {
            const auto& d = Theme::Draw();

            if (intensity < d.glowMinIntensity) return;

            ImDrawList* draw = ImGui::GetWindowDrawList();

            for (int i = d.glowLayerCount; i >= 1; i--)
            {
                float expand = radius * (float)i / d.glowLayerCount;
                float alpha = intensity * d.glowAlphaMultiplier * (1.0f - (float)i / d.glowLayerCount);

                ImU32 layerColor = IM_COL32(
                    (color >> 0) & 0xFF,
                    (color >> 8) & 0xFF,
                    (color >> 16) & 0xFF,
                    (int)(alpha * 255)
                );

                draw->AddRect(
                    ImVec2(min.x - expand, min.y - expand),
                    ImVec2(max.x + expand, max.y + expand),
                    layerColor,
                    Theme::FrameRounding() + expand,
                    0,
                    d.glowLineThickness
                );
            }
        }

        void RectGradientH(ImVec2 min, ImVec2 max, ImU32 left, ImU32 right, float rounding)
        {
            ImDrawList* draw = ImGui::GetWindowDrawList();

            // ImGui doesn't support gradient with rounding directly,
            // so we use AddRectFilledMultiColor for both cases
            draw->AddRectFilledMultiColor(min, max, left, right, right, left);
        }

        void RectGradientV(ImVec2 min, ImVec2 max, ImU32 top, ImU32 bottom, float rounding)
        {
            ImDrawList* draw = ImGui::GetWindowDrawList();
            draw->AddRectFilledMultiColor(min, max, top, top, bottom, bottom);
        }

        void NeonBorder(ImVec2 min, ImVec2 max, ImU32 color, float thickness, float rounding)
        {
            const auto& d = Theme::Draw();

            ImDrawList* draw = ImGui::GetWindowDrawList();

            // Glow layers
            RectGlow(min, max, color, d.neonGlowIntensity, d.neonGlowRadius);

            // Main border
            draw->AddRect(min, max, color, rounding, 0, thickness);
        }

        void IconCentered(ImVec2 min, ImVec2 max, const char* icon, ImU32 color)
        {
            ImDrawList* draw = ImGui::GetWindowDrawList();
            ImVec2 textSize = ImGui::CalcTextSize(icon);
            ImVec2 pos = ImVec2(
                min.x + (max.x - min.x - textSize.x) * 0.5f,
                min.y + (max.y - min.y - textSize.y) * 0.5f
            );
            draw->AddText(pos, color, icon);
        }

        void GlowLayers(ImVec2 pos, ImVec2 size, ImU32 color, float intensity, int layerCount, float expandBase, float rounding, std::optional<Layer> layer)
        {
            const auto& d = Theme::Draw();

            if (intensity < d.glowMinIntensity) return;

            // Extract RGB from color
            int r = (color >> 0) & 0xFF;
            int g = (color >> 8) & 0xFF;
            int b = (color >> 16) & 0xFF;

            // Determine target layer
            Layer targetLayer = layer.value_or(Layers().GetConfig().defaultWidgetGlow);

            // Draw glow layers from outer to inner using deferred drawing
            for (int i = layerCount; i >= 1; i--)
            {
                float expand = (float)i * expandBase;
                float layerAlpha = intensity * d.glowLayersRectAlpha * (1.0f - (float)i / (layerCount + 1.0f));
                ImU32 layerColor = IM_COL32(r, g, b, (int)(layerAlpha * 255));
                float layerRounding = rounding + expand;

                ImVec2 min(pos.x - expand, pos.y - expand);
                ImVec2 max(pos.x + size.x + expand, pos.y + size.y + expand);

                Layers().AddRectFilled(targetLayer, min, max, layerColor, layerRounding);
            }
        }

        void GlowLayersCircle(ImVec2 center, float baseRadius, ImU32 color, float intensity, int layerCount, float expandBase, std::optional<Layer> layer)
        {
            const auto& d = Theme::Draw();

            if (intensity < d.glowMinIntensity) return;

            // Extract RGB from color
            int r = (color >> 0) & 0xFF;
            int g = (color >> 8) & 0xFF;
            int b = (color >> 16) & 0xFF;

            // Determine target layer
            Layer targetLayer = layer.value_or(Layers().GetConfig().defaultWidgetGlow);

            // Draw glow layers from outer to inner using deferred drawing
            for (int i = layerCount; i >= 1; i--)
            {
                float layerRadius = baseRadius + (float)i * expandBase;
                float layerAlpha = intensity * d.glowLayersCircleAlpha * (1.0f - (float)i / (layerCount + 1.0f));
                ImU32 layerColor = IM_COL32(r, g, b, (int)(layerAlpha * 255));

                Layers().AddCircleFilled(targetLayer, center, layerRadius, layerColor);
            }
        }

        void MarqueeBorder(ImVec2 pos, ImVec2 size, ImU32 color, float sweepPos, float sweepLengthFrac, float rounding, float lineThickness, float hoverAnim, std::optional<Layer> layer)
        {
            const auto& d = Theme::Draw();

            // Determine target layer
            Layer targetLayer = layer.value_or(Layers().GetConfig().defaultMarqueeBorder);

            if (hoverAnim < d.marqueeHoverThreshold)
            {
                // Static border when not hovered
                Layers().AddRect(targetLayer, pos, ImVec2(pos.x + size.x, pos.y + size.y), Theme::BorderDefault(), rounding, 0, 1.0f);
                return;
            }

            // Extract RGB from color
            int r = (color >> 0) & 0xFF;
            int g = (color >> 8) & 0xFF;
            int b = (color >> 16) & 0xFF;

            // Build path points along rounded rect
            std::vector<ImVec2> pathPoints;
            CalculateRoundedRectPath(pos, size, rounding, d.marqueeNumSegments, d.marqueeCornerSegments, pathPoints);

            // Draw marquee segments with alpha falloff
            DrawMarqueeSegments(targetLayer, pathPoints, r, g, b, sweepPos, sweepLengthFrac, lineThickness, hoverAnim);
        }

        bool GlassmorphismBg(ImVec2 pos, ImVec2 size, float rounding, float hoverAnim, bool isActive, std::optional<uint8_t> bgAlpha)
        {
            const auto& d = Theme::Draw();

            ImDrawList* draw = ImGui::GetWindowDrawList();

            // Try to use blurred background for glassmorphism effect
            void* blurredBg = GetBlurResult();
            if (blurredBg)
            {
                // Draw opaque base layer to prevent transparency stacking
                // This ensures consistent alpha 200 appearance regardless of layers below
                draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), Theme::GlassBaseLayer(), rounding);

                // Calculate UV coordinates for button region
                ImGuiIO& io = ImGui::GetIO();
                float screenW = io.DisplaySize.x;
                float screenH = io.DisplaySize.y;

                ImVec2 uv0(pos.x / screenW, pos.y / screenH);
                ImVec2 uv1((pos.x + size.x) / screenW, (pos.y + size.y) / screenH);

                // Draw the blurred game background
                draw->AddImageRounded(
                    (ImTextureID)blurredBg,
                    pos,
                    ImVec2(pos.x + size.x, pos.y + size.y),
                    uv0, uv1,
                    IM_COL32(255, 255, 255, d.glassBlurAlpha),
                    rounding
                );

                // Overlay for color tinting
                ImU32 overlayColor;
                if (bgAlpha.has_value())
                {
                    // Use custom alpha - apply to the appropriate overlay base color
                    uint8_t alpha = bgAlpha.value();
                    ImU32 baseColor = isActive ? Theme::GlassOverlayActive() :
                                      Animation::LerpColorU32(Theme::GlassOverlayDefault(), Theme::GlassOverlayHover(), hoverAnim);
                    overlayColor = Theme::ApplyAlpha(baseColor, alpha);
                }
                else
                {
                    // Use default overlay colors
                    overlayColor = isActive ? Theme::GlassOverlayActive() :
                                   Animation::LerpColorU32(Theme::GlassOverlayDefault(), Theme::GlassOverlayHover(), hoverAnim);
                }
                draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), overlayColor, rounding);
                return true;
            }
            else
            {
                // Fallback: solid background
                ImU32 bgColor = Animation::LerpColorU32(
                    Theme::ButtonDefault(),
                    Theme::ButtonHover(),
                    hoverAnim
                );
                if (isActive)
                {
                    bgColor = Theme::ButtonActive();
                }
                // Apply custom alpha if provided
                if (bgAlpha.has_value())
                {
                    bgColor = Theme::ApplyAlpha(bgColor, bgAlpha.value());
                }
                draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, rounding);
                return false;
            }
        }
    }
}
