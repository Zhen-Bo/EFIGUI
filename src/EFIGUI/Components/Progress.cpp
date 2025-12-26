// =============================================
// EFIGUI Components - Progress
// Progress bar and status indicator
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Progress / Status
    // =============================================

    void ModernProgressBar(float fraction, ImVec2 size, const char* overlay, std::optional<Layer> layer)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        if (size.x <= 0) size.x = ImGui::GetContentRegionAvail().x;
        if (size.y <= 0) size.y = 8.0f;

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Background
        draw->AddRectFilled(
            pos,
            ImVec2(pos.x + size.x, pos.y + size.y),
            Theme::ButtonDefault,
            size.y * 0.5f
        );

        // Fill
        float fillWidth = size.x * std::clamp(fraction, 0.0f, 1.0f);
        if (fillWidth > 0)
        {
            Draw::RectGradientH(
                pos,
                ImVec2(pos.x + fillWidth, pos.y + size.y),
                Theme::AccentPurple,
                Theme::AccentCyan,
                size.y * 0.5f
            );

            // Glow effect using layer system
            ImVec2 fillSize = ImVec2(fillWidth, size.y);
            Draw::GlowLayers(pos, fillSize, Theme::AccentCyan, 0.5f, Theme::ProgressGlowLayers, Theme::ProgressGlowExpand, size.y * 0.5f, layer);
        }

        // Overlay text
        if (overlay)
        {
            ImVec2 textSize = ImGui::CalcTextSize(overlay);
            draw->AddText(
                ImVec2(pos.x + (size.x - textSize.x) * 0.5f, pos.y + size.y + 4.0f),
                Theme::TextSecondary,
                overlay
            );
        }

        ImGui::Dummy(ImVec2(size.x, size.y + (overlay ? 20.0f : 0.0f)));
    }

    void StatusIndicator(const char* label, ImU32 color, bool pulse, std::optional<float> dotSize)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        float effectiveDotSize = dotSize.value_or(Theme::DefaultDotSize);
        float alpha = pulse ? (Animation::Pulse(2.0f) * 0.5f + 0.5f) : 1.0f;

        auto colorRGB = Theme::ExtractRGB(color);
        ImU32 dotColor = IM_COL32(colorRGB.r, colorRGB.g, colorRGB.b, (int)(alpha * 255));

        draw->AddCircleFilled(
            ImVec2(pos.x + effectiveDotSize * 0.5f, pos.y + ImGui::GetFontSize() * 0.5f),
            effectiveDotSize * 0.5f,
            dotColor
        );

        draw->AddText(
            ImVec2(pos.x + effectiveDotSize + 8.0f, pos.y),
            Theme::TextPrimary,
            label
        );

        ImGui::Dummy(ImVec2(ImGui::CalcTextSize(label).x + effectiveDotSize + 8.0f, ImGui::GetFontSize()));
    }
}
