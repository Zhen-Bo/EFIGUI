// =============================================
// EFIGUI Components - Glass Panel
// Semi-transparent panel with blur background effect
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Glass Panel
    // =============================================

    void BeginGlassPanel(const char* id, ImVec2 size, bool border, std::optional<uint8_t> bgAlpha, std::optional<ImU32> glowColor)
    {
        ImGui::PushID(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();

        if (size.x <= 0) size.x = ImGui::GetContentRegionAvail().x;
        if (size.y <= 0) size.y = ImGui::GetContentRegionAvail().y;

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Determine alpha to use (nullopt = use Theme default)
        uint8_t alpha = bgAlpha.value_or((Theme::BackgroundContent() >> 24) & 0xFF);

        // Extract RGB from Theme color and apply custom alpha
        ImU32 bgColor = (Theme::BackgroundContent() & 0x00FFFFFF) | ((ImU32)alpha << 24);

        // Simple solid background (blur effect is on main window only)
        draw->AddRectFilled(
            pos,
            ImVec2(pos.x + size.x, pos.y + size.y),
            bgColor,
            Theme::FrameRounding()
        );

        if (border)
        {
            // Subtle border
            draw->AddRect(
                pos,
                ImVec2(pos.x + size.x, pos.y + size.y),
                Theme::BorderDefault(),
                Theme::FrameRounding(),
                0,
                1.0f
            );

            // Glow effect on top edge - use custom or theme color
            ImU32 effectiveGlowColor = glowColor.value_or(Theme::AccentCyan());
            float glowIntensity = Animation::Breathe(1.0f) * 0.3f + 0.2f;
            auto glowRGB = Theme::ExtractRGB(effectiveGlowColor);
            ImU32 glowColorWithAlpha = IM_COL32(glowRGB.r, glowRGB.g, glowRGB.b, (int)(glowIntensity * 80));
            draw->AddLine(
                ImVec2(pos.x + Theme::FrameRounding(), pos.y),
                ImVec2(pos.x + size.x - Theme::FrameRounding(), pos.y),
                glowColorWithAlpha,
                2.0f
            );
        }

        ImGui::BeginChild(id, size, false, ImGuiWindowFlags_NoBackground);

        // Apply internal padding by creating an inner child with margins
        float padding = Theme::ContentPadding();
        ImVec2 innerSize = ImVec2(size.x - padding * 2, size.y - padding * 2);
        ImGui::SetCursorPos(ImVec2(padding, padding));
        ImGui::BeginChild("##GlassPanelInner", innerSize, false, ImGuiWindowFlags_NoBackground);
    }

    void EndGlassPanel()
    {
        ImGui::EndChild();  // End inner padded child
        ImGui::EndChild();  // End outer glass panel child
        ImGui::PopID();
    }
}
