// src/EFIGUI/Components/Popup.h
#pragma once
#include "imgui.h"
#include "../Styles/PopupStyle.h"

namespace EFIGUI {

/// Open a popup - matches ImGui::OpenPopup API
void OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0);

/// Begin popup - matches ImGui::BeginPopup API
/// IMPORTANT: Must be paired with EFIGUI::EndPopup(), NOT ImGui::EndPopup()
bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);

/// Begin popup with custom style
bool BeginPopupEx(const char* str_id, ImGuiWindowFlags flags, const PopupStyle& style);

/// End popup - must be called after BeginPopup() returns true
/// WARNING: Do not use ImGui::EndPopup() - it will cause style stack imbalance
void EndPopup();

/// Begin modal popup - matches ImGui::BeginPopupModal API
/// IMPORTANT: Must be paired with EFIGUI::EndPopupModal(), NOT ImGui::EndPopup()
bool BeginPopupModal(const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = 0);

/// Begin modal with custom style
bool BeginPopupModalEx(const char* name, bool* p_open, ImGuiWindowFlags flags, const PopupStyle& style);

/// End modal popup - must be called after BeginPopupModal() returns true
/// WARNING: Do not use ImGui::EndPopup() - it will cause style stack imbalance
void EndPopupModal();

/// Close current popup
void CloseCurrentPopup();

} // namespace EFIGUI
