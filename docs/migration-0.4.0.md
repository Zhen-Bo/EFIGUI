# Migration Guide: v0.3.x to v0.4.0

This guide helps you migrate from EFIGUI v0.3.x to v0.4.0, which introduces **Core Layout Types**, **Builder Pattern**, and **Enhanced Theme Customization**.

---

## Overview of Changes

v0.4.0 is a **major refactoring release** that introduces unified layout types and fluent builder patterns for easier customization while maintaining backward compatibility.

### What Changed

| Aspect | v0.3.x | v0.4.0 |
|--------|--------|--------|
| Padding/Margin | Individual float fields | `EdgeInsets` type |
| Spacing | Separate x/y values | `Spacing` type |
| Config structs | Direct field assignment | Builder pattern support |
| ButtonConfig | Not available | Full customization struct |
| Theme structs | Basic fields | Enhanced with colors and layout |

### New Core Layout Types

- **`EdgeInsets`** - Unified padding/margin with named constructors
- **`Spacing`** - Element spacing (horizontal and vertical)
- **`CornerRadius`** - Per-corner rounding with presets
- **`BorderStyle`** - Border width and color configuration
- **`SizeConstraints`** - Min/max size constraints

---

## Step-by-Step Migration

### Step 1: No Breaking Changes Required

v0.4.0 maintains full backward compatibility. Existing code will continue to work:

```cpp
// This v0.3.x code still works in v0.4.0
SliderConfig cfg;
cfg.height = 32.0f;
cfg.glowColor = Theme::StatusError();
ModernSliderFloat("Value", &val, 0, 100, "%.0f", std::nullopt, cfg);
```

### Step 2: Adopt Builder Pattern (Optional, Recommended)

The new builder pattern provides a cleaner, more fluent API:

**Before (v0.3.x):**
```cpp
SliderConfig cfg;
cfg.height = 32.0f;
cfg.knobRadius = 12.0f;
cfg.glowColor = Theme::AccentPurple();
ModernSliderFloat("Volume", &volume, 0, 100, "%.0f", std::nullopt, cfg);
```

**After (v0.4.0):**
```cpp
ModernSliderFloat("Volume", &volume, 0, 100, "%.0f", std::nullopt,
    SliderConfig()
        .withHeight(32.0f)
        .withKnobRadius(12.0f)
        .withGlowColor(Theme::AccentPurple()));
```

### Step 3: Use EdgeInsets for Padding/Margin

The new `EdgeInsets` type provides more control over spacing:

**Before (v0.3.x):**
```cpp
// Limited padding control
auto& btn = Theme::ButtonMut();
btn.paddingX = 24.0f;  // Only horizontal padding
```

**After (v0.4.0):**
```cpp
// Full padding control with EdgeInsets
auto& btn = Theme::ButtonMut();
btn.padding = EdgeInsets::Symmetric(8.0f, 24.0f);  // Vertical and horizontal

// Or use named constructors
btn.padding = EdgeInsets::All(16.0f);                    // Same on all sides
btn.padding = EdgeInsets::Only(10, 20, 10, 20);          // Top, Right, Bottom, Left
btn.padding = EdgeInsets::Horizontal(24.0f);             // Left and right only
btn.padding = EdgeInsets::Vertical(8.0f);                // Top and bottom only
```

### Step 4: Use ButtonConfig for Button Customization

v0.4.0 introduces `ButtonConfig` for full button customization:

**Before (v0.3.x):**
```cpp
// Limited customization with positional parameters
GlowButton("OK", ImVec2(120, 36), Theme::StatusSuccess());
```

