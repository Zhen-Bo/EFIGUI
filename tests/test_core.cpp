#include "doctest.h"
#include "EFIGUI/ThemeConfig.h" // EdgeInsets, Spacing, CornerRadius (depends on imgui.h)
#include "EFIGUI/Theme.h"       // Resolve helper (depends on imgui.h)

// Note: These tests verify pure data structures and template utilities.
// No ImGui Context is required - we only need imgui.h for ImVec2 type.

// Tests for ThemeConfig types (EdgeInsets, CornerRadius, etc.)
// Spacing tests are moved to after Components.h is included due to name conflict

TEST_CASE("EdgeInsets Calculation") {
    using namespace EFIGUI;

    SUBCASE("Default Constructor") {
        EdgeInsets p;
        CHECK(p.top == 0.0f);
        CHECK(p.right == 0.0f);
        CHECK(p.bottom == 0.0f);
        CHECK(p.left == 0.0f);
    }

    SUBCASE("All Constructor") {
        EdgeInsets p(15.0f);
        CHECK(p.top == 15.0f);
        CHECK(p.right == 15.0f);
        CHECK(p.bottom == 15.0f);
        CHECK(p.left == 15.0f);
    }

    SUBCASE("Symmetric Constructor") {
        EdgeInsets p = EdgeInsets::Symmetric(10.0f, 20.0f);
        CHECK(p.top == 10.0f);
        CHECK(p.bottom == 10.0f);
        CHECK(p.left == 20.0f);
        CHECK(p.right == 20.0f);
    }

    SUBCASE("Only Constructor") {
        EdgeInsets p = EdgeInsets::Only(1.0f, 2.0f, 3.0f, 4.0f);
        CHECK(p.top == 1.0f);
        CHECK(p.right == 2.0f);
        CHECK(p.bottom == 3.0f);
        CHECK(p.left == 4.0f);
    }

    SUBCASE("Horizontal and Vertical Named Constructors") {
        EdgeInsets h = EdgeInsets::Horizontal(10.0f);
        CHECK(h.left == 10.0f);
        CHECK(h.right == 10.0f);
        CHECK(h.top == 0.0f);
        CHECK(h.bottom == 0.0f);

        EdgeInsets v = EdgeInsets::Vertical(10.0f);
        CHECK(v.top == 10.0f);
        CHECK(v.bottom == 10.0f);
        CHECK(v.left == 0.0f);
        CHECK(v.right == 0.0f);
    }

    SUBCASE("Utility Methods") {
        EdgeInsets p = EdgeInsets::Only(10.0f, 20.0f, 30.0f, 40.0f);
        CHECK(p.horizontal() == 60.0f);  // left + right = 40 + 20
        CHECK(p.vertical() == 40.0f);    // top + bottom = 10 + 30
    }

    SUBCASE("Vector Conversion") {
        EdgeInsets p = EdgeInsets::All(10.0f);
        ImVec2 tl = p.topLeft();
        ImVec2 br = p.bottomRight();
        CHECK(tl.x == 10.0f);
        CHECK(tl.y == 10.0f);
        CHECK(br.x == 10.0f);
        CHECK(br.y == 10.0f);
    }
}

TEST_CASE("Spacing Calculation") {
    // Note: Each SUBCASE needs its own typedef to avoid scope issues with doctest

    SUBCASE("Default Constructor") {
        typedef EFIGUI::Spacing SpacingType;
        SpacingType spacing1;
        CHECK(spacing1.x == 0.0f);
        CHECK(spacing1.y == 0.0f);
    }

    SUBCASE("Both Constructor") {
        typedef EFIGUI::Spacing SpacingType;
        SpacingType spacing2 = SpacingType::Both(8.0f);
        CHECK(spacing2.x == 8.0f);
        CHECK(spacing2.y == 8.0f);
    }

    SUBCASE("ToVec2 Conversion") {
        typedef EFIGUI::Spacing SpacingType;
        SpacingType spacing3(12.0f, 16.0f);
        ImVec2 v = spacing3.toVec2();
        CHECK(v.x == 12.0f);
        CHECK(v.y == 16.0f);
    }
}

TEST_CASE("CornerRadius Calculation") {
    using namespace EFIGUI;

    SUBCASE("All Constructor") {
        CornerRadius r = CornerRadius::All(8.0f);
        CHECK(r.topLeft == 8.0f);
        CHECK(r.topRight == 8.0f);
        CHECK(r.bottomLeft == 8.0f);
        CHECK(r.bottomRight == 8.0f);
        CHECK(r.isUniform() == true);
    }

    SUBCASE("Top and Bottom Constructor") {
        CornerRadius r = CornerRadius::Top(10.0f);
        CHECK(r.topLeft == 10.0f);
        CHECK(r.topRight == 10.0f);
        CHECK(r.bottomLeft == 0.0f);
        CHECK(r.bottomRight == 0.0f);
    }

    SUBCASE("Pill Style") {
        CornerRadius r = CornerRadius::Pill();
        CHECK(r.uniform() == 999.0f);
    }

    SUBCASE("Non-uniform Detection") {
        CornerRadius r(10.0f, 5.0f, 10.0f, 5.0f);
        CHECK(r.isUniform() == false);
    }
}

