// src/EFIGUI/Components/Checkbox.h
#pragma once
#include "imgui.h"
#include "../Styles/CheckboxStyle.h"

namespace EFIGUI {

/// Traditional checkbox - matches ImGui::Checkbox API
/// @param label Checkbox label
/// @param v Pointer to bool value
/// @return true if value was changed
bool Checkbox(const char* label, bool* v);

/// Checkbox with custom style
/// @param label Checkbox label
/// @param v Pointer to bool value
/// @param style Custom style using C++20 designated initializers
/// @return true if value was changed
bool CheckboxEx(const char* label, bool* v, const CheckboxStyle& style);

} // namespace EFIGUI
