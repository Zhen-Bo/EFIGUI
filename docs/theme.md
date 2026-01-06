# Theme Customization

## Runtime Theme Configuration (v0.3.0+)

EFIGUI provides a comprehensive runtime theme system. All theme values can be modified at runtime.

```cpp
// Access the mutable configuration
auto& config = EFIGUI::Theme::GetConfig();

// Modify any value
config.colors.accentPrimary = IM_COL32(255, 100, 50, 255);
config.dimensions.windowRounding = 16.0f;
config.effects.glowIntensity = 0.8f;
config.animation.hoverSpeed = 15.0f;
```

---

## Core Layout Types (v0.4.0+)

v0.4.0 introduces unified layout types for consistent padding, margin, and spacing:

### EdgeInsets

Represents padding or margin on all four edges:

```cpp
// Named constructors
EdgeInsets::All(16.0f)                    // {16, 16, 16, 16}
EdgeInsets::Symmetric(8.0f, 16.0f)        // vertical=8, horizontal=16
EdgeInsets::Horizontal(24.0f)             // {0, 24, 0, 24}
EdgeInsets::Vertical(8.0f)                // {8, 0, 8, 0}
EdgeInsets::Only(10, 20, 10, 20)          // top, right, bottom, left

// Utility methods
EdgeInsets padding(10, 20, 10, 20);
float h = padding.horizontal();      // left + right = 40
float v = padding.vertical();        // top + bottom = 20
ImVec2 tl = padding.topLeft();       // {20, 10}
ImVec2 sz = padding.size();          // {40, 20}
```

### Spacing

Represents spacing between elements:

```cpp
Spacing::Both(8.0f)                       // {8, 8}
Spacing(12.0f, 8.0f)                      // x=12, y=8
ImVec2 v = spacing.toVec2();              // Convert to ImVec2
```

### CornerRadius

Represents corner rounding for rounded rectangles:

```cpp
CornerRadius::All(8.0f)                   // All corners
CornerRadius::Top(12.0f)                  // Top corners only
CornerRadius::Bottom(12.0f)               // Bottom corners only
CornerRadius::None()                      // No rounding
CornerRadius::Pill()                      // Maximum rounding
```

---

## Component Theme Accessors (v0.3.1+)

Access component-specific theme configurations directly:

```cpp
// Component-specific theme values
float sliderHeight = EFIGUI::Theme::Slider().height;        // 24.0f
float toggleWidth = EFIGUI::Theme::Toggle().width;          // 48.0f
float cardIconSize = EFIGUI::Theme::Card().iconSize;        // 32.0f
float navAccentBar = EFIGUI::Theme::Nav().accentBarWidth;   // 3.0f

// Modify component themes at runtime
EFIGUI::Theme::SliderMut().height = 32.0f;
EFIGUI::Theme::ToggleMut().width = 56.0f;

// v0.4.0+: Use EdgeInsets for padding/margin
EFIGUI::Theme::ButtonMut().padding = EFIGUI::EdgeInsets::Symmetric(8, 24);
EFIGUI::Theme::CardMut().padding = EFIGUI::EdgeInsets(12, 16, 12, 16);
EFIGUI::Theme::NavMut().itemPadding = EFIGUI::EdgeInsets::Horizontal(16);
```

**Available Component Accessors:**

| Accessor | Mutable Accessor | Description |
|----------|------------------|-------------|
| `Theme::Slider()` | `Theme::SliderMut()` | Slider dimensions and effects |
| `Theme::Toggle()` | `Theme::ToggleMut()` | Toggle switch dimensions |
| `Theme::Card()` | `Theme::CardMut()` | Feature card layout |
| `Theme::Nav()` | `Theme::NavMut()` | Navigation item styling |
| `Theme::Button()` | `Theme::ButtonMut()` | Button dimensions and effects |
| `Theme::Window()` | `Theme::WindowMut()` | Window/title bar settings |
| `Theme::Draw()` | `Theme::DrawMut()` | Glow/marquee effect settings |
| `Theme::Layout()` | `Theme::LayoutMut()` | Spacing and layout values |
| `Theme::NumericInput()` | `Theme::NumericInputMut()` | Numeric input box settings |

