// src/EFIGUI/Components/TreeNode.cpp
#include "TreeNode.h"
#include "../Core/Animation.h"
#include "../Core/Style.h"
#include "imgui_internal.h"
#include <cstdarg>

namespace EFIGUI {

bool TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags, const TreeNodeStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiID id = window->GetID(label);

    // Get animation state for smooth transitions
    Animation::WidgetState& state = Animation::GetState(id);

    // Push style colors
    ImGui::PushStyleColor(ImGuiCol_Header, style.bgColor);
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, style.bgHoverColor);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, style.bgActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    bool is_open = ImGui::TreeNodeEx(label, flags);

    bool hovered = ImGui::IsItemHovered();
    Animation::UpdateWidgetState(state, hovered, false, is_open);

    ImGui::PopStyleColor(4);

    // Custom arrow glow effect when open
    if (state.selectedAnim > 0.01f && !(flags & ImGuiTreeNodeFlags_Leaf)) {
        ImDrawList* draw = window->DrawList;
        ImVec2 item_min = ImGui::GetItemRectMin();

        // Draw glow around arrow area
        float arrowX = item_min.x + ImGui::GetTreeNodeToLabelSpacing() * 0.5f - style.arrowSize * 0.5f;
        float arrowY = item_min.y + ImGui::GetFrameHeight() * 0.5f;
        ImVec2 arrowCenter(arrowX, arrowY);

        ImU32 glowColor = Animation::LerpColorU32(IM_COL32(0, 0, 0, 0), style.glowColor,
                                                   state.selectedAnim * style.glowIntensity);
        float glowRadius = style.arrowSize * (1.0f + state.selectedAnim * 0.5f);

        draw->AddCircleFilled(arrowCenter, glowRadius, glowColor);
    }

    // Draw connector lines if enabled
    if (style.showConnectorLines && is_open) {
        ImDrawList* draw = window->DrawList;
        ImVec2 item_min = ImGui::GetItemRectMin();
        float lineX = item_min.x + style.indentWidth * 0.5f;
        // Connector line will be drawn by child nodes
        // Store current position for child connection
    }

    return is_open;
}

bool TreeNode(const char* label, ImGuiTreeNodeFlags flags) {
    return TreeNodeEx(label, flags, StyleSystem::GetCurrentStyle<TreeNodeStyle>());
}

bool TreeNode(const char* str_id, const char* fmt, ...) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    va_list args;
    va_start(args, fmt);
    char buf[256];
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    ImGui::PushID(str_id);
    bool result = TreeNode(buf, 0);
    if (!result)
        ImGui::PopID();
    return result;
}

void TreePop() {
    ImGui::TreePop();
    // Note: Do NOT call PopID() here - ImGui::TreePop() handles the full scope.
    // The PushID() in the variadic TreeNode(str_id, fmt, ...) is balanced by
    // the PopID() that happens when result is false (see line 80).
    // When result is true, the ID is popped by ImGui::TreePop() internally.
}

bool CollapsingHeaderEx(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags, const TreeNodeStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiID id = window->GetID(label);

    // Get animation state
    Animation::WidgetState& state = Animation::GetState(id);

    // Push style colors
    ImGui::PushStyleColor(ImGuiCol_Header, style.bgColor);
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, style.bgHoverColor);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, style.bgActiveColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.textColor);

    bool is_open = p_visible ?
        ImGui::CollapsingHeader(label, p_visible, flags) :
        ImGui::CollapsingHeader(label, flags);

    bool hovered = ImGui::IsItemHovered();
    Animation::UpdateWidgetState(state, hovered, false, is_open);

    ImGui::PopStyleColor(4);

    // Glow effect when expanded
    if (state.selectedAnim > 0.01f) {
        ImDrawList* draw = window->DrawList;
        ImVec2 item_min = ImGui::GetItemRectMin();
        ImVec2 item_max = ImGui::GetItemRectMax();

        ImU32 glowColor = Animation::LerpColorU32(IM_COL32(0, 0, 0, 0), style.glowColor,
                                                   state.selectedAnim * style.glowIntensity);

        // Glow along bottom edge
        ImVec2 glowMin(item_min.x, item_max.y - 2.0f);
        ImVec2 glowMax(item_max.x, item_max.y + 4.0f);
        draw->AddRectFilled(glowMin, glowMax, glowColor, style.rounding);
    }

    return is_open;
}

bool CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags) {
    return CollapsingHeaderEx(label, nullptr, flags, StyleSystem::GetCurrentStyle<TreeNodeStyle>());
}

bool CollapsingHeader(const char* label, bool* p_visible, ImGuiTreeNodeFlags flags) {
    return CollapsingHeaderEx(label, p_visible, flags, StyleSystem::GetCurrentStyle<TreeNodeStyle>());
}

} // namespace EFIGUI
