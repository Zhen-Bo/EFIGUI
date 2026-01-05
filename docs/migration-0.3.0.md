# Migration Guide: v0.2.x to v0.3.0

This guide helps you migrate from EFIGUI v0.2.x to v0.3.0, which introduces the **Runtime Theme Customization System**.

---

## Overview of Changes

v0.3.0 is a **breaking change** release that transforms the theme system from compile-time `constexpr` constants to runtime-configurable settings.

### What Changed

| Aspect | v0.2.x | v0.3.0 |
|--------|--------|--------|
| Theme values | `constexpr` constants | Runtime functions |
| Customization | Limited (`SetAccentColor()` only) | Full runtime control |
| Access pattern | `Theme::AccentCyan` | `Theme::AccentCyan()` |
| Configuration | None | `ThemeConfig` struct |

---

## Step-by-Step Migration

### Step 1: Update Theme Value Access

All theme values are now accessed via function calls instead of direct constants.

**Before (v0.2.x):**
```cpp
ImU32 color = EFIGUI::Theme::AccentCyan;
float rounding = EFIGUI::Theme::WindowRounding;
float navWidth = EFIGUI::Theme::NavbarWidth;
```

**After (v0.3.0):**
```cpp
ImU32 color = EFIGUI::Theme::AccentCyan();
float rounding = EFIGUI::Theme::WindowRounding();
float navWidth = EFIGUI::Theme::NavbarWidth();
```

### Step 2: Update Component Default Parameters

Some component functions that previously used `constexpr` defaults now use sentinel values (0 = use theme default).

**NavItem:**
```cpp
// Before (v0.2.x)
NavItem(icon, label, selected, Theme::NavbarWidth);

// After (v0.3.0) - Both are valid:
NavItem(icon, label, selected);        // Omit parameter (recommended)
NavItem(icon, label, selected, 0);     // 0 = use theme default
NavItem(icon, label, selected, 180);   // Custom width
```

**Spacing:**
```cpp
// Before (v0.2.x)
Spacing(Theme::ItemSpacing);

// After (v0.3.0) - Both are valid:
Spacing();     // Omit parameter (recommended)
Spacing(0);    // 0 = use theme default
Spacing(20);   // Custom spacing
```

### Step 3: Update Color Utility Calls

Color utilities now use function-style access:

**Before (v0.2.x):**
```cpp
auto rgb = Theme::ExtractRGB(Theme::AccentCyan);
ImU32 color = Theme::ApplyAlpha(Theme::AccentPurple, 128);
```

**After (v0.3.0):**
```cpp
auto rgb = Theme::ExtractRGB(Theme::AccentCyan());
ImU32 color = Theme::ApplyAlpha(Theme::AccentPurple(), 128);
```

---

## New Features in v0.3.0

### Config Struct API (Recommended)

v0.3.0 introduces config struct versions for components with many parameters. This provides cleaner, more maintainable code compared to the legacy positional parameter API.

**GlowButton:**
```cpp
// Before (v0.2.x) - positional parameters
EFIGUI::GlowButton("Confirm", ImVec2(120, 36), Theme::StatusSuccess(), false, std::nullopt, 180);

// After (v0.3.0) - config struct (recommended)
EFIGUI::GlowButtonConfig config;
config.size = ImVec2(120, 36);
config.glowColor = Theme::StatusSuccess();
config.bgAlpha = 180;
EFIGUI::GlowButton("Confirm", config);
```

**CooldownButton:**
```cpp
// Before (v0.2.x) - positional parameters
static float cooldown = 0.0f;
EFIGUI::CooldownButton("Fire", ImVec2(100, 36), Theme::AccentPurple(), cooldown, std::nullopt, 200);

// After (v0.3.0) - config struct (recommended)
EFIGUI::CooldownButtonConfig config;
config.size = ImVec2(100, 36);
config.glowColor = Theme::AccentPurple();
config.cooldownProgress = cooldown;
config.bgAlpha = 200;
EFIGUI::CooldownButton("Fire", config);
```

**NavbarHeader:**
```cpp
// Before (v0.2.x) - positional parameters
EFIGUI::NavbarHeader("App", Icons::Bars, Icons::Cog, collapsed, width, nullptr, Theme::AccentPurple());

// After (v0.3.0) - config struct (recommended)
EFIGUI::NavbarHeaderConfig config;
config.iconExpanded = Icons::Bars;
config.iconCollapsed = Icons::Cog;
config.collapsed = isCollapsed;
config.width = navbarWidth;
config.glowColor = Theme::AccentPurple();
EFIGUI::NavbarHeader("App", config);
```

