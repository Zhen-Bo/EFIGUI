// =============================================
// EFIGUI Components - Tooltip
// Modern tooltips and help markers
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Tooltip
    // =============================================

    void ModernTooltip(const char* text)
    {
        const auto& l = Theme::Layout();

        ImGui::PushStyleColor(ImGuiCol_PopupBg, Theme::ToVec4(Theme::BackgroundPanel()));
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::ToVec4(Theme::BorderDefault()));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(l.tooltipPaddingX, l.tooltipPaddingY));
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, Theme::FrameRounding());

        ImGui::SetTooltip("%s", text);

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(2);
    }

    void HelpMarker(const char* text)
    {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered())
        {
            ModernTooltip(text);
        }
    }
}
