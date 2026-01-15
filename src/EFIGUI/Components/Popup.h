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
///
/// @warning MUST be paired with EFIGUI::BeginPopup(), NOT ImGui::BeginPopup().
///          Mixing EFIGUI and ImGui popup calls will cause style stack corruption.
///
/// @note This function pops 3 style vars and 2 style colors that were pushed
///       by BeginPopup(). The push/pop counts are:
///       - BeginPopup:  Push 2 colors, 3 vars
///       - EndPopup:    Pop  3 vars,  2 colors (LIFO order)
///
/// @example Correct usage:
/// @code
///     if (EFIGUI::BeginPopup("MyPopup")) {
///         // ... popup content ...
///         EFIGUI::EndPopup();  // Correct
///     }
/// @endcode
///
/// @example WRONG usage (will crash):
/// @code
///     if (EFIGUI::BeginPopup("MyPopup")) {
///         // ... popup content ...
///         ImGui::EndPopup();  // WRONG! Style stack corruption!
///     }
/// @endcode
void EndPopup();

/// Begin modal popup - matches ImGui::BeginPopupModal API
/// IMPORTANT: Must be paired with EFIGUI::EndPopupModal(), NOT ImGui::EndPopup()
bool BeginPopupModal(const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = 0);

/// Begin modal with custom style
bool BeginPopupModalEx(const char* name, bool* p_open, ImGuiWindowFlags flags, const PopupStyle& style);

/// End modal popup - must be called after BeginPopupModal() returns true
///
/// @warning MUST be paired with EFIGUI::BeginPopupModal(), NOT ImGui::BeginPopupModal().
///          Modal pops 3 colors (includes ModalWindowDimBg) vs regular popup's 2 colors.
///
/// @note Push/pop counts for modal:
///       - BeginPopupModal:  Push 3 colors, 3 vars
///       - EndPopupModal:    Pop  3 vars,  3 colors (LIFO order)
void EndPopupModal();

/// Close current popup
void CloseCurrentPopup();

} // namespace EFIGUI
