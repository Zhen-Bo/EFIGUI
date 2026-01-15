// src/EFIGUI/Components/Selectable.h
#pragma once
#include "imgui.h"
#include "../Styles/SelectableStyle.h"

namespace EFIGUI {

/// Selectable item - matches ImGui::Selectable API
bool Selectable(const char* label, bool selected = false,
                ImGuiSelectableFlags flags = 0, ImVec2 size = ImVec2(0, 0));

/// Selectable with pointer to selection state
bool Selectable(const char* label, bool* p_selected,
                ImGuiSelectableFlags flags = 0, ImVec2 size = ImVec2(0, 0));

/// Selectable with custom style
bool SelectableEx(const char* label, bool selected,
                  ImGuiSelectableFlags flags, ImVec2 size,
                  const SelectableStyle& style);

bool SelectableEx(const char* label, bool* p_selected,
                  ImGuiSelectableFlags flags, ImVec2 size,
                  const SelectableStyle& style);

} // namespace EFIGUI