**After (v0.4.0):**
```cpp
// Full customization with ButtonConfig
GlowButton("OK", ButtonConfig()
    .withSize(120, 36)
    .withGlowColor(Theme::StatusSuccess())
    .withRounding(12.0f)
    .withPadding(EdgeInsets::Symmetric(8, 24)));

// Or using DangerButton with config
DangerButton("Delete", ButtonConfig()
    .withSize(100, 36)
    .withRounding(8.0f));

// ColoredButton with full control
ColoredButton("Custom", Theme::AccentPurple(), ButtonConfig()
    .withSize(150, 40)
    .withGlowLayers(6)
    .withGlowExpand(3.0f));
```

---

## New Features in v0.4.0

### EdgeInsets Named Constructors

```cpp
// All sides equal
EdgeInsets::All(16.0f)                    // {16, 16, 16, 16}

// Symmetric: vertical and horizontal
EdgeInsets::Symmetric(8.0f, 16.0f)        // {8, 16, 8, 16}

// Individual sides
EdgeInsets::Only(10, 20, 10, 20)          // top, right, bottom, left

// Horizontal only (left and right)
EdgeInsets::Horizontal(24.0f)             // {0, 24, 0, 24}

// Vertical only (top and bottom)
EdgeInsets::Vertical(8.0f)                // {8, 0, 8, 0}
```

### EdgeInsets Utility Methods

```cpp
EdgeInsets padding(10, 20, 10, 20);

float h = padding.horizontal();      // left + right = 40
float v = padding.vertical();        // top + bottom = 20
ImVec2 tl = padding.topLeft();       // {20, 10}
ImVec2 br = padding.bottomRight();   // {20, 10}
ImVec2 sz = padding.size();          // {40, 20}
```

### Builder Pattern for All Config Structs

All Config structs now support fluent builder methods:

**SliderConfig:**
```cpp
SliderConfig()
    .withHeight(32.0f)
    .withTrackHeight(8.0f)
    .withKnobRadius(12.0f)
    .withPadding(EdgeInsets::Symmetric(4, 8))
    .withGlowColor(Theme::AccentPurple())
    .withShowInput(false);
```

**ToggleConfig:**
```cpp
ToggleConfig()
    .withWidth(56.0f)
    .withHeight(28.0f)
    .withMargin(EdgeInsets::All(4))
    .withTrackOnColor(Theme::StatusSuccess())
    .withGlowRadius(6.0f);
```

**CardConfig:**
```cpp
CardConfig()
    .withPadding(12.0f)                    // EdgeInsets::All(12)
    .withPadding(8.0f, 16.0f)              // EdgeInsets::Symmetric(8, 16)
    .withMargin(EdgeInsets::Vertical(8))
    .withRounding(12.0f)
    .withBgColor(IM_COL32(30, 30, 50, 200))
    .withBgAlpha(180);
```

**NavItemConfig:**
```cpp
NavItemConfig()
    .withHeight(48.0f)
    .withPadding(EdgeInsets::Symmetric(0, 16))
    .withAccentColor(Theme::AccentPurple())
    .withRounding(12.0f);
```

**ButtonConfig:**
```cpp
ButtonConfig()
    .withSize(150, 40)
    .withPadding(32.0f)                    // EdgeInsets::Horizontal(32)
    .withRounding(12.0f)
    .withMinWidth(100.0f)
    .withGlowColor(Theme::AccentCyan())
    .withGlowLayers(5)
    .withGlowExpand(2.0f)
    .withForceHover(true);
```

**WindowConfig:**
```cpp
WindowConfig()
    .withTitleBarHeight(40.0f)
    .withRounding(16.0f)
    .withPadding(EdgeInsets::All(8))
    .withTitleBarPadding(EdgeInsets::Symmetric(0, 20));
```

**PanelConfig:**
```cpp
PanelConfig()
    .withRounding(12.0f)
    .withPadding(16.0f)
    .withBgAlpha(200)
    .withBorderColor(Theme::BorderDefault())
    .withGlowColor(Theme::AccentCyan());
```

**ProgressConfig:**
```cpp
ProgressConfig()
    .withHeight(8.0f)
    .withRounding(4.0f)
    .withMargin(EdgeInsets::Vertical(4))
    .withFillColor(Theme::AccentCyan())
    .withGlowLayers(4);
```

