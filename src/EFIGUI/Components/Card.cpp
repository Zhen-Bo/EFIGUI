// =============================================
// EFIGUI Components - Card
// Feature cards and section headers
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Cards / Sections
    // =============================================

    bool FeatureCard(const char* icon, const char* name, const char* description, bool* enabled, std::optional<uint8_t> bgAlpha)
    {
        ImGuiID id = ImGui::GetID(name);
        Animation::WidgetState& state = Animation::GetState(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();

        // Calculate layout dimensions
        float iconSize = 32.0f;
        float iconX = pos.x + 12.0f;
        float textX = iconX + iconSize + 12.0f;
        float toggleWidth = Theme::ToggleWidth;
        float togglePadding = 12.0f;
        float cardWidth = ImGui::GetContentRegionAvail().x;

        // Calculate available width for description text
        float availableTextWidth = cardWidth - (textX - pos.x) - toggleWidth - togglePadding;

        // Check if description needs wrapping
        ImVec2 descSize = ImGui::CalcTextSize(description);
        bool needsWrap = descSize.x > availableTextWidth;

        // Calculate card height based on whether text wraps
        float baseHeight = 56.0f;
        float cardHeight = baseHeight;

        if (needsWrap && availableTextWidth > 50.0f)
        {
            // Calculate wrapped text height
            ImVec2 wrappedSize = ImGui::CalcTextSize(description, nullptr, false, availableTextWidth);
            float extraHeight = wrappedSize.y - ImGui::GetFontSize();
            if (extraHeight > 0)
            {
                cardHeight = baseHeight + extraHeight;
            }
        }

        ImVec2 size = ImVec2(cardWidth, cardHeight);

        ImGui::InvisibleButton(name, size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();

        if (clicked && enabled)
        {
            *enabled = !*enabled;
        }

        bool isOn = enabled ? *enabled : false;
        Animation::UpdateWidgetState(state, hovered, false, isOn);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Background with optional alpha override
        ImU32 bgBase = Theme::ButtonDefault;
        ImU32 bgHoverTarget = Theme::ButtonHover;

        // Apply custom alpha if specified (nullopt = use Theme default)
        if (bgAlpha.has_value())
        {
            bgBase = (bgBase & 0x00FFFFFF) | ((ImU32)bgAlpha.value() << 24);
            bgHoverTarget = (bgHoverTarget & 0x00FFFFFF) | ((ImU32)bgAlpha.value() << 24);
        }

        ImU32 bgColor = Animation::LerpColorU32(
            bgBase,
            bgHoverTarget,
            state.hoverAnim
        );
        draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, Theme::FrameRounding);

        // Icon
        float iconY = pos.y + (size.y - iconSize) * 0.5f;
        ImU32 iconColor = isOn ? Theme::AccentCyan : Theme::TextMuted;
        draw->AddText(ImVec2(iconX + 6.0f, iconY + 6.0f), iconColor, icon);

        // Name
        draw->AddText(ImVec2(textX, pos.y + 10.0f), Theme::TextPrimary, name);

        // Description - with automatic wrapping if needed
        if (needsWrap && availableTextWidth > 50.0f)
        {
            const char* textStart = description;
            const char* textEnd = description + strlen(description);
            float lineY = pos.y + 28.0f;
            float lineHeight = ImGui::GetFontSize();

            while (textStart < textEnd)
            {
                const char* lineEnd = ImGui::GetFont()->CalcWordWrapPositionA(
                    1.0f, textStart, textEnd, availableTextWidth);

                if (lineEnd == textStart)
                    lineEnd = textStart + 1;

                draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
                    ImVec2(textX, lineY), Theme::TextMuted, textStart, lineEnd);

                lineY += lineHeight;
                textStart = lineEnd;

                while (textStart < textEnd && *textStart == ' ')
                    textStart++;
            }
        }
        else
        {
            draw->AddText(ImVec2(textX, pos.y + 28.0f), Theme::TextMuted, description);
        }

        // Toggle indicator on right (vertically centered)
        float toggleX = pos.x + size.x - 48.0f;
        float toggleY = pos.y + (size.y - 24.0f) * 0.5f;

        // Simple toggle visual
        state.slideAnim = Animation::Lerp(state.slideAnim, isOn ? 1.0f : 0.0f, 12.0f);

        ImU32 toggleColor = Animation::LerpColorU32(
            Theme::ButtonDefault,
            Theme::AccentCyan,
            state.slideAnim
        );
        draw->AddRectFilled(
            ImVec2(toggleX, toggleY),
            ImVec2(toggleX + 40.0f, toggleY + 24.0f),
            toggleColor,
            12.0f
        );

        float knobX = toggleX + 3.0f + state.slideAnim * 16.0f;
        draw->AddCircleFilled(
            ImVec2(knobX + 9.0f, toggleY + 12.0f),
            9.0f,
            Theme::TextPrimary
        );

        return clicked;
    }

    bool SectionHeader(const char* label, bool* collapsed, std::optional<ImU32> accentColor)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        ImU32 effectiveAccentColor = accentColor.value_or(Theme::TextAccent);

        draw->AddText(
            ImVec2(pos.x, pos.y),
            effectiveAccentColor,
            label
        );

        // Line under header
        float lineY = pos.y + ImGui::GetFontSize() + 4.0f;
        draw->AddLine(
            ImVec2(pos.x, lineY),
            ImVec2(pos.x + ImGui::GetContentRegionAvail().x, lineY),
            Theme::BorderDefault
        );

        ImGui::Dummy(ImVec2(0, ImGui::GetFontSize() + 12.0f));

        return collapsed ? !*collapsed : true;
    }
}
