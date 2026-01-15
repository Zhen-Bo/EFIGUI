// src/EFIGUI/Components/Drag.h
#pragma once
#include "imgui.h"
#include "../Styles/DragStyle.h"

namespace EFIGUI {

// =============================================
// DragFloat variants
// =============================================

/// DragFloat - Drag to edit a float value
/// @param label Widget label
/// @param v Pointer to float value
/// @param v_speed Drag speed multiplier
/// @param v_min Minimum value
/// @param v_max Maximum value
/// @param format Printf format string
/// @param flags ImGuiSliderFlags
/// @return true if value was changed
bool DragFloat(const char* label, float* v, float v_speed = 1.0f,
               float v_min = 0.0f, float v_max = 0.0f,
               const char* format = "%.3f", ImGuiSliderFlags flags = 0);

/// DragFloat with custom style
bool DragFloatEx(const char* label, float* v, float v_speed,
                 float v_min, float v_max, const char* format,
                 ImGuiSliderFlags flags, const DragStyle& style);

/// DragFloat2 - Drag to edit 2 float values
bool DragFloat2(const char* label, float v[2], float v_speed = 1.0f,
                float v_min = 0.0f, float v_max = 0.0f,
                const char* format = "%.3f", ImGuiSliderFlags flags = 0);

/// DragFloat3 - Drag to edit 3 float values
bool DragFloat3(const char* label, float v[3], float v_speed = 1.0f,
                float v_min = 0.0f, float v_max = 0.0f,
                const char* format = "%.3f", ImGuiSliderFlags flags = 0);

/// DragFloat4 - Drag to edit 4 float values
bool DragFloat4(const char* label, float v[4], float v_speed = 1.0f,
                float v_min = 0.0f, float v_max = 0.0f,
                const char* format = "%.3f", ImGuiSliderFlags flags = 0);

/// DragFloatRange2 - Drag to edit a range (min/max)
bool DragFloatRange2(const char* label, float* v_current_min, float* v_current_max,
                     float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f,
                     const char* format = "%.3f", const char* format_max = nullptr,
                     ImGuiSliderFlags flags = 0);

// =============================================
// DragInt variants
// =============================================

/// DragInt - Drag to edit an int value
bool DragInt(const char* label, int* v, float v_speed = 1.0f,
             int v_min = 0, int v_max = 0,
             const char* format = "%d", ImGuiSliderFlags flags = 0);

/// DragInt with custom style
bool DragIntEx(const char* label, int* v, float v_speed,
               int v_min, int v_max, const char* format,
               ImGuiSliderFlags flags, const DragStyle& style);

/// DragInt2 - Drag to edit 2 int values
bool DragInt2(const char* label, int v[2], float v_speed = 1.0f,
              int v_min = 0, int v_max = 0,
              const char* format = "%d", ImGuiSliderFlags flags = 0);

/// DragInt3 - Drag to edit 3 int values
bool DragInt3(const char* label, int v[3], float v_speed = 1.0f,
              int v_min = 0, int v_max = 0,
              const char* format = "%d", ImGuiSliderFlags flags = 0);

/// DragInt4 - Drag to edit 4 int values
bool DragInt4(const char* label, int v[4], float v_speed = 1.0f,
              int v_min = 0, int v_max = 0,
              const char* format = "%d", ImGuiSliderFlags flags = 0);

/// DragIntRange2 - Drag to edit a range (min/max)
bool DragIntRange2(const char* label, int* v_current_min, int* v_current_max,
                   float v_speed = 1.0f, int v_min = 0, int v_max = 0,
                   const char* format = "%d", const char* format_max = nullptr,
                   ImGuiSliderFlags flags = 0);

// =============================================
// State Management
// =============================================

/// Prune stale drag value states that haven't been used recently.
/// @param maxIdleSeconds States unused for this duration will be removed (default: 2.0s)
void PruneDragValueStates(float maxIdleSeconds = 2.0f);

} // namespace EFIGUI
