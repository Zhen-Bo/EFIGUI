// src/EFIGUI/Components/Table.h
#pragma once
#include "imgui.h"
#include "../Styles/TableStyle.h"

namespace EFIGUI {

/// Begin a styled table - matches ImGui::BeginTable API
bool BeginTable(const char* str_id, int columns, ImGuiTableFlags flags = 0,
                ImVec2 outer_size = ImVec2(0.0f, 0.0f), float inner_width = 0.0f);

/// Begin a styled table with custom style
bool BeginTableEx(const char* str_id, int columns, ImGuiTableFlags flags,
                  ImVec2 outer_size, float inner_width, const TableStyle& style);

/// End a styled table
void EndTable();

/// Setup a table column - wrapper for ImGui::TableSetupColumn
void TableSetupColumn(const char* label, ImGuiTableColumnFlags flags = 0,
                      float init_width_or_weight = 0.0f, ImGuiID user_id = 0);

/// Render table headers row with styling
void TableHeadersRow();

/// Move to next table row
void TableNextRow(ImGuiTableRowFlags flags = 0, float min_row_height = 0.0f);

/// Move to next column, returns false if not visible
bool TableNextColumn();

/// Move to specific column, returns false if not visible
bool TableSetColumnIndex(int column_n);

} // namespace EFIGUI
