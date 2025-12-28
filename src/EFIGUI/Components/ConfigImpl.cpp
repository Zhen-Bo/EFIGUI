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

        cfg.height = t.baseHeight;
        cfg.iconSize = t.iconSize;
        cfg.iconPadding = t.iconPadding;

        cfg.toggleWidth = t.toggleWidth;
        cfg.toggleHeight = t.toggleHeight;
        cfg.toggleRightMargin = t.toggleRightMargin;

        cfg.bgColor = Theme::BackgroundPanel();

        return cfg;
    }

    // =============================================
    // NavItemConfig::FromTheme
    // =============================================

    NavItemConfig NavItemConfig::FromTheme()
    {
        NavItemConfig cfg;
        const auto& t = Theme::Nav();

        cfg.height = Theme::NavItemHeight();
        cfg.iconPadding = t.iconPadding;
        cfg.labelSpacing = t.labelSpacing;
        cfg.accentBarWidth = t.accentBarWidth;
        cfg.rounding = Theme::NavItemRounding();

        cfg.accentColor = Theme::AccentCyan();
        cfg.collapsedIconScale = t.collapsedIconScale;

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
