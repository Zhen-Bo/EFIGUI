# EFIGUI - Escape From ImGui

A modern, cyberpunk-themed UI framework built on top of [Dear ImGui](https://github.com/ocornut/imgui). Features glassmorphism effects, neon glow animations, and a complete set of styled components.

## Features

- **Cyberpunk Theme**: Dark backgrounds with neon cyan/purple accents
- **Glassmorphism Effects**: Real-time blur with DX11 backend (DX12/Vulkan/OpenGL planned)
- **Animated Components**: Smooth hover transitions, marquee borders, pulsing glows
- **Complete Widget Set**: Full ImGui component coverage with consistent styling
- **StyleSystem** (v0.5.0): Unified style management with per-component customization
- **Font Awesome Icons**: Pre-defined icon constants for easy use
- **Cross-Platform Backend**: Abstract blur interface for multiple graphics APIs

---

## What's New in v0.5.0

**Complete Component Coverage & StyleSystem**: v0.5.0 brings full ImGui component coverage with a new unified styling system.

### New Components

```cpp
// Checkbox, RadioButton, Selectable
EFIGUI::Checkbox("Enable Feature", &enabled);
EFIGUI::RadioButton("Option A", &selected, 0);
EFIGUI::Selectable("Item", isSelected);

// TabBar
if (EFIGUI::BeginTabBar("tabs")) {
    if (EFIGUI::BeginTabItem("Settings")) {
        // Tab content...
        EFIGUI::EndTabItem();
    }
    EFIGUI::EndTabBar();
}

// ColorEdit with glow effects
static float color[4] = {1, 0, 0, 1};
EFIGUI::ColorEdit4("Color", color);

// DragFloat/Int with visual feedback
EFIGUI::DragFloat("Speed", &speed, 0.1f, 0.0f, 100.0f);

// Vector inputs with X/Y/Z/W color coding
static float pos[3] = {0, 0, 0};
EFIGUI::InputFloat3("Position", pos);
EFIGUI::SliderFloat3("Scale", scale, 0.0f, 10.0f);

// Vertical sliders
EFIGUI::VSliderFloat("##v", ImVec2(30, 150), &value, 0.0f, 1.0f);

// Plot with grid and glow
EFIGUI::PlotLines("Signal", data, 100);
```

### StyleSystem

```cpp
// Default style - zero configuration
EFIGUI::Checkbox("Option", &val);

// Custom style for single widget
CheckboxStyle style;
style.checkColor = CyberpunkTheme::Colors::AccentPurple;
EFIGUI::CheckboxEx("Special", &val, style);

// Scoped style override
EFIGUI::WithStyle(style, [&] {
    EFIGUI::Checkbox("A", &a);
    EFIGUI::Checkbox("B", &b);
    EFIGUI::Checkbox("C", &c);
});
```

### C++23 Upgrade

v0.5.0 requires C++23 for modern features including designated initializers:

```cpp
CheckboxStyle style{
    .checkColor = CyberpunkTheme::Colors::AccentCyan,
    .size = 20.0f,
    .rounding = 4.0f
};
```

See [Migration Guide](docs/migration-0.5.0.md) and [CHANGELOG.md](CHANGELOG.md) for complete details.

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
    EFIGUI::Checkbox("Enable Feature", &enabled);

    static float value = 0.5f;
    EFIGUI::ModernSliderFloat("Value", &value, 0.0f, 1.0f);

    static float color[3] = {0.0f, 0.8f, 1.0f};
    EFIGUI::ColorEdit3("Accent", color);
}
EFIGUI::EndCustomWindow();

EFIGUI::EndFrame();

// Cleanup
EFIGUI::Shutdown();
```

---

## Requirements

- **C++23** or later
- **Dear ImGui** (linked separately)
- **Visual Studio 2022+** or compatible C++23 compiler

---

## Documentation

| Document | Description |
|----------|-------------|
| [Installation](docs/installation.md) | Setup guide for Visual Studio and CMake |
| [Quick Start](docs/quick-start.md) | Basic usage examples |
| [Components](docs/components.md) | Full component reference |
| [Theme](docs/theme.md) | Color and style customization |
| [Animation](docs/animation.md) | Animation system reference |
| [Layer System](docs/layer-system.md) | Z-order control for rendering |
| [Blur Backend](docs/blur-backend.md) | Glassmorphism blur effects |
| [Icons](docs/icons.md) | Icon customization and Font Awesome |
| [Migration Guide](docs/migration-0.5.0.md) | Upgrading from v0.4.x |

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
- `ImageButton`

### Selection
- `Checkbox`, `RadioButton`, `Selectable`

### Input
- `ModernToggle`, `ModernInputText`, `ModernSliderFloat/Int`, `ModernCombo`
- `NumericInput`, `DragFloat/Int`, `InputFloat2/3/4`, `InputInt2/3/4`
- `SliderFloat2/3/4`, `SliderInt2/3/4`, `VSliderFloat/Int`

### Color
- `ColorEdit3/4`, `ColorPicker3/4`, `ColorButton`

### Layout
- `BeginTabBar` / `EndTabBar`, `BeginTabItem` / `EndTabItem`
- `TreeNode` / `TreePop`, `CollapsingHeader`
- `BeginTable` / `EndTable`, `BeginListBox` / `EndListBox`
- `BeginMenu` / `EndMenu`, `MenuItem`

### Popup
- `OpenPopup`, `BeginPopup` / `EndPopup`
- `BeginPopupModal` / `EndPopupModal`

### Display
- `ModernProgressBar`, `StatusIndicator`, `FeatureCard`, `SectionHeader`
- `PlotLines`, `PlotHistogram`
- `ModernTooltip`, `HelpMarker`

### Layout Helpers
- `Spacing`, `Separator`, `SameLine`

---

## License

Copyright (c) 2026 Zhen-Bo

This project is licensed under the [Polyform Noncommercial License 1.0.0](https://polyformproject.org/licenses/noncommercial/1.0.0/) - see the [LICENSE](LICENSE) file for details.

### Commercial Use

Commercial use requires a separate license. For commercial licensing inquiries, please open an issue.
