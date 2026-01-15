// src/EFIGUI/Components/Popup.cpp
#include "Popup.h"
#include "../Core/Draw.h"
#include "../Core/Style.h"
#include "imgui_internal.h"

namespace EFIGUI {

#ifdef IMGUI_DEBUG_PARANOID
namespace {
    // Track style stack depths for debug assertions
    thread_local int s_popupColorStackDepth = 0;
    thread_local int s_popupVarStackDepth = 0;
    thread_local int s_modalColorStackDepth = 0;
    thread_local int s_modalVarStackDepth = 0;
}
#endif

void OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags) {
    ImGui::OpenPopup(str_id, popup_flags);
}

bool BeginPopupEx(const char* str_id, ImGuiWindowFlags flags, const PopupStyle& style) {
#ifdef IMGUI_DEBUG_PARANOID
    s_popupColorStackDepth = GImGui->ColorStack.Size;
    s_popupVarStackDepth = GImGui->StyleVarStack.Size;
#endif

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

#ifdef IMGUI_DEBUG_PARANOID
    IM_ASSERT(GImGui->ColorStack.Size == s_popupColorStackDepth &&
              "EFIGUI::EndPopup() color stack imbalance - did you mix with ImGui calls?");
    IM_ASSERT(GImGui->StyleVarStack.Size == s_popupVarStackDepth &&
              "EFIGUI::EndPopup() style var stack imbalance - did you mix with ImGui calls?");
#endif
}

bool BeginPopupModalEx(const char* name, bool* p_open, ImGuiWindowFlags flags, const PopupStyle& style) {
#ifdef IMGUI_DEBUG_PARANOID
    s_modalColorStackDepth = GImGui->ColorStack.Size;
    s_modalVarStackDepth = GImGui->StyleVarStack.Size;
#endif

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

#ifdef IMGUI_DEBUG_PARANOID
    IM_ASSERT(GImGui->ColorStack.Size == s_modalColorStackDepth &&
              "EFIGUI::EndPopupModal() color stack imbalance - did you mix with ImGui calls?");
    IM_ASSERT(GImGui->StyleVarStack.Size == s_modalVarStackDepth &&
              "EFIGUI::EndPopupModal() style var stack imbalance - did you mix with ImGui calls?");
#endif
}

void CloseCurrentPopup() {
    ImGui::CloseCurrentPopup();
}

} // namespace EFIGUI
