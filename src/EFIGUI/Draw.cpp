#include "Draw.h"
#include "Theme.h"
#include "Animation.h"
#include "EFIGUI.h"
#include <cmath>

namespace EFIGUI
{
    namespace Draw
    {
        void RectGlow(ImVec2 min, ImVec2 max, ImU32 color, float intensity, float radius)
        {
            if (intensity < 0.01f) return;

            ImDrawList* draw = ImGui::GetWindowDrawList();

            // Multiple layers for glow effect
            int layers = 4;
            for (int i = layers; i >= 1; i--)
            {
                float expand = radius * (float)i / layers;
                float alpha = intensity * 0.2f * (1.0f - (float)i / layers);

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
                    Theme::FrameRounding + expand,
                    0,
                    2.0f
                );
            }
        }

        void RectGradientH(ImVec2 min, ImVec2 max, ImU32 left, ImU32 right, float rounding)
        {
            ImDrawList* draw = ImGui::GetWindowDrawList();

            if (rounding > 0)
            {
                // ImGui doesn't support gradient with rounding directly,
                // so we approximate with AddRectFilledMultiColor
                draw->AddRectFilledMultiColor(min, max, left, right, right, left);
            }
            else
            {
                draw->AddRectFilledMultiColor(min, max, left, right, right, left);
            }
        }

        void RectGradientV(ImVec2 min, ImVec2 max, ImU32 top, ImU32 bottom, float rounding)
        {
            ImDrawList* draw = ImGui::GetWindowDrawList();
            draw->AddRectFilledMultiColor(min, max, top, top, bottom, bottom);
        }

        void NeonBorder(ImVec2 min, ImVec2 max, ImU32 color, float thickness, float rounding)
        {
            ImDrawList* draw = ImGui::GetWindowDrawList();

            // Glow layers
            RectGlow(min, max, color, 0.5f, 4.0f);

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

        void GlowLayers(ImVec2 pos, ImVec2 size, ImU32 color, float intensity, int layerCount, float expandBase, float rounding, bool useForeground)
        {
            if (intensity < 0.01f) return;

            // Extract RGB from color
            int r = (color >> 0) & 0xFF;
            int g = (color >> 8) & 0xFF;
            int b = (color >> 16) & 0xFF;

            ImDrawList* draw = useForeground ? ImGui::GetForegroundDrawList() : ImGui::GetWindowDrawList();

            // Draw glow layers from outer to inner
            for (int i = layerCount; i >= 1; i--)
            {
                float expand = (float)i * expandBase;
                float layerAlpha = intensity * 0.1f * (1.0f - (float)i / (layerCount + 1.0f));
                draw->AddRectFilled(
                    ImVec2(pos.x - expand, pos.y - expand),
                    ImVec2(pos.x + size.x + expand, pos.y + size.y + expand),
                    IM_COL32(r, g, b, (int)(layerAlpha * 255)),
                    rounding + expand
                );
            }
        }

        void GlowLayersCircle(ImVec2 center, float baseRadius, ImU32 color, float intensity, int layerCount, float expandBase, bool useForeground)
        {
            if (intensity < 0.01f) return;

            // Extract RGB from color
            int r = (color >> 0) & 0xFF;
            int g = (color >> 8) & 0xFF;
            int b = (color >> 16) & 0xFF;

            ImDrawList* draw = useForeground ? ImGui::GetForegroundDrawList() : ImGui::GetWindowDrawList();

            // Draw glow layers from outer to inner
            for (int i = layerCount; i >= 1; i--)
            {
                float layerRadius = baseRadius + (float)i * expandBase;
                float layerAlpha = intensity * 0.15f * (1.0f - (float)i / (layerCount + 1.0f));
                draw->AddCircleFilled(center, layerRadius, IM_COL32(r, g, b, (int)(layerAlpha * 255)));
            }
        }

        void MarqueeBorder(ImVec2 pos, ImVec2 size, ImU32 color, float sweepPos, float sweepLengthFrac, float rounding, float lineThickness, float hoverAnim, bool useForeground)
        {
            if (hoverAnim < 0.1f)
            {
                // Static border when not hovered
                ImDrawList* draw = useForeground ? ImGui::GetForegroundDrawList() : ImGui::GetWindowDrawList();
                draw->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), Theme::BorderDefault, rounding, 0, 1.0f);
                return;
            }

            // Extract RGB from color
            int r = (color >> 0) & 0xFF;
            int g = (color >> 8) & 0xFF;
            int b = (color >> 16) & 0xFF;

            // Build path points along rounded rect
            int numSegments = 80;
            int cornerSegments = 8;
            float straightW = size.x - 2 * rounding;
            float straightH = size.y - 2 * rounding;
            float cornerArc = 3.14159f * rounding * 0.5f;
            float perimeter = 2 * straightW + 2 * straightH + 4 * cornerArc;
            float PI = 3.14159265f;