---

## Using Theme Values

All theme values are accessed via function calls:

```cpp
// Colors
ImU32 text = EFIGUI::Theme::TextPrimary();      // White
ImU32 muted = EFIGUI::Theme::TextMuted();       // Gray
ImU32 accent = EFIGUI::Theme::AccentCyan();     // Neon cyan
ImU32 error = EFIGUI::Theme::StatusError();     // Red

// Dimensions
float rounding = EFIGUI::Theme::FrameRounding();  // 8.0f
float navWidth = EFIGUI::Theme::NavbarWidth();    // 200.0f
```

---

## Theme Presets

Load predefined theme presets:

```cpp
// Load preset
EFIGUI::Theme::LoadPreset(EFIGUI::ThemePreset::Cyberpunk);

// Reset to default
EFIGUI::Theme::ResetToDefault();
```

---

## Available Theme Colors

| Accessor Function | Description |
|-------------------|-------------|
| `BackgroundDark()` | Main dark background |
| `BackgroundPanel()` | Semi-transparent panel |
| `BackgroundNavbar()` | Left navbar |
| `BackgroundContent()` | Content area (glass) |
| `AccentCyan()` | Primary neon accent |
| `AccentPurple()` | Secondary accent |
| `AccentPink()` | Pink accent |
| `AccentGreen()` | Green accent |
| `TextPrimary()` | White text |
| `TextSecondary()` | Light gray text |
| `TextMuted()` | Muted gray text |
| `TextAccent()` | Accent-colored text |
| `StatusSuccess()` | Green (success) |
| `StatusWarning()` | Yellow (warning) |
| `StatusError()` | Red (error) |
| `StatusInfo()` | Cyan (info) |

---

## Available Theme Dimensions

| Accessor Function | Description | Default |
|-------------------|-------------|---------|
| `WindowRounding()` | Window corner rounding | 12.0f |
| `FrameRounding()` | Frame corner rounding | 8.0f |
| `NavItemRounding()` | Nav item rounding | 6.0f |
| `ButtonRounding()` | Button corner rounding | 8.0f |
| `NavbarWidth()` | Sidebar width | 200.0f |
| `NavbarWidthCollapsed()` | Collapsed width | 60.0f |
| `TitleBarHeight()` | Title bar height | 32.0f |
| `NavItemHeight()` | Nav item height | 36.0f |
| `ItemSpacing()` | Default item spacing | 8.0f |

---

## Color Utilities

EFIGUI provides helper functions for color manipulation:

```cpp
// Extract RGB components from ImU32 color
ImU32 myColor = Theme::AccentCyan();
Theme::ColorRGB rgb = Theme::ExtractRGB(myColor);
// rgb.r, rgb.g, rgb.b are now available

// Apply new alpha to existing color (preserves RGB)
ImU32 semiTransparent = Theme::ApplyAlpha(Theme::AccentCyan(), 128);

// Extract alpha from color
uint8_t alpha = Theme::ExtractAlpha(myColor);

// Create color from RGB + alpha
ImU32 newColor = Theme::MakeColor(rgb, 200);

// Convert ImU32 to ImVec4 (for ImGui style functions)
ImVec4 colorVec = Theme::ToVec4(Theme::AccentCyan());
```

---

## ThemeConfig Structure

### ThemeColors

