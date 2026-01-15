// src/EFIGUI/Components/Menu.cpp
#include "Menu.h"
#include "../Core/Animation.h"
#include "../Core/Style.h"
#include "imgui_internal.h"

namespace EFIGUI {

// Storage for current menu style during Begin/End scope.
// Note: Using thread_local here instead of StyleSystem::GetComponentState<> is intentional.
// Menu styles are short-lived (Begin to End), not persistent across frames like Drag state.
// thread_local is simpler and sufficient for this use case.
static thread_local MenuStyle s_currentMenuStyle;

static void PushMenuStyle(const MenuStyle& style) {
    s_currentMenuStyle = style;

    ImGui::PushStyleColor(ImGuiCol_PopupBg, style.popupBgColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.popupBorderColor);
    ImGui::PushStyleColor(ImGuiCol_Header, style.itemBgHoverColor);
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, style.itemBgHoverColor);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, style.itemBgHoverColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.itemTextColor);
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, style.itemTextDisabledColor);

    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.itemPadding, 4.0f));
}

static void PopMenuStyle() {
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(7);
}

bool BeginMainMenuBar() {
    MenuStyle style = StyleSystem::GetCurrentStyle<MenuStyle>();
    s_currentMenuStyle = style;

    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, style.popupBgColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.popupBorderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.itemTextColor);

    bool result = ImGui::BeginMainMenuBar();

    if (!result) {
        ImGui::PopStyleColor(3);
    }

    return result;
}

void EndMainMenuBar() {
    ImGui::EndMainMenuBar();
    ImGui::PopStyleColor(3);
}

bool BeginMenuBar() {
    MenuStyle style = StyleSystem::GetCurrentStyle<MenuStyle>();
    s_currentMenuStyle = style;

    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, style.popupBgColor);
    ImGui::PushStyleColor(ImGuiCol_Border, style.popupBorderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.itemTextColor);

    bool result = ImGui::BeginMenuBar();

    if (!result) {
        ImGui::PopStyleColor(3);
    }

    return result;
}

void EndMenuBar() {
    ImGui::EndMenuBar();
    ImGui::PopStyleColor(3);
}

bool BeginMenuEx(const char* label, bool enabled, const MenuStyle& style) {
    PushMenuStyle(style);

    bool result = ImGui::BeginMenu(label, enabled);

    if (!result) {
        PopMenuStyle();
    }

    return result;
}

bool BeginMenu(const char* label, bool enabled) {
    return BeginMenuEx(label, enabled, StyleSystem::GetCurrentStyle<MenuStyle>());
}

void EndMenu() {
    ImGui::EndMenu();
    PopMenuStyle();
}

bool MenuItemEx(const char* label, const char* shortcut, bool selected, bool enabled,
                const MenuStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiID id = window->GetID(label);
    Animation::WidgetState& state = Animation::GetState(id);

    // Hide ImGui's default checkmark (we draw our own glow dot)
    ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(0, 0, 0, 0));

    // Push hover color
    ImGui::PushStyleColor(ImGuiCol_Text,
        state.hoverAnim > 0.5f ? style.itemTextHoverColor :
        (enabled ? style.itemTextColor : style.itemTextDisabledColor));

    bool pressed = ImGui::MenuItem(label, shortcut, selected, enabled);

    bool hovered = ImGui::IsItemHovered();
    Animation::UpdateWidgetState(state, hovered, false, selected);

    ImGui::PopStyleColor(2);

    // Draw checkmark glow if selected
    if (selected && state.hoverAnim > 0.01f) {
        ImDrawList* draw = window->DrawList;
        ImVec2 item_min = ImGui::GetItemRectMin();

        ImU32 glowColor = Animation::LerpColorU32(IM_COL32(0, 0, 0, 0), style.glowColor,
                                                   state.hoverAnim * style.glowIntensity);
        float checkSize = 12.0f;
        ImVec2 checkCenter(item_min.x + checkSize * 0.5f + 4.0f,
                           (ImGui::GetItemRectMin().y + ImGui::GetItemRectMax().y) * 0.5f);
        draw->AddCircleFilled(checkCenter, checkSize, glowColor);
    }

    return pressed;
}

bool MenuItem(const char* label, const char* shortcut, bool selected, bool enabled) {
    return MenuItemEx(label, shortcut, selected, enabled, s_currentMenuStyle);
}

bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled) {
    if (MenuItemEx(label, shortcut, p_selected ? *p_selected : false, enabled, s_currentMenuStyle)) {
        if (p_selected)
            *p_selected = !*p_selected;
        return true;
    }
    return false;
}

void MenuSeparator() {
    ImGui::PushStyleColor(ImGuiCol_Separator, s_currentMenuStyle.separatorColor);
    ImGui::Separator();
    ImGui::PopStyleColor(1);
}

void MenuHeader(const char* label) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImDrawList* draw = window->DrawList;
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 textSize = ImGui::CalcTextSize(label);
    ImVec2 size(ImGui::GetContentRegionAvail().x, textSize.y + s_currentMenuStyle.itemPadding);

    ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(size);
    if (!ImGui::ItemAdd(bb, 0))
        return;

    // Draw header background
    draw->AddRectFilled(bb.Min, bb.Max, s_currentMenuStyle.headerBgColor, s_currentMenuStyle.rounding);

    // Draw text
    ImVec2 textPos(bb.Min.x + s_currentMenuStyle.itemPadding,
                   bb.Min.y + (size.y - textSize.y) * 0.5f);
    draw->AddText(textPos, s_currentMenuStyle.headerTextColor, label);
}

} // namespace EFIGUI
