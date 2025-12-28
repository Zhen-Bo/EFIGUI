// =============================================
// EFIGUI Components - Card
// Feature cards and section headers
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Card Helper Functions
    // =============================================

    // Layout information for a feature card
    struct CardLayout
    {
        float iconX;
        float textX;
        float availableTextWidth;
        bool needsWrap;
        float cardHeight;
        ImVec2 size;
    };

    // Calculate layout dimensions for a feature card
    static CardLayout CalculateCardLayout(ImVec2 pos, const char* description)
    {
        using namespace CardConstants;

        CardLayout layout;

        layout.iconX = pos.x + IconPadding;
        layout.textX = layout.iconX + IconSize + IconPadding;

        float togglePadding = IconPadding;
        float cardWidth = ImGui::GetContentRegionAvail().x;

        // Calculate available width for description text
        layout.availableTextWidth = cardWidth - (layout.textX - pos.x) - Theme::ToggleWidth() - togglePadding;

        // Check if description needs wrapping
        ImVec2 descSize = ImGui::CalcTextSize(description);
        layout.needsWrap = descSize.x > layout.availableTextWidth;

        // Calculate card height based on whether text wraps
        layout.cardHeight = BaseHeight;

        if (layout.needsWrap && layout.availableTextWidth > MinTextWidth)
        {
            ImVec2 wrappedSize = ImGui::CalcTextSize(description, nullptr, false, layout.availableTextWidth);
            float extraHeight = wrappedSize.y - ImGui::GetFontSize();
            if (extraHeight > 0)
            {
                layout.cardHeight = BaseHeight + extraHeight;
            }
        }

        layout.size = ImVec2(cardWidth, layout.cardHeight);
        return layout;
    }

    // Draw card background with hover animation
    static void DrawCardBackground(
        ImDrawList* draw,
        ImVec2 pos,
        ImVec2 size,
        float hoverAnim,
        std::optional<uint8_t> bgAlpha)
    {
        ImU32 bgBase = Theme::ButtonDefault();
        ImU32 bgHoverTarget = Theme::ButtonHover();

        // Apply custom alpha if specified
        if (bgAlpha.has_value())
        {
            bgBase = (bgBase & 0x00FFFFFF) | ((ImU32)bgAlpha.value() << 24);
            bgHoverTarget = (bgHoverTarget & 0x00FFFFFF) | ((ImU32)bgAlpha.value() << 24);
        }

        ImU32 bgColor = Animation::LerpColorU32(bgBase, bgHoverTarget, hoverAnim);
        draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, Theme::FrameRounding());
    }

    // Draw wrapped description text
    static void DrawWrappedDescription(
        ImDrawList* draw,
        const char* description,
        float textX,
        float startY,
        float availableWidth)
    {
        const char* textStart = description;
        const char* textEnd = description + strlen(description);
        float lineY = startY;
        float lineHeight = ImGui::GetFontSize();

        while (textStart < textEnd)
        {
            const char* lineEnd = ImGui::GetFont()->CalcWordWrapPositionA(
                1.0f, textStart, textEnd, availableWidth);

            if (lineEnd == textStart)
                lineEnd = textStart + 1;

            draw->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
                ImVec2(textX, lineY), Theme::TextMuted(), textStart, lineEnd);

            lineY += lineHeight;
            textStart = lineEnd;

            while (textStart < textEnd && *textStart == ' ')
                textStart++;
        }
    }

    // Draw inline toggle switch for card
    static void DrawCardToggle(
        ImDrawList* draw,
        ImVec2 pos,
        ImVec2 size,
        float slideAnim)
    {
        using namespace CardConstants;

        float toggleX = pos.x + size.x - ToggleRightMargin;
        float toggleY = pos.y + (size.y - ToggleHeight) * 0.5f;

        // Toggle track
        ImU32 toggleColor = Animation::LerpColorU32(
            Theme::ButtonDefault(),
            Theme::AccentCyan(),
            slideAnim
        );
        draw->AddRectFilled(
            ImVec2(toggleX, toggleY),
            ImVec2(toggleX + ToggleWidth, toggleY + ToggleHeight),
            toggleColor,
            ToggleRounding
        );

        // Toggle knob
        float knobX = toggleX + KnobPadding + slideAnim * KnobTravel;
        draw->AddCircleFilled(
            ImVec2(knobX + KnobRadius, toggleY + ToggleHeight * 0.5f),
            KnobRadius,
            Theme::TextPrimary()
        );
    }

    // =============================================
    // Cards / Sections
    // =============================================

    bool FeatureCard(const char* icon, const char* name, const char* description, bool* enabled, std::optional<uint8_t> bgAlpha)
    {
        using namespace CardConstants;

        ImGuiID id = ImGui::GetID(name);
        Animation::WidgetState& state = Animation::GetState(id);

        // Sync initial state on first frame to avoid visual transition
        bool isOn = enabled ? *enabled : false;
        if (state.lastUpdateFrame == 0)
        {
            state.slideAnim = isOn ? 1.0f : 0.0f;
            state.selectedAnim = isOn ? 1.0f : 0.0f;
        }

        ImVec2 pos = ImGui::GetCursorScreenPos();
        CardLayout layout = CalculateCardLayout(pos, description);

        // Interaction
        ImGui::InvisibleButton(name, layout.size);
        bool clicked = ImGui::IsItemClicked();
        bool hovered = ImGui::IsItemHovered();

        if (clicked && enabled)
        {
            *enabled = !*enabled;
        }

        isOn = enabled ? *enabled : false;
        Animation::UpdateWidgetState(state, hovered, false, isOn);
        state.slideAnim = Animation::Lerp(state.slideAnim, isOn ? 1.0f : 0.0f, ToggleAnimSpeed);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Background
        DrawCardBackground(draw, pos, layout.size, state.hoverAnim, bgAlpha);

        // Icon
        float iconY = pos.y + (layout.size.y - IconSize) * 0.5f;
        ImU32 iconColor = isOn ? Theme::AccentCyan() : Theme::TextMuted();
        draw->AddText(ImVec2(layout.iconX + IconTextOffset, iconY + IconTextOffset), iconColor, icon);

        // Name
        draw->AddText(ImVec2(layout.textX, pos.y + NameOffsetY), Theme::TextPrimary(), name);

        // Description
        if (layout.needsWrap && layout.availableTextWidth > MinTextWidth)
        {
            DrawWrappedDescription(draw, description, layout.textX, pos.y + DescOffsetY, layout.availableTextWidth);
        }
        else
        {
            draw->AddText(ImVec2(layout.textX, pos.y + DescOffsetY), Theme::TextMuted(), description);
        }

        // Toggle
        DrawCardToggle(draw, pos, layout.size, state.slideAnim);

        return clicked;
    }

    bool SectionHeader(const char* label, bool* collapsed, std::optional<ImU32> accentColor)
    {
        using namespace CardConstants;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        ImU32 effectiveAccentColor = accentColor.value_or(Theme::TextAccent());

        draw->AddText(
            ImVec2(pos.x, pos.y),
            effectiveAccentColor,
            label
        );

        // Line under header
        float lineY = pos.y + ImGui::GetFontSize() + SectionLineOffset;
        draw->AddLine(
            ImVec2(pos.x, lineY),
            ImVec2(pos.x + ImGui::GetContentRegionAvail().x, lineY),
            Theme::BorderDefault()
        );

        ImGui::Dummy(ImVec2(0, ImGui::GetFontSize() + SectionSpacing));

        return collapsed ? !*collapsed : true;
    }
}
