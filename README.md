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
- `ModernToggle`, `ModernInputText`, `ModernSliderFloat`, `ModernSliderInt`, `ModernCombo`

### Display
- `ModernProgressBar`, `StatusIndicator`, `FeatureCard`, `SectionHeader`, `ModernTooltip`, `HelpMarker`

### Layout
- `Spacing`, `Separator`, `SameLine`

---

## License

GNU General Public License v3.0 - See LICENSE file for details.
