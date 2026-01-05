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
- **Core Layout Types** (v0.4.0): Unified `EdgeInsets`, `Spacing`, `CornerRadius` for padding/margin control
- **Builder Pattern** (v0.4.0): Fluent API for Config structs (`.withHeight()`, `.withGlowColor()`, etc.)

---

## What's New in v0.4.0

**Core Layout Types & Builder Pattern**: v0.4.0 introduces unified layout types and fluent builder patterns for easier customization:

```cpp
// New fluent builder pattern for inline customization
EFIGUI::ModernSliderFloat("Volume", &volume, 0, 100, "%.0f", std::nullopt,
    EFIGUI::SliderConfig()
        .withHeight(32.0f)
        .withKnobRadius(12.0f)
        .withGlowColor(EFIGUI::Theme::AccentPurple()));

// EdgeInsets for precise padding control
EFIGUI::GlowButton("Confirm", EFIGUI::ButtonConfig()
    .withSize(150, 40)
    .withPadding(EFIGUI::EdgeInsets::Symmetric(8, 24))
    .withGlowColor(EFIGUI::Theme::StatusSuccess()));
```

**New Layout Types**:
- `EdgeInsets` - Unified padding/margin with named constructors (`All`, `Symmetric`, `Horizontal`, `Vertical`)
- `Spacing` - Element spacing (horizontal and vertical)
- `CornerRadius` - Per-corner rounding with presets (`All`, `Top`, `Bottom`, `Pill`)
- `ButtonConfig` - Full button customization (size, padding, colors, glow effects)

See [Migration Guide](docs/migration-0.4.0.md) and [CHANGELOG.md](CHANGELOG.md) for complete details.

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
