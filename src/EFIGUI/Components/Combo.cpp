// =============================================
// EFIGUI Components - Combo
// Modern dropdown/combobox
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Combo / Dropdown
    // =============================================

    bool ModernCombo(const char* label, int* current_item, const char* const items[], int items_count, std::optional<uint8_t> popupBgAlpha)
    {
        // Determine alpha to use (nullopt = use Theme default)
        uint8_t alpha = popupBgAlpha.value_or((Theme::BackgroundPanel() >> 24) & 0xFF);
        ImU32 popupBgColor = (Theme::BackgroundPanel() & 0x00FFFFFF) | ((ImU32)alpha << 24);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::ToVec4(Theme::ButtonDefault()));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Theme::ToVec4(Theme::ButtonHover()));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Theme::ToVec4(Theme::ButtonHover()));
        ImGui::PushStyleColor(ImGuiCol_PopupBg, Theme::ToVec4(popupBgColor));
        ImGui::PushStyleColor(ImGuiCol_Header, Theme::ToVec4(Theme::AccentCyan()));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(0, 245, 255, 80));
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::ToVec4(Theme::BorderDefault()));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Theme::FrameRounding());
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, Theme::FrameRounding());
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

        bool result = ImGui::Combo(label, current_item, items, items_count);

        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor(7);

        return result;
    }
}
