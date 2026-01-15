// src/EFIGUI/Components/Plot.cpp
#include "Plot.h"
#include "../Core/Animation.h"
#include "../Core/Draw.h"
#include "../Core/Layer.h"
#include "../Core/Style.h"
#include "imgui_internal.h"
#include <algorithm>
#include <cmath>
#include <cfloat>
#include <vector>
#include <cstdio>

namespace EFIGUI {

// Draw grid lines
static void DrawGrid(ImDrawList* draw, const ImRect& bb, const PlotStyle& style) {
    if (!style.showGrid)
        return;

    float width = bb.GetWidth();
    float height = bb.GetHeight();

    // Vertical grid lines
    for (int i = 1; i < style.gridLinesX; ++i) {
        float x = bb.Min.x + (width * i) / style.gridLinesX;
        draw->AddLine(ImVec2(x, bb.Min.y), ImVec2(x, bb.Max.y),
                      style.gridColor, style.gridLineWidth);
    }

    // Horizontal grid lines
    for (int i = 1; i < style.gridLinesY; ++i) {
        float y = bb.Min.y + (height * i) / style.gridLinesY;
        draw->AddLine(ImVec2(bb.Min.x, y), ImVec2(bb.Max.x, y),
                      style.gridColor, style.gridLineWidth);
    }
}

// Calculate auto scale
static void CalculateAutoScale(const float* values, int count, int offset, int stride,
                                float& scale_min, float& scale_max) {
    if (count <= 0)
        return;

    float min_val = FLT_MAX;
    float max_val = -FLT_MAX;

    for (int i = 0; i < count; ++i) {
        int idx = (offset + i) % count;
        const float* p = reinterpret_cast<const float*>(
            reinterpret_cast<const char*>(values) + idx * stride);
        float v = *p;
        if (v < min_val) min_val = v;
        if (v > max_val) max_val = v;
    }

    if (scale_min == FLT_MAX)
        scale_min = min_val;
    if (scale_max == FLT_MAX)
        scale_max = max_val;

    if (scale_max == scale_min) {
        scale_max = scale_min + 1.0f;
    }
}

// PlotLines Implementation
void PlotLinesEx(const char* label, const float* values, int values_count,
                 int values_offset, const char* overlay_text,
                 float scale_min, float scale_max, ImVec2 graph_size,
                 int stride, const PlotStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    const ImGuiID id = window->GetID(label);

    if (graph_size.x == 0.0f)
        graph_size.x = ImGui::GetContentRegionAvail().x;
    if (graph_size.y == 0.0f)
        graph_size.y = 80.0f;

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect frame_bb(pos, ImVec2(pos.x + graph_size.x, pos.y + graph_size.y));

    bool labelHidden = (label[0] == '#' && label[1] == '#');
    ImVec2 labelSize = labelHidden ? ImVec2(0, 0) : ImGui::CalcTextSize(label);
    const ImRect total_bb(pos, ImVec2(frame_bb.Max.x,
        frame_bb.Max.y + (labelSize.y > 0 ? ImGui::GetStyle().ItemSpacing.y + labelSize.y : 0)));

    ImGui::ItemSize(total_bb, 0.0f);
    if (!ImGui::ItemAdd(total_bb, id))
        return;

    Animation::WidgetState& animState = Animation::GetState(id);
    bool hovered = ImGui::IsItemHovered();
    Animation::UpdateWidgetState(animState, hovered, false, false);

    CalculateAutoScale(values, values_count, values_offset, stride, scale_min, scale_max);

    ImDrawList* draw = window->DrawList;

    // Draw frame background
    draw->AddRectFilled(frame_bb.Min, frame_bb.Max, style.frameColor, style.rounding);

    // Draw border
    ImU32 borderColor = hovered ? style.frameBorderHoverColor : style.frameBorderColor;
    draw->AddRect(frame_bb.Min, frame_bb.Max, borderColor, style.rounding, 0, style.borderWidth);

    // Draw grid
    ImRect inner_bb(frame_bb.Min.x + style.padding, frame_bb.Min.y + style.padding,
                    frame_bb.Max.x - style.padding, frame_bb.Max.y - style.padding);
    DrawGrid(draw, inner_bb, style);

    // Draw line plot
    if (values_count > 1) {
        float width = inner_bb.GetWidth();
        float height = inner_bb.GetHeight();
        float inv_scale = (scale_max == scale_min) ? 0.0f : 1.0f / (scale_max - scale_min);

        // Draw glow first (if hovered)
        if (animState.hoverAnim > 0.1f) {
            for (int i = 0; i < values_count - 1; ++i) {
                int idx0 = (values_offset + i) % values_count;
                int idx1 = (values_offset + i + 1) % values_count;

                const float* p0 = reinterpret_cast<const float*>(
                    reinterpret_cast<const char*>(values) + idx0 * stride);
                const float* p1 = reinterpret_cast<const float*>(
                    reinterpret_cast<const char*>(values) + idx1 * stride);

                float t0 = (float)i / (values_count - 1);
                float t1 = (float)(i + 1) / (values_count - 1);

                float y0 = 1.0f - ((*p0 - scale_min) * inv_scale);
                float y1 = 1.0f - ((*p1 - scale_min) * inv_scale);

                ImVec2 point0(inner_bb.Min.x + t0 * width, inner_bb.Min.y + y0 * height);
                ImVec2 point1(inner_bb.Min.x + t1 * width, inner_bb.Min.y + y1 * height);

                ImU32 glowColor = (style.lineGlowColor & 0x00FFFFFF) |
                                  (static_cast<ImU32>(animState.hoverAnim * 100) << 24);
                draw->AddLine(point0, point1, glowColor, style.lineThickness + 4.0f);
            }
        }

        // Draw main line
        for (int i = 0; i < values_count - 1; ++i) {
            int idx0 = (values_offset + i) % values_count;
            int idx1 = (values_offset + i + 1) % values_count;

            const float* p0 = reinterpret_cast<const float*>(
                reinterpret_cast<const char*>(values) + idx0 * stride);
            const float* p1 = reinterpret_cast<const float*>(
                reinterpret_cast<const char*>(values) + idx1 * stride);

            float t0 = (float)i / (values_count - 1);
            float t1 = (float)(i + 1) / (values_count - 1);

            float y0 = 1.0f - ((*p0 - scale_min) * inv_scale);
            float y1 = 1.0f - ((*p1 - scale_min) * inv_scale);

            ImVec2 point0(inner_bb.Min.x + t0 * width, inner_bb.Min.y + y0 * height);
            ImVec2 point1(inner_bb.Min.x + t1 * width, inner_bb.Min.y + y1 * height);

            draw->AddLine(point0, point1, style.lineColor, style.lineThickness);
        }

        // Draw points if enabled
        if (style.showPoints && style.pointRadius > 0) {
            for (int i = 0; i < values_count; ++i) {
                int idx = (values_offset + i) % values_count;
                const float* p = reinterpret_cast<const float*>(
                    reinterpret_cast<const char*>(values) + idx * stride);

                float t = (float)i / (values_count - 1);
                float y = 1.0f - ((*p - scale_min) * inv_scale);

                ImVec2 point(inner_bb.Min.x + t * width, inner_bb.Min.y + y * height);
                draw->AddCircleFilled(point, style.pointRadius, style.pointColor);
            }
        }
    }

    // Draw overlay text
    if (overlay_text) {
        ImVec2 textSize = ImGui::CalcTextSize(overlay_text);
        float textX = frame_bb.Min.x + (graph_size.x - textSize.x) * 0.5f;
        float textY = frame_bb.Min.y + style.padding;
        draw->AddText(ImVec2(textX, textY), style.overlayTextColor, overlay_text);
    }

    // Draw label
    if (!labelHidden) {
        float textX = frame_bb.Min.x;
        float textY = frame_bb.Max.y + ImGui::GetStyle().ItemSpacing.y;
        draw->AddText(ImVec2(textX, textY), style.labelColor, label);
    }
}

void PlotLines(const char* label, const float* values, int values_count,
               int values_offset, const char* overlay_text,
               float scale_min, float scale_max, ImVec2 graph_size, int stride) {
    PlotLinesEx(label, values, values_count, values_offset, overlay_text,
                scale_min, scale_max, graph_size, stride,
                StyleSystem::GetCurrentStyle<PlotStyle>());
}

void PlotLines(const char* label,
               float (*values_getter)(void* data, int idx), void* data,
               int values_count, int values_offset,
               const char* overlay_text,
               float scale_min, float scale_max, ImVec2 graph_size) {
    // Stack buffer for small data (256 floats = 1KB, safe for stack)
    constexpr int STACK_THRESHOLD = 256;
    float stackBuffer[STACK_THRESHOLD];

    // Heap buffer for large data
    std::vector<float> heapBuffer;

    // Choose buffer based on size
    float* values;
    if (values_count <= STACK_THRESHOLD) {
        values = stackBuffer;
    } else {
        heapBuffer.resize(values_count);
        values = heapBuffer.data();
    }

    // Fill buffer from getter
    for (int i = 0; i < values_count; ++i) {
        values[i] = values_getter(data, i);
    }

    PlotLines(label, values, values_count, values_offset, overlay_text,
              scale_min, scale_max, graph_size, sizeof(float));
}

// PlotHistogram Implementation
void PlotHistogramEx(const char* label, const float* values, int values_count,
                     int values_offset, const char* overlay_text,
                     float scale_min, float scale_max, ImVec2 graph_size,
                     int stride, const PlotStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    const ImGuiID id = window->GetID(label);

    if (graph_size.x == 0.0f)
        graph_size.x = ImGui::GetContentRegionAvail().x;
    if (graph_size.y == 0.0f)
        graph_size.y = 80.0f;

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect frame_bb(pos, ImVec2(pos.x + graph_size.x, pos.y + graph_size.y));

    bool labelHidden = (label[0] == '#' && label[1] == '#');
    ImVec2 labelSize = labelHidden ? ImVec2(0, 0) : ImGui::CalcTextSize(label);
    const ImRect total_bb(pos, ImVec2(frame_bb.Max.x,
        frame_bb.Max.y + (labelSize.y > 0 ? ImGui::GetStyle().ItemSpacing.y + labelSize.y : 0)));

    ImGui::ItemSize(total_bb, 0.0f);
    if (!ImGui::ItemAdd(total_bb, id))
        return;

    Animation::WidgetState& animState = Animation::GetState(id);
    bool hovered = ImGui::IsItemHovered();
    Animation::UpdateWidgetState(animState, hovered, false, false);

    CalculateAutoScale(values, values_count, values_offset, stride, scale_min, scale_max);

    ImDrawList* draw = window->DrawList;

    // Draw frame background
    draw->AddRectFilled(frame_bb.Min, frame_bb.Max, style.frameColor, style.rounding);

    // Draw border
    ImU32 borderColor = hovered ? style.frameBorderHoverColor : style.frameBorderColor;
    draw->AddRect(frame_bb.Min, frame_bb.Max, borderColor, style.rounding, 0, style.borderWidth);

    // Draw grid
    ImRect inner_bb(frame_bb.Min.x + style.padding, frame_bb.Min.y + style.padding,
                    frame_bb.Max.x - style.padding, frame_bb.Max.y - style.padding);
    DrawGrid(draw, inner_bb, style);

    // Draw histogram bars
    if (values_count > 0) {
        float width = inner_bb.GetWidth();
        float height = inner_bb.GetHeight();
        float inv_scale = (scale_max == scale_min) ? 0.0f : 1.0f / (scale_max - scale_min);
        float bar_width = (width - style.histogramGap * (values_count - 1)) / values_count;

        // Find hovered bar
        int hoveredBar = -1;
        if (hovered) {
            float mouseX = ImGui::GetMousePos().x;
            float relX = mouseX - inner_bb.Min.x;
            hoveredBar = static_cast<int>(relX / (bar_width + style.histogramGap));
            if (hoveredBar < 0 || hoveredBar >= values_count)
                hoveredBar = -1;
        }

        for (int i = 0; i < values_count; ++i) {
            int idx = (values_offset + i) % values_count;
            const float* p = reinterpret_cast<const float*>(
                reinterpret_cast<const char*>(values) + idx * stride);
            float v = *p;

            float normalized = (v - scale_min) * inv_scale;
            normalized = std::clamp(normalized, 0.0f, 1.0f);

            float bar_height = normalized * height;
            float bar_x = inner_bb.Min.x + i * (bar_width + style.histogramGap);
            float bar_y = inner_bb.Max.y - bar_height;

            ImVec2 bar_min(bar_x, bar_y);
            ImVec2 bar_max(bar_x + bar_width, inner_bb.Max.y);

            ImU32 barColor = (i == hoveredBar) ? style.histogramHoverColor : style.histogramColor;

            draw->AddRectFilled(bar_min, bar_max, barColor, 2.0f);

            // Draw glow on hovered bar
            if (i == hoveredBar) {
                Draw::RectGlow(bar_min, bar_max, style.highlightGlowColor,
                               style.glowIntensity, 3.0f);

                char buf[64];
                snprintf(buf, sizeof(buf), "%.2f", v);
                ImGui::SetTooltip("%s", buf);
            }
        }
    }

    // Draw overlay text
    if (overlay_text) {
        ImVec2 textSize = ImGui::CalcTextSize(overlay_text);
        float textX = frame_bb.Min.x + (graph_size.x - textSize.x) * 0.5f;
        float textY = frame_bb.Min.y + style.padding;
        draw->AddText(ImVec2(textX, textY), style.overlayTextColor, overlay_text);
    }

    // Draw label
    if (!labelHidden) {
        float textX = frame_bb.Min.x;
        float textY = frame_bb.Max.y + ImGui::GetStyle().ItemSpacing.y;
        draw->AddText(ImVec2(textX, textY), style.labelColor, label);
    }
}

void PlotHistogram(const char* label, const float* values, int values_count,
                   int values_offset, const char* overlay_text,
                   float scale_min, float scale_max, ImVec2 graph_size, int stride) {
    PlotHistogramEx(label, values, values_count, values_offset, overlay_text,
                    scale_min, scale_max, graph_size, stride,
                    StyleSystem::GetCurrentStyle<PlotStyle>());
}

void PlotHistogram(const char* label,
                   float (*values_getter)(void* data, int idx), void* data,
                   int values_count, int values_offset,
                   const char* overlay_text,
                   float scale_min, float scale_max, ImVec2 graph_size) {
    // Stack buffer for small data (256 floats = 1KB, safe for stack)
    constexpr int STACK_THRESHOLD = 256;
    float stackBuffer[STACK_THRESHOLD];

    // Heap buffer for large data
    std::vector<float> heapBuffer;

    // Choose buffer based on size
    float* values;
    if (values_count <= STACK_THRESHOLD) {
        values = stackBuffer;
    } else {
        heapBuffer.resize(values_count);
        values = heapBuffer.data();
    }

    // Fill buffer from getter
    for (int i = 0; i < values_count; ++i) {
        values[i] = values_getter(data, i);
    }

    PlotHistogram(label, values, values_count, values_offset, overlay_text,
                  scale_min, scale_max, graph_size, sizeof(float));
}

} // namespace EFIGUI
