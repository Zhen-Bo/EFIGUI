// src/EFIGUI/Components/Radio.h
#pragma once
#include "imgui.h"
#include "../Styles/RadioStyle.h"

namespace EFIGUI {

/// Radio button - matches ImGui::RadioButton API
/// @param label Radio label
/// @param active Whether this radio is selected
/// @return true if clicked
bool RadioButton(const char* label, bool active);

/// Radio button with value selection
/// @param label Radio label
/// @param v Pointer to current value
/// @param v_button Value this button represents
/// @return true if clicked
bool RadioButton(const char* label, int* v, int v_button);

/// Radio button with custom style
bool RadioButtonEx(const char* label, bool active, const RadioStyle& style);

/// Radio button with value selection and custom style
bool RadioButtonEx(const char* label, int* v, int v_button, const RadioStyle& style);

} // namespace EFIGUI
