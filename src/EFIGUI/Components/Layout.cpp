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
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float width = ImGui::GetContentRegionAvail().x;

        ImU32 effectiveColor = color.value_or(Theme::BorderDefault);

        ImGui::GetWindowDrawList()->AddLine(
            ImVec2(pos.x, pos.y + 4.0f),
            ImVec2(pos.x + width, pos.y + 4.0f),
            effectiveColor
        );

        ImGui::Dummy(ImVec2(0, 8.0f));
    }

    void SameLine(float offset, float spacing)
    {
        ImGui::SameLine(offset, spacing);
    }
}
