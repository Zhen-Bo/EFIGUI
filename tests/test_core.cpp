#include "doctest.h"
#include "EFIGUI/ThemeConfig.h" // EdgeInsets, Spacing, CornerRadius (depends on imgui.h)
#include "EFIGUI/Theme.h"       // Resolve helper (depends on imgui.h)

// Note: These tests verify pure data structures and template utilities.
// No ImGui Context is required - we only need imgui.h for ImVec2 type.

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
    using namespace EFIGUI;

    SUBCASE("Default Constructor") {
        Spacing s;
        CHECK(s.x == 0.0f);
        CHECK(s.y == 0.0f);
    }

    SUBCASE("Both Constructor") {
        Spacing s = Spacing::Both(8.0f);
        CHECK(s.x == 8.0f);
        CHECK(s.y == 8.0f);
    }

    SUBCASE("ToVec2 Conversion") {
        Spacing s(12.0f, 16.0f);
        ImVec2 v = s.toVec2();
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