```cpp
struct ThemeColors
{
    ImU32 accentPrimary;      // Primary accent (cyan)
    ImU32 accentSecondary;    // Secondary accent (purple)
    ImU32 accentPink;         // Pink accent
    ImU32 accentGreen;        // Green accent
    ImU32 backgroundDark;     // Main background
    ImU32 backgroundPanel;    // Panel background
    ImU32 backgroundNavbar;   // Navbar background
    ImU32 backgroundContent;  // Content area
    ImU32 textPrimary;        // Primary text
    ImU32 textSecondary;      // Secondary text
    ImU32 textMuted;          // Muted text
    ImU32 textAccent;         // Accent text
    ImU32 statusSuccess;      // Success indicator
    ImU32 statusWarning;      // Warning indicator
    ImU32 statusError;        // Error indicator
    ImU32 statusInfo;         // Info indicator
    // ... more colors
};
```

### ThemeDimensions

```cpp
struct ThemeDimensions
{
    float windowRounding;
    float frameRounding;
    float navItemRounding;
    float buttonRounding;
    float navbarWidth;
    float navbarWidthCollapsed;
    float titleBarHeight;
    float navItemHeight;
    float navItemPadding;
    float toggleWidth;
    float toggleHeight;
    float toggleKnobSize;
    float toggleLabelGap;
    float itemSpacing;
    // ... more dimensions
};
```

### ThemeEffects

```cpp
struct ThemeEffects
{
    float glowIntensity;
    float glowRadius;
    float glassBlurRadius;
    ImU32 glassTint;
    float marqueeBorderWidth;
    float marqueeSpeed;
    // ... more effects
};
```

### ThemeAnimation

```cpp
struct ThemeAnimation
{
    float hoverSpeed;
    float activeSpeed;
    float selectedSpeed;
    float pulseFrequency;
    float sweepDuration;
    float breatheFrequency;
};
```

---

## Default Value Constants

Theme provides named constants for default values used by optional parameters:

| Accessor Function | Value | Used by |
|-------------------|-------|---------|
| `DefaultOverlayAlpha()` | 200 | `BeginBorderlessWindow` overlay |
| `DefaultBgAlpha()` | 30 | `IconButton` hover background |
| `DefaultButtonBgAlpha()` | 200 | `ColoredButton` background |
| `DefaultButtonHoverAlpha()` | 220 | `ColoredButton` hover |
| `DefaultDotSize()` | 8.0f | `StatusIndicator` dot size |
| `DefaultGlowBaseAlpha()` | 180.0f | Glow animation base |
| `DefaultGlowMaxAlpha()` | 75.0f | Glow animation intensity |

---

## Example: Creating a Custom Theme

```cpp
void InitializeCustomTheme()
{
    auto& config = EFIGUI::Theme::GetConfig();

    // Custom color palette - warm orange/red theme
    config.colors.accentPrimary = IM_COL32(255, 100, 50, 255);    // Orange
    config.colors.accentSecondary = IM_COL32(255, 50, 100, 255);  // Red-pink
    config.colors.backgroundDark = IM_COL32(20, 15, 15, 255);     // Warm dark

    // Rounder UI elements - manually set rounding values
    config.dimensions.windowRounding = 999.0f;  // Pill shape
    config.dimensions.frameRounding = 999.0f;
    config.dimensions.buttonRounding = 999.0f;
    config.dimensions.navItemRounding = 999.0f;

    // Stronger glow effects
    config.effects.glowIntensity = 0.8f;
    config.effects.glowRadius = 12.0f;

    // Snappy animations - adjust speed values directly
    config.animation.defaultSpeed = 16.0f;  // 2x faster
}
```

---

## Migration from v0.2.x / v0.3.x

See [Migration Guide v0.3.0](migration-0.3.0.md) for migrating from v0.2.x.
See [Migration Guide v0.4.0](migration-0.4.0.md) for migrating from v0.3.x.

**Quick reference:**
```cpp
// Old (v0.2.x)
ImU32 color = Theme::AccentCyan;

// v0.3.0+
ImU32 color = Theme::AccentCyan();

// v0.4.0+ (using EdgeInsets)
Theme::ButtonMut().padding = EdgeInsets::Symmetric(8, 24);
```
