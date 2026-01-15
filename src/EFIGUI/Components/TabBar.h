// src/EFIGUI/Components/TabBar.h
#pragma once
#include "imgui.h"
#include "../Styles/TabBarStyle.h"

namespace EFIGUI {

/// Begin a styled tab bar - matches ImGui::BeginTabBar API
/// @return true if the tab bar is visible and items should be rendered
bool BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0);

/// Begin a styled tab bar with custom style
bool BeginTabBarEx(const char* str_id, ImGuiTabBarFlags flags, const TabBarStyle& style);

/// End a styled tab bar
void EndTabBar();

/// Create a tab item within a tab bar
/// @return true if the tab is selected
bool TabItem(const char* label, bool* p_open = nullptr, ImGuiTabItemFlags flags = 0);

/// Create a tab item with custom style
bool TabItemEx(const char* label, bool* p_open, ImGuiTabItemFlags flags, const TabBarStyle& style);

/// Begin a tab item (alternative API matching ImGui::BeginTabItem)
/// Use this when you need to render custom content within tabs
/// @return true if the tab is selected and content should be rendered
bool BeginTabItem(const char* label, bool* p_open = nullptr, ImGuiTabItemFlags flags = 0);

/// End a tab item (call when BeginTabItem returns true)
void EndTabItem();

} // namespace EFIGUI
