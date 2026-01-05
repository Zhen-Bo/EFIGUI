// =============================================
// EFIGUI Components - Card
// Feature cards and section headers
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Card Local Constants (non-configurable)
    // =============================================

    namespace CardLocal
    {
        // Currently empty - all constants moved to CardTheme
        // This namespace exists for future non-configurable values
    }

    // =============================================
    // Card Helper Structures
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

    // =============================================
    // Card Helper Functions
    // =============================================

    // Calculate layout dimensions for a feature card
    static CardLayout CalculateCardLayout(ImVec2 pos, const char* description, const CardConfig& config)
    {
        using namespace CardLocal;

        // Resolve config values with Theme defaults
        const auto& t = Theme::Card();
        const float iconSize = Resolve(config.iconSize, t.iconSize);
        const float iconPadding = Resolve(config.iconPadding, t.iconPadding);
        const float baseHeight = Resolve(config.height, t.baseHeight);
        const float minTextWidth = Resolve(config.minTextWidth, t.minTextWidth);

        // Semantic padding parameters
        const float topPadding = Resolve(config.topPadding, t.topPadding);
        const float bottomPadding = Resolve(config.bottomPadding, t.bottomPadding);
        const float titleDescGap = Resolve(config.titleDescGap, t.titleDescGap);

        CardLayout layout;

        layout.iconX = pos.x + iconPadding;
        layout.textX = layout.iconX + iconSize + iconPadding;

        float togglePadding = iconPadding;
        float cardWidth = ImGui::GetContentRegionAvail().x;

        // Calculate available width for description text
        layout.availableTextWidth = cardWidth - (layout.textX - pos.x) - Theme::ToggleWidth() - togglePadding;

        // Check if description needs wrapping
        ImVec2 descSize = ImGui::CalcTextSize(description);
        layout.needsWrap = descSize.x > layout.availableTextWidth;

        // Dual-path height calculation
        const bool useLegacy = (baseHeight > 0.0f);

        if (useLegacy)
        {
            // Legacy: fixed height + extra height for wrapped text
            layout.cardHeight = baseHeight;

            if (layout.needsWrap && layout.availableTextWidth > minTextWidth)
            {
                ImVec2 wrappedSize = ImGui::CalcTextSize(description, nullptr, false, layout.availableTextWidth);
                float extraHeight = wrappedSize.y - ImGui::GetFontSize();
                if (extraHeight > 0)
                {
                    layout.cardHeight = baseHeight + extraHeight;
                }
            }
        }
        else
        {
            // Semantic: content-driven calculation with consistent padding
            const float fontSize = ImGui::GetFontSize();
            float descHeight = fontSize;

            if (layout.needsWrap && layout.availableTextWidth > minTextWidth)
            {
                // Count actual lines using the same algorithm as DrawWrappedDescription
                const char* textStart = description;
                const char* textEnd = description + strlen(description);
                int lineCount = 0;

                while (textStart < textEnd)
                {
                    const char* lineEnd = ImGui::GetFont()->CalcWordWrapPositionA(
                        1.0f, textStart, textEnd, layout.availableTextWidth);

                    if (lineEnd == textStart)
                        lineEnd = textStart + 1;

                    lineCount++;
                    textStart = lineEnd;

                    while (textStart < textEnd && *textStart == ' ')
                        textStart++;
                }

                descHeight = lineCount * fontSize;
            }

            layout.cardHeight = topPadding + fontSize + titleDescGap + descHeight + bottomPadding;
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
        const CardConfig& config)
    {
        ImU32 bgBase = config.bgColor.value_or(Theme::ButtonDefault());
        ImU32 bgHoverTarget = Theme::ButtonHover();

        // Apply custom alpha if specified
        if (config.bgAlpha.has_value())
        {
            bgBase = (bgBase & 0x00FFFFFF) | ((ImU32)config.bgAlpha.value() << 24);
            bgHoverTarget = (bgHoverTarget & 0x00FFFFFF) | ((ImU32)config.bgAlpha.value() << 24);
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
        float slideAnim,
        const CardConfig& config)
    {
        using namespace CardLocal;

        // Resolve config values with Theme defaults
        const auto& t = Theme::Card();
        const float toggleWidth = Resolve(config.toggleWidth, t.toggleWidth);
        const float toggleHeight = Resolve(config.toggleHeight, t.toggleHeight);
        const float toggleRounding = Resolve(config.toggleRounding, t.toggleRounding);
        const float toggleRightMargin = Resolve(config.toggleRightMargin, t.toggleRightMargin);
        const float knobRadius = Resolve(config.knobRadius, t.knobRadius);
        const float knobPadding = Resolve(config.knobPadding, t.knobPadding);
        const float knobTravel = Resolve(config.knobTravel, t.knobTravel);

        float toggleX = pos.x + size.x - toggleRightMargin;
        float toggleY = pos.y + (size.y - toggleHeight) * 0.5f;

        // Toggle track
        ImU32 toggleColor = Animation::LerpColorU32(
            Theme::ButtonDefault(),
            Theme::AccentCyan(),
            slideAnim
        );
        draw->AddRectFilled(
            ImVec2(toggleX, toggleY),
            ImVec2(toggleX + toggleWidth, toggleY + toggleHeight),
            toggleColor,
            toggleRounding
        );

        // Toggle knob
        float knobX = toggleX + knobPadding + slideAnim * knobTravel;
        draw->AddCircleFilled(
            ImVec2(knobX + knobRadius, toggleY + toggleHeight * 0.5f),
            knobRadius,
            Theme::TextPrimary()
        );
    }

    // =============================================
    // Cards / Sections (Config version - recommended)
    // =============================================

    bool FeatureCard(const char* icon, const char* name, const char* description, bool* enabled, const CardConfig& config)
    {
        using namespace CardLocal;

        // Resolve config values with Theme defaults
        const auto& t = Theme::Card();
        const float iconSize = Resolve(config.iconSize, t.iconSize);
        const float iconTextOffset = Resolve(config.iconTextOffset, t.iconTextOffset);
        const float minTextWidth = Resolve(config.minTextWidth, t.minTextWidth);
        const float toggleAnimSpeed = Resolve(config.toggleAnimSpeed, t.toggleAnimSpeed);

        // Dual-path text positioning
        const float baseHeight = Resolve(config.height, t.baseHeight);
        const bool useLegacy = (baseHeight > 0.0f);

        float nameY, descY;
        if (useLegacy)
        {
            // Legacy: use fixed offsets
            nameY = Resolve(config.nameOffsetY, t.nameOffsetY);
            descY = Resolve(config.descOffsetY, t.descOffsetY);
        }
        else
        {
            // Semantic: calculate from padding parameters
            const float topPadding = Resolve(config.topPadding, t.topPadding);
            const float titleDescGap = Resolve(config.titleDescGap, t.titleDescGap);
            const float fontSize = ImGui::GetFontSize();

            nameY = topPadding;
            descY = topPadding + fontSize + titleDescGap;
        }

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
        CardLayout layout = CalculateCardLayout(pos, description, config);

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
        state.slideAnim = Animation::Lerp(state.slideAnim, isOn ? 1.0f : 0.0f, toggleAnimSpeed);

        ImDrawList* draw = ImGui::GetWindowDrawList();

        // Background
        DrawCardBackground(draw, pos, layout.size, state.hoverAnim, config);

        // Icon
        float iconY = pos.y + (layout.size.y - iconSize) * 0.5f;
        ImU32 iconColor = isOn ? Theme::AccentCyan() : Theme::TextMuted();
        draw->AddText(ImVec2(layout.iconX + iconTextOffset, iconY + iconTextOffset), iconColor, icon);

        // Name
        draw->AddText(ImVec2(layout.textX, pos.y + nameY), Theme::TextPrimary(), name);

        // Description
        if (layout.needsWrap && layout.availableTextWidth > minTextWidth)
        {
            DrawWrappedDescription(draw, description, layout.textX, pos.y + descY, layout.availableTextWidth);
        }
        else
        {
            draw->AddText(ImVec2(layout.textX, pos.y + descY), Theme::TextMuted(), description);
        }

        // Toggle
        DrawCardToggle(draw, pos, layout.size, state.slideAnim, config);

        return clicked;
    }

    bool SectionHeader(const char* label, bool* collapsed, const CardConfig& config)
    {
        using namespace CardLocal;

        // Resolve config values with Theme defaults
        const auto& t = Theme::Card();
        const float sectionLineOffset = Resolve(config.sectionLineOffset, t.sectionLineOffset);
        const float sectionSpacing = Resolve(config.sectionSpacing, t.sectionSpacing);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        draw->AddText(
            ImVec2(pos.x, pos.y),
            Theme::TextAccent(),
            label
        );

        // Line under header
        float lineY = pos.y + ImGui::GetFontSize() + sectionLineOffset;
        draw->AddLine(
            ImVec2(pos.x, lineY),
            ImVec2(pos.x + ImGui::GetContentRegionAvail().x, lineY),
            Theme::BorderDefault()
        );

        ImGui::Dummy(ImVec2(0, ImGui::GetFontSize() + sectionSpacing));

        return collapsed ? !*collapsed : true;
    }

    // =============================================
    // Cards / Sections (legacy API - for backward compatibility)
    // =============================================

    bool FeatureCard(const char* icon, const char* name, const char* description, bool* enabled, std::optional<uint8_t> bgAlpha)
    {
        CardConfig config;
        config.bgAlpha = bgAlpha;
        return FeatureCard(icon, name, description, enabled, config);
    }

    bool SectionHeader(const char* label, bool* collapsed, std::optional<ImU32> accentColor)
    {
        // Note: accentColor is not part of CardConfig, so we handle it directly here
        // This is a legacy API that allows setting accent color directly

        const auto& t = Theme::Card();
        const float sectionLineOffset = t.sectionLineOffset;
        const float sectionSpacing = t.sectionSpacing;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();

        ImU32 effectiveAccentColor = accentColor.value_or(Theme::TextAccent());

        draw->AddText(
            ImVec2(pos.x, pos.y),
            effectiveAccentColor,
            label
        );

        // Line under header
        float lineY = pos.y + ImGui::GetFontSize() + sectionLineOffset;
        draw->AddLine(
            ImVec2(pos.x, lineY),
            ImVec2(pos.x + ImGui::GetContentRegionAvail().x, lineY),
            Theme::BorderDefault()
        );

        ImGui::Dummy(ImVec2(0, ImGui::GetFontSize() + sectionSpacing));

        return collapsed ? !*collapsed : true;
    }
}
