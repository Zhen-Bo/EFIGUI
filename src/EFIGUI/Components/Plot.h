// src/EFIGUI/Components/Plot.h
#pragma once
#include "imgui.h"
#include "../Styles/PlotStyle.h"
#include <cfloat>

namespace EFIGUI {

/// PlotLines - Display a line graph
void PlotLines(const char* label, const float* values, int values_count,
               int values_offset = 0, const char* overlay_text = nullptr,
               float scale_min = FLT_MAX, float scale_max = FLT_MAX,
               ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));

/// PlotLines with getter function
void PlotLines(const char* label,
               float (*values_getter)(void* data, int idx), void* data,
               int values_count, int values_offset = 0,
               const char* overlay_text = nullptr,
               float scale_min = FLT_MAX, float scale_max = FLT_MAX,
               ImVec2 graph_size = ImVec2(0, 0));

/// PlotLines with custom style
void PlotLinesEx(const char* label, const float* values, int values_count,
                 int values_offset, const char* overlay_text,
                 float scale_min, float scale_max, ImVec2 graph_size,
                 int stride, const PlotStyle& style);

/// PlotHistogram - Display a histogram/bar graph
void PlotHistogram(const char* label, const float* values, int values_count,
                   int values_offset = 0, const char* overlay_text = nullptr,
                   float scale_min = FLT_MAX, float scale_max = FLT_MAX,
                   ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));

/// PlotHistogram with getter function
void PlotHistogram(const char* label,
                   float (*values_getter)(void* data, int idx), void* data,
                   int values_count, int values_offset = 0,
                   const char* overlay_text = nullptr,
                   float scale_min = FLT_MAX, float scale_max = FLT_MAX,
                   ImVec2 graph_size = ImVec2(0, 0));

/// PlotHistogram with custom style
void PlotHistogramEx(const char* label, const float* values, int values_count,
                     int values_offset, const char* overlay_text,
                     float scale_min, float scale_max, ImVec2 graph_size,
                     int stride, const PlotStyle& style);

} // namespace EFIGUI
