// src/EFIGUI/Components/Menu.h
#pragma once
#include "imgui.h"
#include "../Styles/MenuStyle.h"

namespace EFIGUI {

/// Begin the main menu bar at the top of the screen
bool BeginMainMenuBar();

/// End the main menu bar
void EndMainMenuBar();

/// Begin a menu bar (typically inside a window)
bool BeginMenuBar();

/// End a menu bar
void EndMenuBar();

/// Begin a menu (typically from a menu bar)
bool BeginMenu(const char* label, bool enabled = true);

/// Begin a menu with custom style
bool BeginMenuEx(const char* label, bool enabled, const MenuStyle& style);

/// End a menu
void EndMenu();

/// Create a menu item
/// @return true if clicked
bool MenuItem(const char* label, const char* shortcut = nullptr,
              bool selected = false, bool enabled = true);

/// Create a menu item that toggles a bool value
/// @return true if clicked
bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);

/// Create a menu item with custom style
bool MenuItemEx(const char* label, const char* shortcut, bool selected, bool enabled,
                const MenuStyle& style);

/// Add a separator line in a menu
void MenuSeparator();

/// Add a header text in a menu (non-interactive)
void MenuHeader(const char* label);

} // namespace EFIGUI
