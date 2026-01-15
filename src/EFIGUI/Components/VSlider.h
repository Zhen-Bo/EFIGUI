// src/EFIGUI/Components/VSlider.h
#pragma once
#include "imgui.h"
#include "../Styles/VSliderStyle.h"

namespace EFIGUI {

/// VSliderFloat - Vertical slider for float values
bool VSliderFloat(const char* label, const ImVec2& size, float* v,
                  float v_min, float v_max, const char* format = "%.3f",
                  ImGuiSliderFlags flags = 0);

/// VSliderFloat with custom style
bool VSliderFloatEx(const char* label, const ImVec2& size, float* v,
                    float v_min, float v_max, const char* format,
                    ImGuiSliderFlags flags, const VSliderStyle& style);

/// VSliderInt - Vertical slider for int values
bool VSliderInt(const char* label, const ImVec2& size, int* v,
                int v_min, int v_max, const char* format = "%d",
                ImGuiSliderFlags flags = 0);

/// VSliderInt with custom style
bool VSliderIntEx(const char* label, const ImVec2& size, int* v,
                  int v_min, int v_max, const char* format,
                  ImGuiSliderFlags flags, const VSliderStyle& style);

} // namespace EFIGUI
