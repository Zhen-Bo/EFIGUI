# Theme Customization

EFIGUI provides a comprehensive theming system with two approaches:

1. **CyberpunkTheme** (v0.5.0) - Static constexpr theme values for zero-overhead access
2. **Runtime Theme Configuration** - Dynamic theme modification at runtime

---

## CyberpunkTheme (v0.5.0+)

The new CyberpunkTheme provides static constexpr access to theme values:

```cpp
// Colors
ImU32 accent = CyberpunkTheme::Colors::AccentCyan;
ImU32 surface = CyberpunkTheme::Colors::Surface;
ImU32 text = CyberpunkTheme::Colors::TextPrimary;

// Dimensions
float rounding = CyberpunkTheme::Dimensions::Rounding;
float border = CyberpunkTheme::Dimensions::BorderWidth;
int glowLayers = CyberpunkTheme::Dimensions::GlowLayers;
```

### Available Colors

| Constant | Description |
|----------|-------------|
| `AccentCyan` | Primary neon accent |
| `AccentCyanGlow` | Cyan glow color (with lower alpha) |
| `AccentPurple` | Secondary accent |
| `AccentPurpleGlow` | Purple glow color |
| `Surface` | Component surface background |
| `SurfaceHover` | Hover state surface |
| `SurfaceActive` | Active/pressed state surface |
| `Border` | Default border color |
| `TextPrimary` | Primary white text |
| `TextSecondary` | Secondary gray text |
| `StatusSuccess` | Green success indicator |
| `StatusWarning` | Yellow warning indicator |
| `StatusError` | Red error indicator |

### Available Dimensions

| Constant | Default | Description |
|----------|---------|-------------|
| `Rounding` | 4.0f | Default corner rounding |
| `BorderWidth` | 1.0f | Border stroke width |
| `GlowLayers` | 4 | Number of glow layers |
| `GlowIntensity` | 0.3f | Glow opacity multiplier |

---

## StyleSystem (v0.5.0+)

The StyleSystem manages per-component styles with stack-based scoping:

### Basic Usage

```cpp
// Get current style for a component type
const CheckboxStyle& style = EFIGUI::GetCurrentStyle<CheckboxStyle>();

// Push a custom style onto the stack
CheckboxStyle customStyle;
customStyle.checkColor = CyberpunkTheme::Colors::AccentPurple;
EFIGUI::PushStyle(customStyle);
// ... render widgets ...
EFIGUI::PopStyle<CheckboxStyle>();

// Scoped style with automatic cleanup
EFIGUI::WithStyle(customStyle, [&] {
    EFIGUI::Checkbox("A", &a);
    EFIGUI::Checkbox("B", &b);
});
```

### Available Style Structs

| Style Struct | Component |
|--------------|-----------|
| `CheckboxStyle` | Checkbox |
| `RadioStyle` | RadioButton |
| `SelectableStyle` | Selectable |
| `PopupStyle` | Popup, Modal |
| `TabBarStyle` | TabBar, TabItem |
| `TreeNodeStyle` | TreeNode, CollapsingHeader |
| `TableStyle` | Table |
| `ListBoxStyle` | ListBox |
| `MenuStyle` | Menu, MenuItem |
| `ColorEditStyle` | ColorEdit, ColorPicker |
| `DragStyle` | DragFloat/Int |
| `VSliderStyle` | VSliderFloat/Int |
| `PlotStyle` | PlotLines, PlotHistogram |
| `VectorInputStyle` | InputFloat2/3/4, InputInt2/3/4 |
| `VectorSliderStyle` | SliderFloat2/3/4, SliderInt2/3/4 |
| `ImageButtonStyle` | ImageButton |

### Style Struct Example

```cpp
// CheckboxStyle fields
struct CheckboxStyle {
    ImU32 frameColor = CyberpunkTheme::Colors::Surface;
    ImU32 frameHoverColor = CyberpunkTheme::Colors::SurfaceHover;
    ImU32 frameActiveColor = CyberpunkTheme::Colors::SurfaceActive;
    ImU32 borderColor = CyberpunkTheme::Colors::Border;
    ImU32 borderHoverColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 checkColor = CyberpunkTheme::Colors::AccentCyan;
    ImU32 glowColor = CyberpunkTheme::Colors::AccentCyanGlow;
    float size = 18.0f;
    float rounding = CyberpunkTheme::Dimensions::Rounding;
    float borderWidth = CyberpunkTheme::Dimensions::BorderWidth;
    float animSpeed = 0.15f;
    int glowLayers = CyberpunkTheme::Dimensions::GlowLayers;
    float glowIntensity = CyberpunkTheme::Dimensions::GlowIntensity;
};
```

---

## Runtime Theme Configuration

For dynamic theme changes, use the ThemeManager:

