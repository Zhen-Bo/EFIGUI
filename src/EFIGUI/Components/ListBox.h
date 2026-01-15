// src/EFIGUI/Components/ListBox.h
#pragma once
#include "imgui.h"
#include "../Styles/ListBoxStyle.h"

namespace EFIGUI {

/// Begin a styled list box - matches ImGui::BeginListBox API
bool BeginListBox(const char* label, ImVec2 size = ImVec2(0, 0));

/// Begin a styled list box with custom style
bool BeginListBoxEx(const char* label, ImVec2 size, const ListBoxStyle& style);

/// End a styled list box
void EndListBox();

/// Simple list box with items array - matches ImGui::ListBox API
/// @param current_item pointer to the currently selected index
/// @param items array of string items
/// @param items_count number of items in the array
/// @param height_in_items visible height in item count (-1 for default)
bool ListBox(const char* label, int* current_item, const char* const items[],
             int items_count, int height_in_items = -1);

/// List box with items getter callback
/// @param getter callback function that returns item string by index
/// @param user_data user data passed to getter
bool ListBox(const char* label, int* current_item,
             const char* (*getter)(void* user_data, int idx),
             void* user_data, int items_count, int height_in_items = -1);

/// List box with custom style
bool ListBoxEx(const char* label, int* current_item, const char* const items[],
               int items_count, int height_in_items, const ListBoxStyle& style);

} // namespace EFIGUI
