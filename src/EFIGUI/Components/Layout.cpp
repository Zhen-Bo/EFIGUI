// =============================================
// EFIGUI Components - Layout
// Spacing, separators, and layout helpers
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Spacing / Layout
    // =============================================

    void Spacing(float height)
    {
        // Use theme default if height is 0
        float effectiveHeight = (height <= 0) ? Theme::ItemSpacing() : height;
        ImGui::Dummy(ImVec2(0, effectiveHeight));
    }

    void Separator(std::optional<ImU32> color)
    {
        const auto& l = Theme::Layout();

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float width = ImGui::GetContentRegionAvail().x;

        ImU32 effectiveColor = color.value_or(Theme::BorderDefault());

        ImGui::GetWindowDrawList()->AddLine(
            ImVec2(pos.x, pos.y + l.separatorOffsetY),
            ImVec2(pos.x + width, pos.y + l.separatorOffsetY),
            effectiveColor
        );

        ImGui::Dummy(ImVec2(0, l.separatorSpacing));
    }

    void SameLine(float offset, float spacing)
    {
        ImGui::SameLine(offset, spacing);
    }
}