```cpp
// Access the mutable configuration
auto& config = EFIGUI::Theme::GetConfig();

// Modify any value
config.colors.accentPrimary = IM_COL32(255, 100, 50, 255);
config.dimensions.windowRounding = 16.0f;
config.effects.glowIntensity = 0.8f;
config.animation.hoverSpeed = 15.0f;
```

### Theme Presets

```cpp
// Load preset
EFIGUI::Theme::LoadPreset(EFIGUI::ThemePreset::Cyberpunk);

// Reset to default
EFIGUI::Theme::ResetToDefault();
```

---

## Color Utilities

EFIGUI provides helper functions for color manipulation:

```cpp
// Extract RGB components from ImU32 color
ImU32 myColor = CyberpunkTheme::Colors::AccentCyan;
Theme::ColorRGB rgb = Theme::ExtractRGB(myColor);

// Apply new alpha to existing color
ImU32 semiTransparent = Theme::ApplyAlpha(CyberpunkTheme::Colors::AccentCyan, 128);

// Extract alpha from color
uint8_t alpha = Theme::ExtractAlpha(myColor);

// Create color from RGB + alpha
ImU32 newColor = Theme::MakeColor(rgb, 200);

// Convert ImU32 to ImVec4
ImVec4 colorVec = Theme::ToVec4(CyberpunkTheme::Colors::AccentCyan);
```

---

## Creating Custom Themes

### Static Theme (Compile-time)

```cpp
struct MyCustomTheme {
    struct Colors {
        static constexpr ImU32 AccentCyan = IM_COL32(255, 100, 50, 255);
        static constexpr ImU32 AccentPurple = IM_COL32(255, 50, 100, 255);
        static constexpr ImU32 Surface = IM_COL32(30, 25, 25, 255);
        // ... other colors
    };

    struct Dimensions {
        static constexpr float Rounding = 8.0f;
        static constexpr float BorderWidth = 2.0f;
        // ... other dimensions
    };
};

// Use with StyleSystem
CheckboxStyle myCheckbox{
    .checkColor = MyCustomTheme::Colors::AccentCyan,
    .rounding = MyCustomTheme::Dimensions::Rounding
};
```

### Dynamic Theme (Runtime)

```cpp
void InitializeCustomTheme()
{
    auto& config = EFIGUI::Theme::GetConfig();

    // Custom color palette - warm orange/red theme
    config.colors.accentPrimary = IM_COL32(255, 100, 50, 255);
    config.colors.accentSecondary = IM_COL32(255, 50, 100, 255);
    config.colors.backgroundDark = IM_COL32(20, 15, 15, 255);

    // Rounder UI elements
    config.dimensions.windowRounding = 12.0f;
    config.dimensions.frameRounding = 8.0f;
    config.dimensions.buttonRounding = 8.0f;

    // Stronger glow effects
    config.effects.glowIntensity = 0.8f;
    config.effects.glowRadius = 12.0f;
}
```

---

## ThemeConfig Structure

### ThemeColors

```cpp
struct ThemeColors {
    ImU32 accentPrimary;      // Primary accent (cyan)
    ImU32 accentSecondary;    // Secondary accent (purple)
    ImU32 backgroundDark;     // Main background
    ImU32 backgroundPanel;    // Panel background
    ImU32 textPrimary;        // Primary text
    ImU32 textSecondary;      // Secondary text
    ImU32 statusSuccess;      // Success indicator
    ImU32 statusWarning;      // Warning indicator
    ImU32 statusError;        // Error indicator
    // ... more colors
};
```

### ThemeDimensions

```cpp
struct ThemeDimensions {
    float windowRounding;
    float frameRounding;
    float buttonRounding;
    float navbarWidth;
    float titleBarHeight;
    float itemSpacing;
    // ... more dimensions
};
```

### ThemeEffects

```cpp
struct ThemeEffects {
    float glowIntensity;
    float glowRadius;
    float glassBlurRadius;
    float marqueeBorderWidth;
    float marqueeSpeed;
    // ... more effects
};
```

### ThemeAnimation

```cpp
struct ThemeAnimation {
    float hoverSpeed;
    float activeSpeed;
    float selectedSpeed;
    float pulseFrequency;
    float sweepDuration;
    float breatheFrequency;
};
```

---

## Migration from v0.4.x

See [Migration Guide v0.5.0](migration-0.5.0.md) for details.

**Quick reference:**
```cpp
// v0.4.x - Runtime function calls
ImU32 color = Theme::AccentCyan();

// v0.5.0 - Static constexpr (preferred for new code)
ImU32 color = CyberpunkTheme::Colors::AccentCyan;

// v0.5.0 - Still works (backward compatible)
ImU32 color = Theme::AccentCyan();
```
