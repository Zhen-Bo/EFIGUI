// src/EFIGUI/Components/Popup.cpp
#include "Popup.h"
#include "../Core/Draw.h"
#include "../Core/Style.h"
#include "imgui_internal.h"

namespace EFIGUI {

void OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags) {
    ImGui::OpenPopup(str_id, popup_flags);
}

bool BeginPopupEx(const char* str_id, ImGuiWindowFlags flags, const PopupStyle& style) {
    // Apply themed style
    ImGui::PushStyleColor(ImGuiCol_PopupBg, (style.bgColor & 0x00FFFFFF) | ((ImU32)style.bgAlpha << 24));
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, style.borderWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.padding, style.padding));

    bool open = ImGui::BeginPopup(str_id, flags);

    if (open && style.showGlow) {
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();
        Draw::RectGlow(pos, ImVec2(pos.x + size.x, pos.y + size.y),
                       style.glowColor, style.glowIntensity, 6.0f);
    }

    if (!open) {
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(2);
    }

    return open;
}

bool BeginPopup(const char* str_id, ImGuiWindowFlags flags) {
    return BeginPopupEx(str_id, flags, StyleSystem::GetCurrentStyle<PopupStyle>());
}

// IMPORTANT: Always use EFIGUI::EndPopup() with EFIGUI::BeginPopup().
// Mixing with ImGui::EndPopup() will cause style stack imbalance.
void EndPopup() {
    ImGui::EndPopup();
    // Pop in reverse order (LIFO): 3 StyleVars, then 2 StyleColors
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
}

bool BeginPopupModalEx(const char* name, bool* p_open, ImGuiWindowFlags flags, const PopupStyle& style) {
    // Apply themed style
    ImGui::PushStyleColor(ImGuiCol_PopupBg, (style.bgColor & 0x00FFFFFF) | ((ImU32)style.bgAlpha << 24));
    ImGui::PushStyleColor(ImGuiCol_Border, style.borderColor);
    ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, style.overlayColor);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, style.borderWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.padding, style.padding));

    bool open = ImGui::BeginPopupModal(name, p_open, flags);

    if (open && style.showGlow) {
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();
        Draw::RectGlow(pos, ImVec2(pos.x + size.x, pos.y + size.y),
                       style.glowColor, style.glowIntensity, 8.0f);
    }

    if (!open) {
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(3);
    }

    return open;
}

bool BeginPopupModal(const char* name, bool* p_open, ImGuiWindowFlags flags) {
    return BeginPopupModalEx(name, p_open, flags, StyleSystem::GetCurrentStyle<PopupStyle>());
}

// IMPORTANT: Always use EFIGUI::EndPopupModal() with EFIGUI::BeginPopupModal().
// Mixing with ImGui::EndPopup() will cause style stack imbalance.
void EndPopupModal() {
    ImGui::EndPopup();
    // Pop in reverse order (LIFO): 3 StyleVars, then 3 StyleColors (Modal has extra dim color)
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(3);
}

void CloseCurrentPopup() {
    ImGui::CloseCurrentPopup();
}

} // namespace EFIGUI
