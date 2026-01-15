// src/EFIGUI/Components/TabBar.cpp
#include "TabBar.h"
#include "../Core/Animation.h"
#include "../Core/Style.h"
#include "imgui_internal.h"

namespace EFIGUI {

// Storage for current tab bar style during Begin/End scope.
// Note: Using thread_local here instead of StyleSystem::GetComponentState<> is intentional.
// TabBar styles are short-lived (Begin to End), not persistent across frames like Drag state.
// thread_local is simpler and sufficient for this use case.
static thread_local TabBarStyle s_currentTabBarStyle;

bool BeginTabBarEx(const char* str_id, ImGuiTabBarFlags flags, const TabBarStyle& style) {
    s_currentTabBarStyle = style;

    // Push ImGui style colors
    ImGui::PushStyleColor(ImGuiCol_Tab, style.tabBgColor);
    ImGui::PushStyleColor(ImGuiCol_TabHovered, style.tabBgHoverColor);
    ImGui::PushStyleColor(ImGuiCol_TabSelected, style.tabBgActiveColor);
    ImGui::PushStyleColor(ImGuiCol_TabSelectedOverline, style.indicatorColor);
    ImGui::PushStyleColor(ImGuiCol_Text, style.tabTextColor);

    // Push style vars
    ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, style.rounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(style.tabPadding, 4.0f));

    return ImGui::BeginTabBar(str_id, flags);
}

bool BeginTabBar(const char* str_id, ImGuiTabBarFlags flags) {
    return BeginTabBarEx(str_id, flags, StyleSystem::GetCurrentStyle<TabBarStyle>());
}

void EndTabBar() {
    ImGui::EndTabBar();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(5);
}

bool TabItemEx(const char* label, bool* p_open, ImGuiTabItemFlags flags, const TabBarStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiID id = window->GetID(label);

    // Get animation state for smooth transitions
    Animation::WidgetState& state = Animation::GetState(id);

    // Use ImGui's tab item rendering, but we'll add glow effect
    ImGui::PushStyleColor(ImGuiCol_Text, state.selectedAnim > 0.5f ? style.tabTextActiveColor : style.tabTextColor);

    bool selected = ImGui::TabItemButton(label, flags);

    // Update animation state
    bool is_selected = ImGui::IsItemActivated() || (ImGui::GetCurrentContext()->CurrentTabBar &&
                        ImGui::GetCurrentContext()->CurrentTabBar->SelectedTabId == id);
    Animation::UpdateWidgetState(state, ImGui::IsItemHovered(), false, is_selected);

    ImGui::PopStyleColor(1);

    // Add glow effect when selected
    if (state.selectedAnim > 0.01f) {
        ImVec2 item_min = ImGui::GetItemRectMin();
        ImVec2 item_max = ImGui::GetItemRectMax();
        ImDrawList* draw = window->DrawList;

        // Draw glow under the indicator
        ImU32 glowColor = Animation::LerpColorU32(IM_COL32(0, 0, 0, 0), style.indicatorGlowColor,
                                                   state.selectedAnim * style.glowIntensity);
        float glowRadius = 8.0f * state.selectedAnim;

        ImVec2 indicatorMin(item_min.x, item_max.y - style.indicatorHeight);
        ImVec2 indicatorMax(item_max.x, item_max.y);

        // Glow layers
        for (int i = 0; i < 3; ++i) {
            float expand = glowRadius * (1.0f + i * 0.5f);
            ImVec2 gMin(indicatorMin.x - expand, indicatorMin.y - expand);
            ImVec2 gMax(indicatorMax.x + expand, indicatorMax.y + expand);
            draw->AddRectFilled(gMin, gMax, glowColor, style.rounding);
        }
    }

    return selected;
}

bool TabItem(const char* label, bool* p_open, ImGuiTabItemFlags flags) {
    return TabItemEx(label, p_open, flags, s_currentTabBarStyle);
}

bool BeginTabItem(const char* label, bool* p_open, ImGuiTabItemFlags flags) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiID id = window->GetID(label);
    Animation::WidgetState& state = Animation::GetState(id);

    // Push text color based on selection state
    ImGui::PushStyleColor(ImGuiCol_Text,
        state.selectedAnim > 0.5f ? s_currentTabBarStyle.tabTextActiveColor : s_currentTabBarStyle.tabTextColor);

    bool result = ImGui::BeginTabItem(label, p_open, flags);

    // Update animation state
    Animation::UpdateWidgetState(state, ImGui::IsItemHovered(), false, result);

    if (!result) {
        ImGui::PopStyleColor(1);
    }

    return result;
}

void EndTabItem() {
    ImGui::EndTabItem();
    ImGui::PopStyleColor(1);
}

} // namespace EFIGUI
