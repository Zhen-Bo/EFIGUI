// src/EFIGUI/Components/TreeNode.h
#pragma once
#include "imgui.h"
#include "../Styles/TreeNodeStyle.h"

namespace EFIGUI {

/// Tree node with animated arrow - matches ImGui::TreeNode API
bool TreeNode(const char* label, ImGuiTreeNodeFlags flags = 0);

/// Tree node with custom style
bool TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags, const TreeNodeStyle& style);

/// Tree node with string ID
bool TreeNode(const char* str_id, const char* fmt, ...);

/// End a tree node scope (call when TreeNode returns true)
void TreePop();

/// Collapsing header - matches ImGui::CollapsingHeader API
bool CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);

/// Collapsing header with close button
bool CollapsingHeader(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags = 0);

/// Collapsing header with custom style
bool CollapsingHeaderEx(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags, const TreeNodeStyle& style);

} // namespace EFIGUI
