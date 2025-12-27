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
        ImGui::Dummy(ImVec2(0, height));
    }

    void Separator(std::optional<ImU32> color)
    {
        using namespace LayoutConstants;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float width = ImGui::GetContentRegionAvail().x;

        ImU32 effectiveColor = color.value_or(Theme::BorderDefault);

        ImGui::GetWindowDrawList()->AddLine(
            ImVec2(pos.x, pos.y + SeparatorOffsetY),
            ImVec2(pos.x + width, pos.y + SeparatorOffsetY),
            effectiveColor
        );

        ImGui::Dummy(ImVec2(0, SeparatorSpacing));
    }

    void SameLine(float offset, float spacing)
    {
        ImGui::SameLine(offset, spacing);
    }
}
