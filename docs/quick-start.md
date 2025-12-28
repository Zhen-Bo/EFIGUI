# Quick Start

## Minimal Example

```cpp
#include <EFIGUI/EFIGUI.h>

// Initialize (without blur)
EFIGUI::Initialize();

// In your render loop
EFIGUI::BeginFrame();  // Required for Layer System

if (EFIGUI::BeginCustomWindow("My Window", nullptr))
{
    // A simple glowing button
    if (EFIGUI::GlowButton("Click Me!"))
    {
        // Button was clicked!
    }

    // A toggle switch
    static bool enabled = false;
    EFIGUI::ModernToggle("Enable Feature", &enabled);

    // A slider
    static float value = 0.5f;
    EFIGUI::ModernSliderFloat("Value", &value, 0.0f, 1.0f);
}
EFIGUI::EndCustomWindow();

EFIGUI::EndFrame();  // Flushes all deferred drawing

// Cleanup on exit
EFIGUI::Shutdown();
```

---

## Complete Example with Navbar

```cpp
#include <EFIGUI/EFIGUI.h>

// Initialize with DX11 blur backend (optional)
EFIGUI::Initialize(EFIGUI::BackendType::DX11, pDevice, width, height);

// In your render loop
EFIGUI::BeginFrame();  // Required for Layer System

static int selected = 0;
static bool navCollapsed = false;

if (EFIGUI::BeginCustomWindow("My App", nullptr))
{
    // Navbar header with collapse button
    if (EFIGUI::NavbarHeader("Menu", EFIGUI::Icons::Expand, EFIGUI::Icons::Collapse,
                              navCollapsed, EFIGUI::Theme::NavbarWidth()))
        navCollapsed = !navCollapsed;

    // Navigation items
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

EFIGUI::EndFrame();  // Flushes all deferred drawing

// Cleanup
EFIGUI::Shutdown();
```

---

## Frame Lifecycle

EFIGUI requires explicit frame management for the Layer system:

```cpp
// Start of frame
EFIGUI::BeginFrame();  // Clears deferred commands from previous frame

// ... render all EFIGUI widgets ...

// End of frame
EFIGUI::EndFrame();    // Flushes all layers to ForegroundDrawList
```

**Important**: Always call `BeginFrame()` at the start and `EndFrame()` at the end of your render loop. Missing calls will trigger debug assertions to help catch errors.

---

## Per-Instance Customization (v0.3.1+)

Every widget supports a Config struct for individual customization. This lets you make one slider bigger, one button a different color, etc.

### Basic Example: Custom Slider Size

```cpp
// Create a config and set what you want to change
EFIGUI::SliderConfig config;
config.height = 32.0f;      // Taller slider (default is 24)
config.knobRadius = 12.0f;  // Bigger knob (default is 8)

// Use the config with your slider
static float volume = 0.5f;
EFIGUI::ModernSliderFloat("Volume", &volume, 0.0f, 1.0f, "%.0f%%", config);

// Another slider with default settings (no config needed)
static float brightness = 0.5f;
EFIGUI::ModernSliderFloat("Brightness", &brightness, 0.0f, 1.0f);
```

### Custom Colors

```cpp
// Purple glow instead of default cyan
EFIGUI::SliderConfig purpleSlider;
purpleSlider.glowColor = EFIGUI::Theme::AccentPurple();

static float val = 0.5f;
EFIGUI::ModernSliderFloat("Purple", &val, 0.0f, 1.0f, "%.1f", purpleSlider);
```

### Available Config Structs

| Widget | Config Struct | Common Fields |
|--------|---------------|---------------|
| `ModernSliderFloat/Int` | `SliderConfig` | height, knobRadius, glowColor |
| `ModernToggle` | `ToggleConfig` | width, height, glowColor |
| `FeatureCard` | `CardConfig` | height, iconSize, bgAlpha |
| `NavItem` | `NavItemConfig` | height, accentColor, rounding |
| `GlowButton` | `GlowButtonConfig` | size, glowColor |

See [Components Reference](components.md) for complete field lists.

---

## Dynamic Scaling with Smooth Animation (v0.3.1+)

Want widgets to smoothly grow or shrink based on user interaction? Use `SmoothFloat`:

### Button that Grows on Hover

```cpp
// Declare once (static keeps the value between frames)
static EFIGUI::Animation::SmoothFloat buttonScale;

// Set target based on hover state
bool isHovered = ImGui::IsItemHovered();
if (isHovered) {
    buttonScale.SetTarget(1.2f);  // 120% size when hovered
} else {
    buttonScale.SetTarget(1.0f);  // Normal size
}

// Create button with smooth scaling
EFIGUI::GlowButtonConfig config;
config.size = ImVec2(100 * buttonScale.Update(), 36);
EFIGUI::GlowButton("Hover Me!", config);
```

### Slider Controls Button Sizes

```cpp
// Two buttons that scale opposite to each other based on slider
static EFIGUI::Animation::SmoothFloat leftScale, rightScale;
static float balance = 0.5f;

// Slider at 0 = left big, at 1 = right big
leftScale.SetTarget(1.0f - balance * 0.4f);   // 0.6 to 1.0
rightScale.SetTarget(0.6f + balance * 0.4f);  // 0.6 to 1.0

// Render with smooth animation
EFIGUI::GlowButtonConfig leftCfg, rightCfg;
leftCfg.size = ImVec2(80 * leftScale.Update(), 32);
rightCfg.size = ImVec2(80 * rightScale.Update(), 32);

EFIGUI::GlowButton("Left", leftCfg);
ImGui::SameLine();
EFIGUI::ModernSliderFloat("##balance", &balance, 0.0f, 1.0f);
ImGui::SameLine();
EFIGUI::GlowButton("Right", rightCfg);
```

### SmoothFloat Basics

| Method | What it does |
|--------|--------------|
| `SetTarget(1.5f)` | Set where you want the value to go |
| `Update()` | Smoothly move toward target, returns current value |
| `SetImmediate(1.0f)` | Jump to value instantly (no animation) |

See [Animation Reference](animation.md) for more details.