            std::vector<ImVec2> pathPoints;
            pathPoints.reserve(numSegments + 1);

            // Top edge (left to right)
            int topSegments = (int)(numSegments * straightW / perimeter);
            if (topSegments < 2) topSegments = 2;
            for (int i = 0; i <= topSegments; i++) {
                float t = (float)i / topSegments;
                pathPoints.push_back(ImVec2(pos.x + rounding + t * straightW, pos.y));
            }

            // Top-right corner
            for (int i = 1; i <= cornerSegments; i++) {
                float angle = -PI * 0.5f + (float)i / cornerSegments * PI * 0.5f;
                pathPoints.push_back(ImVec2(pos.x + size.x - rounding + cosf(angle) * rounding, pos.y + rounding + sinf(angle) * rounding));
            }

            // Right edge (top to bottom)
            int rightSegments = (int)(numSegments * straightH / perimeter);
            if (rightSegments < 2) rightSegments = 2;
            for (int i = 1; i <= rightSegments; i++) {
                float t = (float)i / rightSegments;
                pathPoints.push_back(ImVec2(pos.x + size.x, pos.y + rounding + t * straightH));
            }

            // Bottom-right corner
            for (int i = 1; i <= cornerSegments; i++) {
                float angle = 0 + (float)i / cornerSegments * PI * 0.5f;
                pathPoints.push_back(ImVec2(pos.x + size.x - rounding + cosf(angle) * rounding, pos.y + size.y - rounding + sinf(angle) * rounding));
            }

            // Bottom edge (right to left)
            for (int i = 1; i <= topSegments; i++) {
                float t = (float)i / topSegments;
                pathPoints.push_back(ImVec2(pos.x + size.x - rounding - t * straightW, pos.y + size.y));
            }

            // Bottom-left corner
            for (int i = 1; i <= cornerSegments; i++) {
                float angle = PI * 0.5f + (float)i / cornerSegments * PI * 0.5f;
                pathPoints.push_back(ImVec2(pos.x + rounding + cosf(angle) * rounding, pos.y + size.y - rounding + sinf(angle) * rounding));
            }

            // Left edge (bottom to top)
            for (int i = 1; i <= rightSegments; i++) {
                float t = (float)i / rightSegments;
                pathPoints.push_back(ImVec2(pos.x, pos.y + size.y - rounding - t * straightH));
            }

            // Top-left corner
            for (int i = 1; i <= cornerSegments; i++) {
                float angle = PI + (float)i / cornerSegments * PI * 0.5f;
                pathPoints.push_back(ImVec2(pos.x + rounding + cosf(angle) * rounding, pos.y + rounding + sinf(angle) * rounding));
            }

            // Draw line segments with varying alpha
            ImDrawList* borderDraw = useForeground ? ImGui::GetForegroundDrawList() : ImGui::GetWindowDrawList();
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

                // Convert distance to alpha
                float normalizedDist = dist / sweepLengthFrac;
                float alpha = 1.0f - normalizedDist;
                if (alpha < 0.0f) alpha = 0.0f;
                alpha = alpha * alpha;  // Quadratic falloff

                int finalAlpha = (int)(alpha * 255 * hoverAnim);
                if (finalAlpha < 25) finalAlpha = (int)(25 * hoverAnim);

                ImU32 segColor = IM_COL32(r, g, b, finalAlpha);

                int nextIdx = (i + 1) % totalPoints;
                borderDraw->AddLine(pathPoints[i], pathPoints[nextIdx], segColor, lineThickness);
            }
        }

        bool GlassmorphismBg(ImVec2 pos, ImVec2 size, float rounding, float hoverAnim, bool isActive)
        {
            ImDrawList* draw = ImGui::GetWindowDrawList();

            // Try to use blurred background for glassmorphism effect
            void* blurredBg = GetBlurResult();
            if (blurredBg)
            {
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
                    IM_COL32(255, 255, 255, 200),  // Slightly transparent
                    rounding
                );

                // Semi-transparent dark overlay for better contrast
                ImU32 overlayColor = isActive ? IM_COL32(30, 30, 50, 180) :
                                     Animation::LerpColorU32(
                                         IM_COL32(20, 20, 35, 160),
                                         IM_COL32(30, 30, 55, 180),
                                         hoverAnim
                                     );
                draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), overlayColor, rounding);
                return true;
            }
            else
            {
                // Fallback: solid background
                ImU32 bgColor = Animation::LerpColorU32(
                    Theme::ButtonDefault,
                    Theme::ButtonHover,
                    hoverAnim
                );
                if (isActive)
                {
                    bgColor = Theme::ButtonActive;
                }
                draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, rounding);
                return false;
            }
        }
    }
}
