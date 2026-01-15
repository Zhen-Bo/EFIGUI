// src/EFIGUI/Components/ColorEdit.h
#pragma once
#include "imgui.h"
#include "../Styles/ColorEditStyle.h"

namespace EFIGUI {

/// ColorEdit3 - Edit RGB color (3 floats)
/// @param label Widget label
/// @param col Array of 3 floats [0-1]
/// @param flags ImGuiColorEditFlags
/// @return true if color was changed
bool ColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);

/// ColorEdit3 with custom style
bool ColorEdit3Ex(const char* label, float col[3], ImGuiColorEditFlags flags, const ColorEditStyle& style);

/// ColorEdit4 - Edit RGBA color (4 floats)
/// @param label Widget label
/// @param col Array of 4 floats [0-1]
/// @param flags ImGuiColorEditFlags
/// @return true if color was changed
bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);

/// ColorEdit4 with custom style
bool ColorEdit4Ex(const char* label, float col[4], ImGuiColorEditFlags flags, const ColorEditStyle& style);

/// ColorPicker3 - Full color picker (3 floats)
/// @param label Widget label
/// @param col Array of 3 floats [0-1]
/// @param flags ImGuiColorEditFlags
/// @return true if color was changed
bool ColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);

/// ColorPicker3 with custom style
bool ColorPicker3Ex(const char* label, float col[3], ImGuiColorEditFlags flags, const ColorEditStyle& style);

/// ColorPicker4 - Full color picker (4 floats)
/// @param label Widget label
/// @param col Array of 4 floats [0-1]
/// @param flags ImGuiColorEditFlags
/// @param ref_col Optional reference color for comparison
/// @return true if color was changed
bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = nullptr);

/// ColorPicker4 with custom style
bool ColorPicker4Ex(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col, const ColorEditStyle& style);

/// ColorButton - Display a colored square/button
/// @param desc_id Widget ID
/// @param col Color to display
/// @param flags ImGuiColorEditFlags
/// @param size Button size (0,0 = default)
/// @return true if clicked
bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

/// ColorButton with custom style
bool ColorButtonEx(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, const ImVec2& size, const ColorEditStyle& style);

} // namespace EFIGUI