**InputConfig:**
```cpp
InputConfig()
    .withRounding(8.0f)
    .withPadding(EdgeInsets::Symmetric(6, 12))
    .withBgColor(Theme::ButtonDefault())
    .withFocusBorderColor(Theme::AccentCyan());
```

### Enhanced Theme Structs

Theme structs now include full color and layout customization:

```cpp
// ButtonTheme now includes:
auto& btn = Theme::ButtonMut();
btn.padding = EdgeInsets::Symmetric(0, 32);
btn.rounding = 8.0f;
btn.minWidth = 100.0f;
btn.bgDefault = IM_COL32(40, 40, 60, 200);
btn.bgHover = IM_COL32(50, 50, 80, 220);
btn.bgActive = IM_COL32(0, 180, 200, 255);
btn.textColor = IM_COL32(255, 255, 255, 255);
btn.glowColor = IM_COL32(0, 245, 255, 255);
btn.glowLayers = 5;
btn.glowExpand = 2.0f;

// SliderTheme now includes:
auto& slider = Theme::SliderMut();
slider.padding = EdgeInsets();
slider.labelGap = 12.0f;
slider.trackBgColor = IM_COL32(40, 40, 60, 200);
slider.trackFillStart = IM_COL32(184, 41, 255, 255);
slider.trackFillEnd = IM_COL32(0, 245, 255, 255);

// ToggleTheme now includes:
auto& toggle = Theme::ToggleMut();
toggle.margin = EdgeInsets();
toggle.trackOnColor = IM_COL32(0, 245, 255, 255);
toggle.trackOffColor = IM_COL32(60, 60, 80, 255);
toggle.knobColor = IM_COL32(255, 255, 255, 255);

// CardTheme now includes:
auto& card = Theme::CardMut();
card.padding = EdgeInsets(10, 12, 12, 12);
card.margin = EdgeInsets();
card.rounding = 8.0f;
card.bgColor = IM_COL32(30, 30, 50, 200);
card.borderColor = IM_COL32(60, 60, 80, 255);
card.titleColor = IM_COL32(255, 255, 255, 255);
card.descColor = IM_COL32(180, 180, 200, 255);

// NavTheme now includes:
auto& nav = Theme::NavMut();
nav.itemPadding = EdgeInsets::Symmetric(0, 12);
nav.sectionPadding = EdgeInsets::Symmetric(4, 12);
nav.itemBgHover = IM_COL32(50, 50, 80, 60);
nav.itemBgSelected = IM_COL32(0, 245, 255, 30);
nav.textDefault = IM_COL32(180, 180, 200, 255);

// WindowTheme now includes:
auto& window = Theme::WindowMut();
window.padding = EdgeInsets::All(0);
window.titleBarPadding = EdgeInsets::Symmetric(0, 16);
window.titleBarLeft = IM_COL32(30, 30, 50, 255);
window.titleBarRight = IM_COL32(50, 30, 70, 255);

// LayoutTheme now includes:
auto& layout = Theme::LayoutMut();
layout.itemSpacing = Spacing(8.0f);
layout.innerSpacing = Spacing(4.0f);
layout.tooltipPadding = EdgeInsets::Symmetric(6, 8);
layout.contentPadding = EdgeInsets::All(18);
layout.tooltipBgColor = IM_COL32(30, 30, 50, 240);
```

---

## Deprecated Fields

The following fields are deprecated but still functional for backward compatibility:

| Deprecated | Replacement |
|------------|-------------|
| `ButtonTheme.paddingX` | `ButtonTheme.padding` (use `EdgeInsets`) |
| `CardTheme.topPadding` | `CardTheme.padding.top` |
| `CardTheme.bottomPadding` | `CardTheme.padding.bottom` |
| `CardTheme.baseHeight` | Dynamic calculation (set to 0) |
| `NavTheme.sectionPaddingX` | `NavTheme.sectionPadding.left` |
| `NavTheme.sectionPaddingY` | `NavTheme.sectionPadding.top` |
| `LayoutTheme.tooltipPaddingX` | `LayoutTheme.tooltipPadding.left` |
| `LayoutTheme.tooltipPaddingY` | `LayoutTheme.tooltipPadding.top` |

