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

## Quick Styling Helpers

### Rounding Styles

```cpp
auto& config = EFIGUI::Theme::GetConfig();

// Sharp corners (0 rounding)
config.dimensions.ApplyRoundingStyle(EFIGUI::RoundingStyle::Sharp);

// Subtle rounding (4px)
config.dimensions.ApplyRoundingStyle(EFIGUI::RoundingStyle::Subtle);

// Rounded (8px) - default
config.dimensions.ApplyRoundingStyle(EFIGUI::RoundingStyle::Rounded);

// Pill shape (maximum rounding)
config.dimensions.ApplyRoundingStyle(EFIGUI::RoundingStyle::Pill);
```

### Animation Speed Presets

```cpp
auto& config = EFIGUI::Theme::GetConfig();

// Instant transitions (no animation)
config.animation.ApplySpeedPreset(EFIGUI::AnimSpeed::Instant);

// Fast (2x speed)
config.animation.ApplySpeedPreset(EFIGUI::AnimSpeed::Fast);

// Normal - default
config.animation.ApplySpeedPreset(EFIGUI::AnimSpeed::Normal);

// Slow (0.5x speed)
config.animation.ApplySpeedPreset(EFIGUI::AnimSpeed::Slow);

// Relaxed (0.25x speed)
config.animation.ApplySpeedPreset(EFIGUI::AnimSpeed::Relaxed);
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

    void ApplyRoundingStyle(RoundingStyle style);
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

    void ApplySpeedPreset(AnimSpeed speed);
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

    // Rounder UI elements
    config.dimensions.ApplyRoundingStyle(EFIGUI::RoundingStyle::Pill);

    // Stronger glow effects
    config.effects.glowIntensity = 0.8f;
    config.effects.glowRadius = 12.0f;

    // Snappy animations
    config.animation.ApplySpeedPreset(EFIGUI::AnimSpeed::Fast);
}
```

---

## Migration from v0.2.x

See [Migration Guide](migration-0.3.0.md) for detailed instructions on updating from v0.2.x.

**Quick reference:**
```cpp
// Old (v0.2.x)
ImU32 color = Theme::AccentCyan;

// New (v0.3.0)
ImU32 color = Theme::AccentCyan();
```
