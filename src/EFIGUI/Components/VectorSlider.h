// src/EFIGUI/Components/VectorSlider.h
#pragma once
#include "imgui.h"
#include "../Styles/VectorSliderStyle.h"

namespace EFIGUI {

// =============================================
// SliderFloat variants
// =============================================

/// SliderFloat2 - Slider for 2 float values (X, Y)
/// @param label Widget label
/// @param v Array of 2 floats
/// @param v_min Minimum value
/// @param v_max Maximum value
/// @param format Printf format string
/// @param flags ImGuiSliderFlags
/// @return true if any value was changed
bool SliderFloat2(const char* label, float v[2], float v_min, float v_max,
                  const char* format = "%.3f", ImGuiSliderFlags flags = 0);

/// SliderFloat2 with custom style
bool SliderFloat2Ex(const char* label, float v[2], float v_min, float v_max,
                    const char* format, ImGuiSliderFlags flags,
                    const VectorSliderStyle& style);

/// SliderFloat3 - Slider for 3 float values (X, Y, Z)
bool SliderFloat3(const char* label, float v[3], float v_min, float v_max,
                  const char* format = "%.3f", ImGuiSliderFlags flags = 0);

/// SliderFloat3 with custom style
bool SliderFloat3Ex(const char* label, float v[3], float v_min, float v_max,
                    const char* format, ImGuiSliderFlags flags,
                    const VectorSliderStyle& style);

/// SliderFloat4 - Slider for 4 float values (X, Y, Z, W)
bool SliderFloat4(const char* label, float v[4], float v_min, float v_max,
                  const char* format = "%.3f", ImGuiSliderFlags flags = 0);

/// SliderFloat4 with custom style
bool SliderFloat4Ex(const char* label, float v[4], float v_min, float v_max,
                    const char* format, ImGuiSliderFlags flags,
                    const VectorSliderStyle& style);

// =============================================
// SliderInt variants
// =============================================

/// SliderInt2 - Slider for 2 int values (X, Y)
bool SliderInt2(const char* label, int v[2], int v_min, int v_max,
                const char* format = "%d", ImGuiSliderFlags flags = 0);

/// SliderInt2 with custom style
bool SliderInt2Ex(const char* label, int v[2], int v_min, int v_max,
                  const char* format, ImGuiSliderFlags flags,
                  const VectorSliderStyle& style);

/// SliderInt3 - Slider for 3 int values (X, Y, Z)
bool SliderInt3(const char* label, int v[3], int v_min, int v_max,
                const char* format = "%d", ImGuiSliderFlags flags = 0);

/// SliderInt3 with custom style
bool SliderInt3Ex(const char* label, int v[3], int v_min, int v_max,
                  const char* format, ImGuiSliderFlags flags,
                  const VectorSliderStyle& style);

/// SliderInt4 - Slider for 4 int values (X, Y, Z, W)
bool SliderInt4(const char* label, int v[4], int v_min, int v_max,
                const char* format = "%d", ImGuiSliderFlags flags = 0);

/// SliderInt4 with custom style
bool SliderInt4Ex(const char* label, int v[4], int v_min, int v_max,
                  const char* format, ImGuiSliderFlags flags,
                  const VectorSliderStyle& style);

} // namespace EFIGUI