TEST_CASE("Resolve Helper") {
    using namespace EFIGUI;

    SUBCASE("Optional with Value") {
        std::optional<float> optValue = 50.0f;
        float defaultVal = 10.0f;
        CHECK(Resolve(optValue, defaultVal) == 50.0f);
    }

    SUBCASE("Optional without Value (nullopt)") {
        std::optional<float> optNull = std::nullopt;
        float defaultVal = 10.0f;
        CHECK(Resolve(optNull, defaultVal) == 10.0f);
    }

    SUBCASE("Non-optional Value") {
        float value = 25.0f;
        float defaultVal = 10.0f;
        CHECK(Resolve(value, defaultVal) == 25.0f);
    }

    SUBCASE("Optional Integer") {
        std::optional<int> optInt = 42;
        int defaultInt = 0;
        CHECK(Resolve(optInt, defaultInt) == 42);
    }
}

TEST_CASE("BorderStyle") {
    using namespace EFIGUI;

    SUBCASE("None Factory") {
        BorderStyle b = BorderStyle::None();
        CHECK(b.width == 0.0f);
        CHECK(b.color == IM_COL32(0, 0, 0, 0));
    }

    SUBCASE("Custom Constructor") {
        BorderStyle b(2.0f, IM_COL32(255, 0, 0, 255));
        CHECK(b.width == 2.0f);
        CHECK(b.color == IM_COL32(255, 0, 0, 255));
    }
}

TEST_CASE("SizeConstraints") {
    using namespace EFIGUI;

    SUBCASE("Fixed Size") {
        SizeConstraints c = SizeConstraints::Fixed(100.0f, 50.0f);
        CHECK(c.minWidth == 100.0f);
        CHECK(c.maxWidth == 100.0f);
        CHECK(c.minHeight == 50.0f);
        CHECK(c.maxHeight == 50.0f);
    }

    SUBCASE("Min Size Only") {
        SizeConstraints c = SizeConstraints::MinSize(200.0f, 100.0f);
        CHECK(c.minWidth == 200.0f);
        CHECK(c.maxWidth == 0.0f);
        CHECK(c.minHeight == 100.0f);
        CHECK(c.maxHeight == 0.0f);
    }
}

// =============================================
// Config Builder Pattern Tests
// =============================================

// Include Components.h here to avoid Spacing name conflict
// (EFIGUI::Spacing struct vs EFIGUI::Spacing() function)
#include "EFIGUI/Components.h"

TEST_CASE("SliderConfig Builder Pattern") {
    using namespace EFIGUI;

    SUBCASE("Default Construction") {
        SliderConfig cfg;
        CHECK(cfg.height.has_value() == false);
        CHECK(cfg.trackHeight.has_value() == false);
        CHECK(cfg.glowColor.has_value() == false);
        CHECK(cfg.showInput == true);  // Default value
    }

    SUBCASE("Single Builder Method") {
        SliderConfig cfg;
        cfg.withHeight(32.0f);
        CHECK(cfg.height.has_value() == true);
        CHECK(cfg.height.value() == 32.0f);
    }

    SUBCASE("Chained Builder Methods") {
        SliderConfig cfg = SliderConfig()
            .withHeight(40.0f)
            .withTrackHeight(8.0f)
            .withKnobRadius(12.0f)
            .withGlowColor(IM_COL32(255, 0, 0, 255))
            .withShowInput(false);

        CHECK(cfg.height.value() == 40.0f);
        CHECK(cfg.trackHeight.value() == 8.0f);
        CHECK(cfg.knobRadius.value() == 12.0f);
        CHECK(cfg.glowColor.value() == IM_COL32(255, 0, 0, 255));
        CHECK(cfg.showInput == false);
    }

    SUBCASE("Padding Builder with EdgeInsets") {
        SliderConfig cfg = SliderConfig()
            .withPadding(EdgeInsets::Symmetric(4.0f, 8.0f));

        CHECK(cfg.padding.has_value() == true);
        CHECK(cfg.padding.value().top == 4.0f);
        CHECK(cfg.padding.value().left == 8.0f);
    }

    SUBCASE("Padding Builder with float") {
        SliderConfig cfg = SliderConfig()
            .withPadding(10.0f);

        CHECK(cfg.padding.has_value() == true);
        CHECK(cfg.padding.value().top == 10.0f);
        CHECK(cfg.padding.value().right == 10.0f);
        CHECK(cfg.padding.value().bottom == 10.0f);
        CHECK(cfg.padding.value().left == 10.0f);
    }
}

