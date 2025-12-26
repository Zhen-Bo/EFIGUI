# Theme Customization

## Using Theme Constants

```cpp
// Colors
ImU32 text = EFIGUI::Theme::TextPrimary;      // White
ImU32 muted = EFIGUI::Theme::TextMuted;       // Gray
ImU32 accent = EFIGUI::Theme::AccentCyan;     // Neon cyan
ImU32 error = EFIGUI::Theme::StatusError;     // Red

// Dimensions
float rounding = EFIGUI::Theme::FrameRounding;  // 8.0f
float navWidth = EFIGUI::Theme::NavbarWidth;    // 200.0f
```

---

## Setting Custom Accent Colors

```cpp
// Change the global accent color scheme
EFIGUI::Theme::SetAccentColor(
    IM_COL32(255, 100, 100, 255),  // Primary (e.g., red)
    IM_COL32(255, 50, 150, 255)    // Secondary (e.g., pink)
);
```

---

## Available Theme Colors

| Color | Description |
|-------|-------------|
| `BackgroundDark` | Main dark background |
| `BackgroundPanel` | Semi-transparent panel |
| `BackgroundNavbar` | Left navbar |
| `BackgroundContent` | Content area (glass) |
| `AccentCyan` | Primary neon accent |
| `AccentPurple` | Secondary accent |
| `AccentPink` | Pink accent |
| `AccentGreen` | Green accent |
| `TextPrimary` | White text |
| `TextSecondary` | Light gray text |
| `TextMuted` | Muted gray text |
| `StatusSuccess` | Green (success) |
| `StatusWarning` | Yellow (warning) |
| `StatusError` | Red (error) |
| `StatusInfo` | Cyan (info) |

---

## Color Utilities

EFIGUI provides helper functions for color manipulation in Theme.h:

```cpp
// Extract RGB components from ImU32 color
ImU32 myColor = Theme::AccentCyan;
Theme::ColorRGB rgb = Theme::ExtractRGB(myColor);
// rgb.r, rgb.g, rgb.b are now available

// Apply new alpha to existing color (preserves RGB)
ImU32 semiTransparent = Theme::ApplyAlpha(Theme::AccentCyan, 128);

// Extract alpha from color
uint8_t alpha = Theme::ExtractAlpha(myColor);

// Create color from RGB + alpha
ImU32 newColor = Theme::MakeColor(rgb, 200);
```

---

## Default Value Constants

Theme provides named constants for default values used by optional parameters:

| Constant | Value | Used by |
|----------|-------|---------|
| `DefaultOverlayAlpha` | 200 | `BeginBorderlessWindow` overlay |
| `DefaultBgAlpha` | 30 | `IconButton` hover background |
| `DefaultButtonBgAlpha` | 200 | `ColoredButton` background |
| `DefaultButtonHoverAlpha` | 220 | `ColoredButton` hover |
| `DefaultDotSize` | 8.0f | `StatusIndicator` dot size |
| `DefaultGlowBaseAlpha` | 180.0f | Glow animation base |
| `DefaultGlowMaxAlpha` | 75.0f | Glow animation intensity |
