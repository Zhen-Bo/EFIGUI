#include "../Components.h"
#include "../Theme.h"

namespace EFIGUI
{
    // =============================================
    // SliderConfig::FromTheme
    // =============================================

    SliderConfig SliderConfig::FromTheme()
    {
        SliderConfig cfg;
        const auto& t = Theme::Slider();

        cfg.height = t.height;
        cfg.trackHeight = t.trackHeight;
        cfg.knobRadius = t.knobRadius;
        cfg.inputWidth = t.inputWidth;
        cfg.inputHeight = t.inputHeight;
        cfg.inputGap = t.inputGap;
        cfg.inputRounding = t.inputRounding;

        cfg.glowLayers = t.glowLayers;
        cfg.knobGlowExpand = t.knobGlowExpand;
        cfg.inputGlowExpand = t.inputGlowExpand;

        // Colors use Theme defaults (AccentCyan, etc.)
        cfg.glowColor = Theme::AccentCyan();

        return cfg;
    }

    // =============================================
    // ToggleConfig::FromTheme
    // =============================================

    ToggleConfig ToggleConfig::FromTheme()
    {
        ToggleConfig cfg;
        const auto& t = Theme::Toggle();

        cfg.width = t.width;
        cfg.height = t.height;
        cfg.knobSize = t.knobSize;
        cfg.labelGap = t.labelGap;

        cfg.animSpeed = t.animSpeed;
        cfg.glowRadius = t.glowRadius;

        // Colors use Theme defaults
        cfg.trackOnColor = Theme::AccentCyan();
        cfg.knobColor = Theme::TextPrimary();
        cfg.glowColor = Theme::AccentCyan();

        return cfg;
    }

    // =============================================
    // CardConfig::FromTheme
    // =============================================

    CardConfig CardConfig::FromTheme()
    {
        CardConfig cfg;
        const auto& t = Theme::Card();

        // Layout with new padding
        cfg.padding = t.padding;
        cfg.margin = t.margin;
        cfg.rounding = t.rounding;
        cfg.titleDescGap = t.titleDescGap;

        // Layout dimensions
        cfg.height = t.baseHeight;
        cfg.iconSize = t.iconSize;
        cfg.iconPadding = t.iconPadding;
        cfg.iconTextOffset = t.iconTextOffset;
        cfg.minTextWidth = t.minTextWidth;
        cfg.nameOffsetY = t.nameOffsetY;
        cfg.descOffsetY = t.descOffsetY;

        // Legacy
        cfg.topPadding = t.topPadding;
        cfg.bottomPadding = t.bottomPadding;

        // Inline toggle dimensions
        cfg.toggleWidth = t.toggleWidth;
        cfg.toggleHeight = t.toggleHeight;
        cfg.toggleRounding = t.toggleRounding;
        cfg.toggleRightMargin = t.toggleRightMargin;
        cfg.knobRadius = t.knobRadius;
        cfg.knobPadding = t.knobPadding;
        cfg.knobTravel = t.knobTravel;
        cfg.toggleAnimSpeed = t.toggleAnimSpeed;

        // Section header
        cfg.sectionLineOffset = t.sectionLineOffset;
        cfg.sectionSpacing = t.sectionSpacing;

        // Colors
        cfg.bgColor = t.bgColor;
        cfg.borderColor = t.borderColor;
        cfg.titleColor = t.titleColor;
        cfg.descColor = t.descColor;

        return cfg;
    }

    // =============================================
    // NavItemConfig::FromTheme
    // =============================================

    NavItemConfig NavItemConfig::FromTheme()
    {
        NavItemConfig cfg;
        const auto& t = Theme::Nav();

        // Dimensions
        cfg.height = t.itemHeight;
        cfg.iconPadding = t.iconPadding;
        cfg.labelSpacing = t.labelSpacing;
        cfg.accentBarWidth = t.accentBarWidth;
        cfg.accentBarHeightRatio = t.accentBarHeightRatio;
        cfg.accentBarGlowRadius = t.accentBarGlowRadius;
        cfg.rounding = t.itemRounding;
        cfg.collapsedIconScale = t.collapsedIconScale;

        // Padding
        cfg.padding = t.itemPadding;
        cfg.sectionPadding = t.sectionPadding;
        cfg.sectionPaddingX = t.sectionPaddingX;
        cfg.sectionPaddingY = t.sectionPaddingY;
        cfg.sectionHeight = t.sectionHeight;

        // Colors
        cfg.accentColor = Theme::AccentCyan();

        // Alpha values
        cfg.accentGlowAlpha = t.accentGlowAlpha;
        cfg.bgAlphaMultiplier = t.bgAlphaMultiplier;
        cfg.hoverBgAlpha = t.hoverBgAlpha;

        return cfg;
    }

    // =============================================
    // ButtonConfig::FromTheme
    // =============================================

    ButtonConfig ButtonConfig::FromTheme()
    {
        ButtonConfig cfg;
        const auto& t = Theme::Button();

        // Layout
        cfg.padding = t.padding;
        cfg.rounding = t.rounding;
        cfg.minWidth = t.minWidth;

        // Colors
        cfg.bgColor = t.bgDefault;
        cfg.bgHoverColor = t.bgHover;
        cfg.bgActiveColor = t.bgActive;
        cfg.textColor = t.textColor;
        cfg.glowColor = t.glowColor;

        // Effects
        cfg.glowLayers = t.glowLayers;
        cfg.glowExpand = t.glowExpand;
        cfg.glowIntensity = t.glowIntensityBase;

        return cfg;
    }

    // =============================================
    // WindowConfig::FromTheme
    // =============================================

    WindowConfig WindowConfig::FromTheme()
    {
        WindowConfig cfg;
        const auto& t = Theme::Window();

        cfg.titleBarHeight = t.titleBarHeight;
        cfg.titleTextPadding = t.titleTextPadding;
        cfg.titleButtonSize = t.titleButtonSize;
        cfg.rounding = Theme::WindowRounding();

        cfg.titleBarLeftColor = Theme::TitleBarLeft();
        cfg.titleBarRightColor = Theme::TitleBarRight();
        cfg.overlayAlpha = Theme::DefaultOverlayAlpha();

        return cfg;
    }

    // =============================================
    // PanelConfig::FromTheme
    // =============================================

    PanelConfig PanelConfig::FromTheme()
    {
        PanelConfig cfg;

        cfg.rounding = Theme::FrameRounding();
        cfg.bgAlpha = Theme::DefaultBgAlpha();
        cfg.borderColor = Theme::BorderDefault();
        cfg.glowColor = Theme::AccentCyan();

        return cfg;
    }

    // =============================================
    // ProgressConfig::FromTheme
    // =============================================

    ProgressConfig ProgressConfig::FromTheme()
    {
        ProgressConfig cfg;

        cfg.rounding = Theme::FrameRounding();
        cfg.trackColor = Theme::ButtonDefault();
        cfg.fillColor = Theme::AccentCyan();
        cfg.glowColor = Theme::AccentCyan();
        cfg.glowLayers = Theme::ProgressGlowLayers();

        return cfg;
    }

    // =============================================
    // InputConfig::FromTheme
    // =============================================

    InputConfig InputConfig::FromTheme()
    {
        InputConfig cfg;

        cfg.rounding = Theme::FrameRounding();
        cfg.bgColor = Theme::ButtonDefault();
        cfg.borderColor = Theme::BorderDefault();
        cfg.focusBorderColor = Theme::AccentCyan();
        cfg.textColor = Theme::TextPrimary();

        return cfg;
    }
}
