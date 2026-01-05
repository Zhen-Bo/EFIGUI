# EFIGUI - Escape From ImGui

A modern, cyberpunk-themed UI framework built on top of [Dear ImGui](https://github.com/ocornut/imgui). Features glassmorphism effects, neon glow animations, and a complete set of styled components.

## Features

- **Cyberpunk Theme**: Dark backgrounds with neon cyan/purple accents
- **Glassmorphism Effects**: Real-time blur with DX11 backend (DX12/Vulkan/OpenGL planned)
- **Animated Components**: Smooth hover transitions, marquee borders, pulsing glows
- **Complete Widget Set**: Buttons, toggles, sliders, inputs, combos, progress bars, and more
- **Font Awesome Icons**: Pre-defined icon constants for easy use
- **Cross-Platform Backend**: Abstract blur interface for multiple graphics APIs
- **Flexible Customization**: Optional parameters for per-instance styling without breaking defaults
- **Per-Instance Config** (v0.3.1+): Full control over individual widget appearance with Config structs
- **Semantic Card Layout** (v0.3.2+): Consistent card heights at all DPI scales with semantic padding parameters

---

## What's New in v0.3.2

**FeatureCard Height Fix**: Cards now render with consistent bottom margins at 150%/200% UI scale:

```cpp
// New semantic padding parameters in CardTheme
EFIGUI::Theme::CardMut().topPadding = 10.0f;
EFIGUI::Theme::CardMut().bottomPadding = 12.0f;
EFIGUI::Theme::CardMut().titleDescGap = 2.0f;

// Per-instance override via CardConfig
EFIGUI::CardConfig config;
config.topPadding = 15.0f;
config.bottomPadding = 15.0f;
EFIGUI::FeatureCard(Icons::Shield, "Shield", "Protection mode", &enabled, config);
```

The fix uses `CalcWordWrapPositionA` for consistent line counting instead of `CalcTextSize`, ensuring accurate height calculation across all DPI scales.

See [CHANGELOG.md](CHANGELOG.md) for complete details.

---

## What's New in v0.3.1

**Per-Instance Configuration**: Every widget now supports a Config struct for granular customization:

```cpp
// Customize individual sliders
EFIGUI::SliderConfig config;
config.height = 32.0f;              // Taller slider
config.knobRadius = 12.0f;          // Larger knob
config.glowColor = Theme::AccentPurple();

EFIGUI::ModernSliderFloat("Custom", &val, 0.0f, 1.0f, "%.2f", config);
```

**Component Theme Accessors**: Direct access to component-specific theme values:

```cpp
float sliderHeight = EFIGUI::Theme::Slider().height;  // Read
EFIGUI::Theme::SliderMut().height = 32.0f;            // Modify at runtime
```

**Smooth Animation Helpers**: New utilities for dynamic UI transitions:

```cpp
static EFIGUI::Animation::SmoothFloat scale;
scale.SetTarget(isHovered ? 1.2f : 1.0f);

EFIGUI::GlowButtonConfig btnConfig;
btnConfig.size = ImVec2(100 * scale.Update(), 36);
EFIGUI::GlowButton("Dynamic", btnConfig);
```

See [CHANGELOG.md](CHANGELOG.md) for complete details.

---

## Quick Start

```cpp
#include <EFIGUI/EFIGUI.h>

// Initialize
EFIGUI::Initialize();

// In your render loop
EFIGUI::BeginFrame();

if (EFIGUI::BeginCustomWindow("My Window", nullptr))
{
    if (EFIGUI::GlowButton("Click Me!"))
    {
        // Button clicked!
    }

    static bool enabled = false;
    EFIGUI::ModernToggle("Enable Feature", &enabled);

    static float value = 0.5f;
    EFIGUI::ModernSliderFloat("Value", &value, 0.0f, 1.0f);
}
EFIGUI::EndCustomWindow();

EFIGUI::EndFrame();

// Cleanup
EFIGUI::Shutdown();
```

---

## Requirements

- **C++17** or later
- **Dear ImGui** (linked separately)
- **Visual Studio 2019+** or compatible C++17 compiler

---

## Documentation

| Document | Description |
|----------|-------------|
| [Installation](docs/installation.md) | Setup guide for Visual Studio and CMake |
| [Quick Start](docs/quick-start.md) | Basic usage examples |
| [Components](docs/components.md) | Full component reference |
| [Optional Parameters](docs/optional-parameters.md) | Understanding `std::optional` usage |
| [Theme](docs/theme.md) | Color and style customization |
| [Animation](docs/animation.md) | Animation system reference |
| [Layer System](docs/layer-system.md) | Z-order control for rendering |
| [Blur Backend](docs/blur-backend.md) | Glassmorphism blur effects |
| [Icons](docs/icons.md) | Icon customization and Font Awesome |

---

## Available Components

### Windows & Panels
- `BeginBorderlessWindow` / `EndBorderlessWindow`
- `BeginCustomWindow` / `EndCustomWindow`
- `BeginGlassPanel` / `EndGlassPanel`

### Navigation
- `NavItem`, `NavbarHeader`, `NavCollapseButton`, `NavSectionHeader`

### Buttons
- `GlowButton`, `IconButton`, `DangerButton`, `ColoredButton`, `CooldownButton`

### Input
- `ModernToggle`, `ModernInputText`, `ModernSliderFloat`, `ModernSliderInt`, `ModernCombo`, `NumericInput`

### Display
- `ModernProgressBar`, `StatusIndicator`, `FeatureCard`, `SectionHeader`, `ModernTooltip`, `HelpMarker`

### Layout
- `Spacing`, `Separator`, `SameLine`

---

## License

GNU General Public License v3.0 - See LICENSE file for details.