> **Note:** The legacy positional parameter APIs remain available for backward compatibility.

### Runtime Theme Configuration

You can now customize any theme value at runtime:

```cpp
// Access the mutable configuration
auto& config = EFIGUI::Theme::GetConfig();

// Modify colors
config.colors.accentPrimary = IM_COL32(255, 100, 50, 255);  // Custom orange
config.colors.accentSecondary = IM_COL32(255, 50, 150, 255); // Custom pink

// Modify dimensions
config.dimensions.windowRounding = 16.0f;   // More rounded
config.dimensions.frameRounding = 12.0f;
config.dimensions.navbarWidth = 220.0f;     // Wider navbar

// Modify effects
config.effects.glowIntensity = 0.8f;        // Stronger glow
config.effects.glassBlurRadius = 12.0f;

// Modify animation
config.animation.hoverSpeed = 15.0f;        // Faster hover transitions
config.animation.selectedSpeed = 10.0f;
```

### Theme Presets

Load predefined theme presets:

```cpp
// Load preset (currently only Cyberpunk is available)
EFIGUI::Theme::LoadPreset(EFIGUI::ThemePreset::Cyberpunk);

// Reset to default
EFIGUI::Theme::ResetToDefault();
```

---

## Configuration Structure Reference

### ThemeConfig

The root configuration structure containing all sub-configurations:

```cpp
struct ThemeConfig
{
    ThemeColors colors;         // Color palette
    ThemeDimensions dimensions; // Sizes and rounding
    ThemeEffects effects;       // Visual effects
    ThemeAnimation animation;   // Animation timing
};
```

### ThemeColors

All color values in the theme:

| Field | Description | Default |
|-------|-------------|---------|
| `accentPrimary` | Primary accent (cyan) | `IM_COL32(0, 255, 255, 255)` |
| `accentSecondary` | Secondary accent (purple) | `IM_COL32(128, 0, 255, 255)` |
| `accentPink` | Pink accent | `IM_COL32(255, 0, 128, 255)` |
| `accentGreen` | Green accent | `IM_COL32(0, 255, 128, 255)` |
| `backgroundDark` | Main background | `IM_COL32(10, 10, 15, 255)` |
| `backgroundPanel` | Panel background | `IM_COL32(20, 20, 30, 200)` |
| `backgroundNavbar` | Navbar background | `IM_COL32(15, 15, 25, 240)` |
| `backgroundContent` | Content area | `IM_COL32(25, 25, 40, 180)` |
| `textPrimary` | Primary text | `IM_COL32(255, 255, 255, 255)` |
| `textSecondary` | Secondary text | `IM_COL32(180, 180, 200, 255)` |
| `textMuted` | Muted text | `IM_COL32(100, 100, 120, 255)` |
| `textAccent` | Accent text | `IM_COL32(0, 255, 255, 255)` |
| `statusSuccess` | Success indicator | `IM_COL32(0, 255, 128, 255)` |
| `statusWarning` | Warning indicator | `IM_COL32(255, 200, 0, 255)` |
| `statusError` | Error indicator | `IM_COL32(255, 60, 60, 255)` |
| `statusInfo` | Info indicator | `IM_COL32(0, 200, 255, 255)` |

### ThemeDimensions

All size and rounding values:

| Field | Description | Default |
|-------|-------------|---------|
| `windowRounding` | Window corner rounding | `12.0f` |
| `frameRounding` | Frame corner rounding | `8.0f` |
| `navItemRounding` | Nav item rounding | `6.0f` |
| `buttonRounding` | Button rounding | `8.0f` |
| `navbarWidth` | Sidebar width | `200.0f` |
| `navbarWidthCollapsed` | Collapsed sidebar width | `60.0f` |
| `titleBarHeight` | Title bar height | `32.0f` |
| `navItemHeight` | Nav item height | `36.0f` |
| `itemSpacing` | Default item spacing | `8.0f` |

### ThemeEffects

Visual effect parameters:

