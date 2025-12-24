# EFIGUI - Escape From ImGui

A modern, cyberpunk-themed UI framework built on top of [Dear ImGui](https://github.com/ocornut/imgui). Features glassmorphism effects, neon glow animations, and a complete set of styled components.

## Features

- **Cyberpunk Theme**: Dark backgrounds with neon cyan/purple accents
- **Glassmorphism Effects**: Real-time blur with DX11 backend (DX12/Vulkan/OpenGL planned)
- **Animated Components**: Smooth hover transitions, marquee borders, pulsing glows
- **Complete Widget Set**: Buttons, toggles, sliders, inputs, combos, progress bars, and more
- **Font Awesome Icons**: Pre-defined icon constants for easy use
- **Cross-Platform Backend**: Abstract blur interface for multiple graphics APIs

## Quick Start

```cpp
#include <EFIGUI/EFIGUI.h>

// Initialize (without blur)
EFIGUI::Initialize();

// Or with DX11 blur backend
EFIGUI::Initialize(EFIGUI::BackendType::DX11, pDevice, width, height);

// In your render loop
if (EFIGUI::BeginCustomWindow("My Window", nullptr, ImVec2(800, 600)))
{
    // Navbar
    EFIGUI::NavbarHeader("Menu", EFIGUI::Icons::Bars);

    if (EFIGUI::NavItem(EFIGUI::Icons::Home, "Home", selected == 0))
        selected = 0;
    if (EFIGUI::NavItem(EFIGUI::Icons::Cog, "Settings", selected == 1))
        selected = 1;

    // Content area
    ImGui::BeginChild("Content");

    if (EFIGUI::GlowButton("Click Me!", ImVec2(200, 50)))
    {
        // Button clicked
    }

    static bool enabled = false;
    EFIGUI::ModernToggle("Enable Feature", &enabled);

    static float value = 0.5f;
    EFIGUI::ModernSliderFloat("Value", &value, 0.0f, 1.0f);

    ImGui::EndChild();
}
EFIGUI::EndCustomWindow();

// Cleanup
EFIGUI::Shutdown();
```

## Components

### Windows
- `BeginBorderlessWindow` / `EndBorderlessWindow` - Frameless window with custom title bar
- `BeginCustomWindow` / `EndCustomWindow` - Complete themed window

### Navigation
- `NavItem` - Sidebar navigation item with icon
- `NavbarHeader` - Navigation section header
- `NavCollapseButton` - Collapse/expand sidebar button

### Buttons
- `GlowButton` - Primary button with glow effect
- `IconButton` - Icon-only button
- `DangerButton` - Red destructive action button
- `ColoredButton` - Custom colored button
- `CooldownButton` - Button with cooldown timer

### Input
- `ModernToggle` - Animated toggle switch
- `ModernToggleWithDesc` - Toggle with description text
- `ModernInputText` - Styled text input
- `ModernSliderFloat` / `ModernSliderInt` - Styled sliders
- `ModernCombo` - Styled dropdown

### Display
- `ModernProgressBar` - Progress indicator
- `StatusIndicator` - Status dot with label
- `FeatureCard` - Feature card with icon
- `SectionHeader` - Section divider with title
- `ModernTooltip` - Styled tooltip
- `HelpMarker` - Help icon with tooltip

### Drawing Utilities
- `Draw::GlowLayers` - Multi-layer glow effect
- `Draw::MarqueeBorder` - Animated flowing border
- `Draw::GlassmorphismBg` - Blur background (requires backend)
- `Draw::RectGradientH/V` - Gradient rectangles
- `Draw::NeonBorder` - Neon-style border

## Theme Customization

```cpp
// Set custom accent colors
EFIGUI::Theme::SetAccentColor(
    IM_COL32(255, 100, 100, 255),  // Primary (e.g., red)
    IM_COL32(255, 50, 150, 255)    // Secondary (e.g., pink)
);

// Access theme constants
float rounding = EFIGUI::Theme::FrameRounding;
ImU32 textColor = EFIGUI::Theme::TextPrimary;
```

## Animation System

```cpp
// Get widget animation state
EFIGUI::Animation::WidgetState& state = EFIGUI::Animation::GetState(ImGui::GetID("myWidget"));

// Update animations
EFIGUI::Animation::UpdateWidgetState(state, isHovered, isActive, isSelected);

// Use animation values
float hover = state.hoverAnim;  // 0.0 to 1.0

// Periodic effects
float pulse = EFIGUI::Animation::Pulse(2.0f);      // Pulsing 0-1
float sweep = EFIGUI::Animation::Sweep(0.8f);      // Flowing 0-1
float breathe = EFIGUI::Animation::Breathe(1.0f);  // Smooth sine
```

## Blur Backend

EFIGUI uses an abstract backend system for glassmorphism blur effects:

```cpp
// DX11 (Windows)
EFIGUI::Initialize(EFIGUI::BackendType::DX11, pDevice, width, height);

// Check if blur is available
if (EFIGUI::HasBlurBackend())
{
    void* blurredTexture = EFIGUI::ApplyBlur(sourceSRV, 2.0f, 2);
}

// Handle window resize
EFIGUI::ResizeBlurBackend(newWidth, newHeight);
```

## Building

### CMake

```cmake
add_subdirectory(EFIGUI)
target_link_libraries(YourProject PRIVATE EFIGUI)

# EFIGUI requires ImGui - link your imgui target
target_link_libraries(EFIGUI PUBLIC imgui)
```

### Options

| Option | Default | Description |
|--------|---------|-------------|
| `EFIGUI_BUILD_EXAMPLES` | OFF | Build example applications |
| `EFIGUI_ENABLE_DX11` | ON | Enable DirectX 11 blur backend |
| `EFIGUI_ENABLE_DX12` | OFF | Enable DirectX 12 (future) |
| `EFIGUI_ENABLE_VULKAN` | OFF | Enable Vulkan (future) |
| `EFIGUI_ENABLE_OPENGL` | OFF | Enable OpenGL (future) |

## Dependencies

- [Dear ImGui](https://github.com/ocornut/imgui) (required)
- DirectX 11 SDK (for DX11 blur backend, Windows only)
- d3dcompiler_47.dll (runtime shader compilation)

## Font Awesome Icons

EFIGUI includes pre-defined Font Awesome 6 icon constants:

```cpp
// Use icons directly
ImGui::Text("%s Settings", EFIGUI::Icons::Cog);

// Available icons
EFIGUI::Icons::User
EFIGUI::Icons::Home
EFIGUI::Icons::Search
EFIGUI::Icons::Check
EFIGUI::Icons::Warning
// ... and many more in Icons.h
```

To use icons, load Font Awesome into ImGui:

```cpp
// After adding your main font
ImGuiIO& io = ImGui::GetIO();
ImFontConfig config;
config.MergeMode = true;
static const ImWchar iconRanges[] = { 0xf000, 0xf8ff, 0 };
io.Fonts->AddFontFromFileTTF("fa-solid-900.ttf", 16.0f, &config, iconRanges);
```

## License

MIT License - See LICENSE file for details.
