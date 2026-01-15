// src/EFIGUI/Components/Table.cpp
#include "Table.h"
#include "../Core/Animation.h"
#include "../Core/Style.h"
#include "imgui_internal.h"

namespace EFIGUI {

// Storage for current table style
static thread_local TableStyle s_currentTableStyle;

bool BeginTableEx(const char* str_id, int columns, ImGuiTableFlags flags,
                  ImVec2 outer_size, float inner_width, const TableStyle& style) {
    s_currentTableStyle = style;

    // Push style colors for table
    ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, style.headerBgColor);
    ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, style.outerBorderColor);
    ImGui::PushStyleColor(ImGuiCol_TableBorderLight, style.innerBorderColor);
    ImGui::PushStyleColor(ImGuiCol_TableRowBg, style.rowBgColor);
    ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, style.rowBgAltColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.cellTextColor);

    // Push style vars
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(style.cellPadding, style.cellPadding));

    bool result = ImGui::BeginTable(str_id, columns, flags, outer_size, inner_width);

    if (!result) {
        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor(6);
    }

    return result;
}

bool BeginTable(const char* str_id, int columns, ImGuiTableFlags flags,
                ImVec2 outer_size, float inner_width) {
    return BeginTableEx(str_id, columns, flags, outer_size, inner_width,
                        StyleSystem::GetCurrentStyle<TableStyle>());
}

void EndTable() {
    ImGui::EndTable();
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(6);
}

void TableSetupColumn(const char* label, ImGuiTableColumnFlags flags,
                      float init_width_or_weight, ImGuiID user_id) {
    ImGui::TableSetupColumn(label, flags, init_width_or_weight, user_id);
}

void TableHeadersRow() {
    // Push header text color
    ImGui::PushStyleColor(ImGuiCol_Text, s_currentTableStyle.headerTextColor);
    ImGui::TableHeadersRow();
    ImGui::PopStyleColor(1);
}

void TableNextRow(ImGuiTableRowFlags flags, float min_row_height) {
    ImGui::TableNextRow(flags, min_row_height);
}

bool TableNextColumn() {
    return ImGui::TableNextColumn();
}

bool TableSetColumnIndex(int column_n) {
    return ImGui::TableSetColumnIndex(column_n);
}

} // namespace EFIGUI