| Field | Description | Default |
|-------|-------------|---------|
| `glowIntensity` | Glow effect intensity | `0.6f` |
| `glowRadius` | Glow blur radius | `8.0f` |
| `glassBlurRadius` | Glassmorphism blur | `8.0f` |
| `glassTint` | Glass tint color | `IM_COL32(0, 255, 255, 20)` |
| `marqueeBorderWidth` | Marquee border width | `2.0f` |
| `marqueeSpeed` | Marquee animation speed | `1.5f` |

### ThemeAnimation

Animation timing parameters:

| Field | Description | Default |
|-------|-------------|---------|
| `hoverSpeed` | Hover transition speed | `12.0f` |
| `activeSpeed` | Active transition speed | `15.0f` |
| `selectedSpeed` | Selected transition speed | `10.0f` |
| `pulseFrequency` | Pulse effect frequency | `2.0f` |
| `sweepDuration` | Sweep effect duration | `0.8f` |
| `breatheFrequency` | Breathe effect frequency | `1.0f` |

---

## Complete Migration Example

**Before (v0.2.x):**
```cpp
#include <EFIGUI/EFIGUI.h>

void RenderUI()
{
    EFIGUI::BeginFrame();

    if (EFIGUI::BeginCustomWindow("App", nullptr))
    {
        // Navigation with explicit width
        if (EFIGUI::NavItem(EFIGUI::Icons::Home, "Home", selected == 0,
                            EFIGUI::Theme::NavbarWidth))
            selected = 0;

        // Using theme colors directly
        ImU32 accent = EFIGUI::Theme::AccentCyan;
        ImU32 text = EFIGUI::Theme::TextPrimary;

        // Spacing with theme value
        EFIGUI::Spacing(EFIGUI::Theme::ItemSpacing);

        // Color utilities
        auto rgb = EFIGUI::Theme::ExtractRGB(EFIGUI::Theme::AccentPurple);
    }
    EFIGUI::EndCustomWindow();

    EFIGUI::EndFrame();
}
```

**After (v0.3.0):**
```cpp
#include <EFIGUI/EFIGUI.h>

void RenderUI()
{
    EFIGUI::BeginFrame();

    if (EFIGUI::BeginCustomWindow("App", nullptr))
    {
        // Navigation - omit width for default, or use 0
        if (EFIGUI::NavItem(EFIGUI::Icons::Home, "Home", selected == 0))
            selected = 0;

        // Using theme colors via function calls
        ImU32 accent = EFIGUI::Theme::AccentCyan();
        ImU32 text = EFIGUI::Theme::TextPrimary();

        // Spacing - omit parameter for default, or use 0
        EFIGUI::Spacing();

        // Color utilities with function-style access
        auto rgb = EFIGUI::Theme::ExtractRGB(EFIGUI::Theme::AccentPurple());
    }
    EFIGUI::EndCustomWindow();

    EFIGUI::EndFrame();
}

// Optional: Customize theme at startup
void InitializeTheme()
{
    auto& config = EFIGUI::Theme::GetConfig();

    // Custom color scheme
    config.colors.accentPrimary = IM_COL32(255, 100, 50, 255);

    // Rounder corners - manually set rounding values
    config.dimensions.windowRounding = 999.0f;  // Pill shape
    config.dimensions.frameRounding = 999.0f;
    config.dimensions.buttonRounding = 999.0f;

    // Faster animations - adjust speed values directly
    config.animation.defaultSpeed = 16.0f;  // 2x faster
}
```

---

## Troubleshooting

### Compilation Error: "AccentCyan is not a type"

You're using the old constant syntax. Add parentheses:
```cpp
// Wrong
ImU32 color = Theme::AccentCyan;

// Right
ImU32 color = Theme::AccentCyan();
```

### Compilation Error: "Cannot use constexpr as default parameter"

You're passing an old-style constant as a default parameter. Use 0 instead:
```cpp
// Wrong
void MyFunc(float width = Theme::NavbarWidth);

// Right
void MyFunc(float width = 0);  // 0 = use theme default
```

### Theme changes not taking effect

Make sure you're modifying the theme before rendering:
```cpp
// Modify theme once at startup
auto& config = EFIGUI::Theme::GetConfig();
config.colors.accentPrimary = myColor;

// Or modify dynamically each frame (before BeginFrame)
```

---

## See Also

- [Theme Customization](theme.md) - Full theme API reference
- [Components Reference](components.md) - All component functions
- [CHANGELOG](../CHANGELOG.md) - Complete version history