---

## Complete Migration Example

**Before (v0.3.x):**
```cpp
#include <EFIGUI/EFIGUI.h>

void RenderUI()
{
    EFIGUI::BeginFrame();

    if (EFIGUI::BeginCustomWindow("App", nullptr))
    {
        // Slider with custom config
        EFIGUI::SliderConfig sliderCfg;
        sliderCfg.height = 32.0f;
        sliderCfg.glowColor = EFIGUI::Theme::AccentPurple();

        static float value = 0.5f;
        EFIGUI::ModernSliderFloat("Value", &value, 0, 100, "%.0f", std::nullopt, sliderCfg);

        // Button with positional parameters
        if (EFIGUI::GlowButton("Confirm", ImVec2(120, 36), EFIGUI::Theme::StatusSuccess()))
        {
            // Clicked
        }

        // Card with partial config
        EFIGUI::CardConfig cardCfg;
        cardCfg.topPadding = 12.0f;
        cardCfg.bottomPadding = 12.0f;

        static bool enabled = false;
        EFIGUI::FeatureCard(Icons::Shield, "Shield", "Protection", &enabled, cardCfg);
    }
    EFIGUI::EndCustomWindow();

    EFIGUI::EndFrame();
}
```

**After (v0.4.0):**
```cpp
#include <EFIGUI/EFIGUI.h>

void RenderUI()
{
    EFIGUI::BeginFrame();

    if (EFIGUI::BeginCustomWindow("App", nullptr))
    {
        // Slider with builder pattern
        static float value = 0.5f;
        EFIGUI::ModernSliderFloat("Value", &value, 0, 100, "%.0f", std::nullopt,
            EFIGUI::SliderConfig()
                .withHeight(32.0f)
                .withGlowColor(EFIGUI::Theme::AccentPurple()));

        // Button with ButtonConfig (full customization)
        if (EFIGUI::GlowButton("Confirm", EFIGUI::ButtonConfig()
                .withSize(120, 36)
                .withGlowColor(EFIGUI::Theme::StatusSuccess())))
        {
            // Clicked
        }

        // Card with EdgeInsets padding
        static bool enabled = false;
        EFIGUI::FeatureCard(Icons::Shield, "Shield", "Protection", &enabled,
            EFIGUI::CardConfig()
                .withPadding(EFIGUI::EdgeInsets::Symmetric(12, 12)));
    }
    EFIGUI::EndCustomWindow();

    EFIGUI::EndFrame();
}
```

---

## Troubleshooting

### Compilation Error: "'withHeight' is not a member of 'SliderConfig'"

Make sure you're using v0.4.0 or later. Builder methods were added in this version.

### EdgeInsets values seem wrong

Remember that `EdgeInsets::Symmetric(vertical, horizontal)` takes vertical first, then horizontal. This matches the common CSS shorthand pattern.

### Theme changes not applying to components

If you modify theme structs but changes don't appear:
1. Check you're using the `Mut()` accessor (e.g., `Theme::ButtonMut()`)
2. Modify theme before rendering widgets
3. Remember that per-instance Config overrides Theme defaults

### ButtonConfig not affecting button appearance

Ensure you're using the `GlowButton(label, ButtonConfig&)` overload, not the legacy positional parameter version.

---

## See Also

- [Theme Customization](theme.md) - Full theme API reference
- [Components Reference](components.md) - All component functions
- [Migration from v0.2.x](migration-0.3.0.md) - Previous migration guide
- [CHANGELOG](../CHANGELOG.md) - Complete version history
