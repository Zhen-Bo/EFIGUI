// src/EFIGUI/Components/ListBox.cpp
#include "ListBox.h"
#include "../Core/Animation.h"
#include "../Core/Style.h"
#include "imgui_internal.h"
#include <vector>

namespace EFIGUI {

// Storage for current listbox style during Begin/End scope.
// Note: Using thread_local here instead of StyleSystem::GetComponentState<> is intentional.
// ListBox styles are short-lived (Begin to End), not persistent across frames like Drag state.
// thread_local is simpler and sufficient for this use case.
static thread_local ListBoxStyle s_currentListBoxStyle;

bool BeginListBoxEx(const char* label, ImVec2 size, const ListBoxStyle& style) {
    s_currentListBoxStyle = style;

    // Push style colors
    ImGui::PushStyleColor(ImGuiCol_FrameBg, style.frameBgColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.frameBorderColor);
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, style.scrollbarBgColor);
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, style.scrollbarGrabColor);
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, style.scrollbarGrabHoverColor);
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, style.scrollbarGrabHoverColor);

    // Push style vars
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

    bool result = ImGui::BeginListBox(label, size);

    if (!result) {
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(6);
    }

    return result;
}

bool BeginListBox(const char* label, ImVec2 size) {
    return BeginListBoxEx(label, size, StyleSystem::GetCurrentStyle<ListBoxStyle>());
}

void EndListBox() {
    ImGui::EndListBox();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(6);
}

bool ListBoxEx(const char* label, int* current_item, const char* const items[],
               int items_count, int height_in_items, const ListBoxStyle& style) {
    // Calculate size
    ImVec2 size(0.0f, 0.0f);
    if (height_in_items > 0) {
        size.y = ImGui::GetTextLineHeightWithSpacing() * height_in_items + style.itemPadding * 2;
    }

    bool value_changed = false;

    if (BeginListBoxEx(label, size, style)) {
        for (int i = 0; i < items_count; ++i) {
            bool is_selected = (i == *current_item);

            ImGuiWindow* window = ImGui::GetCurrentWindow();
            const ImGuiID id = window->GetID((void*)(intptr_t)i);
            Animation::WidgetState& state = Animation::GetState(id);

            ImGui::PushID(i);

            // Calculate item rect
            ImVec2 pos = window->DC.CursorPos;
            ImVec2 item_size(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing());
            ImRect bb(pos, ImVec2(pos.x + item_size.x, pos.y + item_size.y));

            ImGui::ItemSize(item_size, 0.0f);
            if (ImGui::ItemAdd(bb, id)) {
                bool hovered, held;
                bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

                Animation::UpdateWidgetState(state, hovered, held, is_selected);

                ImDrawList* draw = window->DrawList;

                // Draw background
                ImU32 bgColor = is_selected ? style.itemBgSelectedColor :
                                hovered ? style.itemBgHoverColor : style.itemBgColor;
                if ((bgColor & IM_COL32_A_MASK) != 0)
                    draw->AddRectFilled(bb.Min, bb.Max, bgColor, style.rounding);

                // Draw accent bar when selected
                if (is_selected) {
                    ImVec2 barMin(bb.Min.x, bb.Min.y + style.itemPadding);
                    ImVec2 barMax(bb.Min.x + style.accentBarWidth, bb.Max.y - style.itemPadding);
                    draw->AddRectFilled(barMin, barMax, style.accentColor, style.accentBarWidth * 0.5f);
                }

                // Draw text
                ImU32 textColor = is_selected ? style.itemTextSelectedColor : style.itemTextColor;
                ImVec2 text_pos(bb.Min.x + style.itemPadding + (is_selected ? style.accentBarWidth + 4.0f : 0.0f),
                                bb.Min.y + style.itemPadding);
                draw->AddText(text_pos, textColor, items[i]);

                // Draw separator if enabled
                if (style.showSeparators && i < items_count - 1) {
                    ImVec2 sepMin(bb.Min.x + style.itemPadding, bb.Max.y);
                    ImVec2 sepMax(bb.Max.x - style.itemPadding, bb.Max.y);
                    draw->AddLine(sepMin, sepMax, style.separatorColor);
                }

                if (pressed) {
                    *current_item = i;
                    value_changed = true;
                }
            }

            ImGui::PopID();
        }

        EndListBox();
    }

    return value_changed;
}

bool ListBox(const char* label, int* current_item, const char* const items[],
             int items_count, int height_in_items) {
    return ListBoxEx(label, current_item, items, items_count, height_in_items,
                     StyleSystem::GetCurrentStyle<ListBoxStyle>());
}

bool ListBox(const char* label, int* current_item,
             const char* (*getter)(void* user_data, int idx),
             void* user_data, int items_count, int height_in_items) {
    // Build items array from getter using std::vector (safe, portable)
    std::vector<const char*> items(items_count);
    for (int i = 0; i < items_count; ++i) {
        items[i] = getter(user_data, i);
    }
    return ListBox(label, current_item, items.data(), items_count, height_in_items);
}

} // namespace EFIGUI