TEST_CASE("ButtonConfig Builder Pattern") {
    using namespace EFIGUI;

    SUBCASE("Default Construction") {
        ButtonConfig cfg;
        CHECK(cfg.size.x == 0.0f);
        CHECK(cfg.size.y == 0.0f);
        CHECK(cfg.forceHover == false);
        CHECK(cfg.glowColor.has_value() == false);
    }

    SUBCASE("Size Builder with ImVec2") {
        ButtonConfig cfg = ButtonConfig()
            .withSize(ImVec2(120.0f, 36.0f));

        CHECK(cfg.size.x == 120.0f);
        CHECK(cfg.size.y == 36.0f);
    }

    SUBCASE("Size Builder with width and height") {
        ButtonConfig cfg = ButtonConfig()
            .withSize(150.0f, 40.0f);

        CHECK(cfg.size.x == 150.0f);
        CHECK(cfg.size.y == 40.0f);
    }

    SUBCASE("Full Chained Configuration") {
        ButtonConfig cfg = ButtonConfig()
            .withSize(200.0f, 50.0f)
            .withRounding(12.0f)
            .withMinWidth(100.0f)
            .withGlowColor(IM_COL32(0, 255, 0, 255))
            .withGlowLayers(6)
            .withGlowExpand(3.0f)
            .withForceHover(true);

        CHECK(cfg.size.x == 200.0f);
        CHECK(cfg.size.y == 50.0f);
        CHECK(cfg.rounding.value() == 12.0f);
        CHECK(cfg.minWidth.value() == 100.0f);
        CHECK(cfg.glowColor.value() == IM_COL32(0, 255, 0, 255));
        CHECK(cfg.glowLayers.value() == 6);
        CHECK(cfg.glowExpand.value() == 3.0f);
        CHECK(cfg.forceHover == true);
    }

    SUBCASE("Padding Builder Overloads") {
        // Float overload (horizontal padding)
        ButtonConfig cfg1 = ButtonConfig().withPadding(24.0f);
        CHECK(cfg1.padding.has_value() == true);
        CHECK(cfg1.padding.value().left == 24.0f);
        CHECK(cfg1.padding.value().right == 24.0f);

        // EdgeInsets overload
        ButtonConfig cfg2 = ButtonConfig().withPadding(EdgeInsets::Symmetric(8.0f, 16.0f));
        CHECK(cfg2.padding.value().top == 8.0f);
        CHECK(cfg2.padding.value().left == 16.0f);
    }
}

TEST_CASE("CardConfig Builder Pattern") {
    using namespace EFIGUI;

    SUBCASE("Padding Builder Overloads") {
        // Single float
        CardConfig cfg1 = CardConfig().withPadding(12.0f);
        CHECK(cfg1.padding.value().top == 12.0f);
        CHECK(cfg1.padding.value().left == 12.0f);

        // Two floats (vertical, horizontal)
        CardConfig cfg2 = CardConfig().withPadding(8.0f, 16.0f);
        CHECK(cfg2.padding.value().top == 8.0f);
        CHECK(cfg2.padding.value().left == 16.0f);

        // EdgeInsets
        CardConfig cfg3 = CardConfig().withPadding(EdgeInsets::Only(1.0f, 2.0f, 3.0f, 4.0f));
        CHECK(cfg3.padding.value().top == 1.0f);
        CHECK(cfg3.padding.value().right == 2.0f);
        CHECK(cfg3.padding.value().bottom == 3.0f);
        CHECK(cfg3.padding.value().left == 4.0f);
    }

    SUBCASE("Chained Configuration") {
        CardConfig cfg = CardConfig()
            .withPadding(10.0f)
            .withMargin(EdgeInsets::Vertical(8.0f))
            .withRounding(8.0f)
            .withBgColor(IM_COL32(30, 30, 50, 200))
            .withBgAlpha(180);

        CHECK(cfg.padding.value().top == 10.0f);
        CHECK(cfg.margin.value().top == 8.0f);
        CHECK(cfg.margin.value().left == 0.0f);
        CHECK(cfg.rounding.value() == 8.0f);
        CHECK(cfg.bgColor.value() == IM_COL32(30, 30, 50, 200));
        CHECK(cfg.bgAlpha.value() == 180);
    }
}

TEST_CASE("ToggleConfig Builder Pattern") {
    using namespace EFIGUI;

    SUBCASE("Default Values") {
        ToggleConfig cfg;
        CHECK(cfg.width.has_value() == false);
        CHECK(cfg.height.has_value() == false);
    }

    SUBCASE("Chained Configuration") {
        ToggleConfig cfg = ToggleConfig()
            .withWidth(56.0f)
            .withHeight(28.0f)
            .withKnobSize(20.0f)
            .withMargin(EdgeInsets::All(4.0f))
            .withAnimSpeed(0.15f);

        CHECK(cfg.width.value() == 56.0f);
        CHECK(cfg.height.value() == 28.0f);
        CHECK(cfg.knobSize.value() == 20.0f);
        CHECK(cfg.margin.value().top == 4.0f);
        CHECK(cfg.animSpeed.value() == 0.15f);
    }
}

TEST_CASE("NavItemConfig Builder Pattern") {
    using namespace EFIGUI;

    SUBCASE("Chained Configuration") {
        NavItemConfig cfg = NavItemConfig()
            .withHeight(48.0f)
            .withPadding(EdgeInsets::Symmetric(0.0f, 16.0f))
            .withRounding(12.0f)
            .withAccentColor(IM_COL32(255, 0, 255, 255));

        CHECK(cfg.height.value() == 48.0f);
        CHECK(cfg.padding.value().left == 16.0f);
        CHECK(cfg.padding.value().top == 0.0f);
        CHECK(cfg.rounding.value() == 12.0f);
        CHECK(cfg.accentColor.value() == IM_COL32(255, 0, 255, 255));
    }
}

TEST_CASE("WindowConfig Builder Pattern") {
    using namespace EFIGUI;

    SUBCASE("Chained Configuration") {
        WindowConfig cfg = WindowConfig()
            .withTitleBarHeight(40.0f)
            .withRounding(16.0f)
            .withPadding(EdgeInsets::All(8.0f))
            .withTitleBarPadding(EdgeInsets::Symmetric(0.0f, 20.0f));

        CHECK(cfg.titleBarHeight.value() == 40.0f);
        CHECK(cfg.rounding.value() == 16.0f);
        CHECK(cfg.padding.value().top == 8.0f);
        CHECK(cfg.titleBarPadding.value().left == 20.0f);
        CHECK(cfg.titleBarPadding.value().top == 0.0f);
    }
}

TEST_CASE("PanelConfig Builder Pattern") {
    using namespace EFIGUI;

    SUBCASE("Chained Configuration") {
        PanelConfig cfg = PanelConfig()
            .withRounding(12.0f)
            .withPadding(16.0f)
            .withBgAlpha(200)
            .withBorderColor(IM_COL32(100, 100, 100, 255))
            .withGlowColor(IM_COL32(0, 245, 255, 255));

        CHECK(cfg.rounding.value() == 12.0f);
        CHECK(cfg.padding.value().top == 16.0f);
        CHECK(cfg.bgAlpha.value() == 200);
        CHECK(cfg.borderColor.value() == IM_COL32(100, 100, 100, 255));
        CHECK(cfg.glowColor.value() == IM_COL32(0, 245, 255, 255));
    }
}

TEST_CASE("ProgressConfig Builder Pattern") {
    using namespace EFIGUI;

    SUBCASE("Chained Configuration") {
        ProgressConfig cfg = ProgressConfig()
            .withHeight(8.0f)
            .withRounding(4.0f)
            .withMargin(EdgeInsets::Vertical(4.0f))
            .withFillColor(IM_COL32(0, 200, 255, 255))
            .withGlowLayers(4);

        CHECK(cfg.height.value() == 8.0f);
        CHECK(cfg.rounding.value() == 4.0f);
        CHECK(cfg.margin.value().top == 4.0f);
        CHECK(cfg.margin.value().left == 0.0f);
        CHECK(cfg.fillColor.value() == IM_COL32(0, 200, 255, 255));
        CHECK(cfg.glowLayers.value() == 4);
    }
}

TEST_CASE("InputConfig Builder Pattern") {
    using namespace EFIGUI;

    SUBCASE("Chained Configuration") {
        InputConfig cfg = InputConfig()
            .withRounding(8.0f)
            .withPadding(EdgeInsets::Symmetric(6.0f, 12.0f))
            .withBgColor(IM_COL32(40, 40, 60, 200))
            .withFocusBorderColor(IM_COL32(0, 245, 255, 255));

        CHECK(cfg.rounding.value() == 8.0f);
        CHECK(cfg.padding.value().top == 6.0f);
        CHECK(cfg.padding.value().left == 12.0f);
        CHECK(cfg.bgColor.value() == IM_COL32(40, 40, 60, 200));
        CHECK(cfg.focusBorderColor.value() == IM_COL32(0, 245, 255, 255));
    }
}
