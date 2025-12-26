# Layer System (Z-Order Control)

## Overview

EFIGUI provides a Layer system for fine-grained control over rendering order. This solves z-order issues where glow effects might cover popup/dropdown content.

---

## Layer Hierarchy

| Layer | Order | Purpose |
|-------|-------|---------|
| `Background` | 0 | Background decorations |
| `Content` | 1 | General window content |
| `Widget` | 2 | Widget surfaces (buttons, inputs) |
| `WidgetGlow` | 3 | Widget glow effects |
| `Popup` | 4 | Popup/Dropdown surfaces |
| `PopupGlow` | 5 | Popup glow effects |
| `Overlay` | 6 | Tooltips, notifications |
| `Debug` | 7 | Debug info (topmost) |

---

## Basic Usage

The Layer system requires explicit frame management:

```cpp
// In your render loop
EFIGUI::BeginFrame();  // Start deferred drawing

if (EFIGUI::BeginBorderlessWindow("MyApp", &open))
{
    EFIGUI::GlowButton("Click Me");  // Uses default WidgetGlow layer

    static int selected = 0;
    const char* items[] = {"Option A", "Option B"};
    EFIGUI::ModernCombo("Dropdown", &selected, items, 2);  // Popup won't be covered
}
EFIGUI::EndBorderlessWindow();

EFIGUI::EndFrame();  // Flush all deferred drawing commands
```

---

## Frame Lifecycle Functions

| Function | Description |
|----------|-------------|
| `BeginFrame()` | Clears deferred drawing commands from previous frame |
| `EndFrame(targetDrawList)` | Flushes all deferred drawing commands in layer order |

**EndFrame optional parameter:**
- `targetDrawList` - Custom ImDrawList to render to (default: ForegroundDrawList)

```cpp
// Default: renders to ForegroundDrawList (recommended for single-window apps)
EFIGUI::EndFrame();

// Custom: render to a specific draw list (useful for multi-window or testing)
ImDrawList* myDrawList = ImGui::GetWindowDrawList();
EFIGUI::EndFrame(myDrawList);
```

---

## Custom Layer Per Widget

Override the default layer for specific widgets:

```cpp
// Button glow below popups (Widget layer instead of WidgetGlow)
EFIGUI::GlowButton("Low", ImVec2(0,0), std::nullopt, false, EFIGUI::Layer::Widget);

// Button glow above everything (overlay layer)
EFIGUI::GlowButton("High", ImVec2(0,0), std::nullopt, false, EFIGUI::Layer::Overlay);

// Slider with glow in popup layer
static float val = 0.5f;
EFIGUI::ModernSliderFloat("Value", &val, 0.0f, 1.0f, "%.1f", EFIGUI::Layer::PopupGlow);
```

---

## Global Layer Configuration

Change default layers for all widgets:

```cpp
// Get and modify layer configuration
EFIGUI::LayerConfig config;
config.defaultWidgetGlow = EFIGUI::Layer::Widget;      // Lower glow layer
config.defaultMarqueeBorder = EFIGUI::Layer::Widget;   // Lower marquee layer
config.autoElevateInPopup = false;                     // Disable auto-elevation
EFIGUI::GetLayerManager().SetConfig(config);
```

---

## LayerConfig Options

| Option | Default | Description |
|--------|---------|-------------|
| `defaultWidgetGlow` | `WidgetGlow` | Default layer for widget glow effects |
| `defaultMarqueeBorder` | `WidgetGlow` | Default layer for marquee borders |
| `defaultButtonBackground` | `Widget` | Default layer for button backgrounds |
| `defaultPopupBackground` | `Popup` | Default layer for popup backgrounds |
| `defaultPopupGlow` | `PopupGlow` | Default layer for popup glow effects |
| `defaultTooltip` | `Overlay` | Default layer for tooltips |
| `autoElevateInPopup` | `true` | Auto-elevate glow layers inside popups |

---

## Components with Layer Support

The following components accept an optional `layer` parameter:

- `GlowButton` - Glow and marquee border layer
- `DangerButton` - Glow and marquee border layer
- `ColoredButton` - Glow layer
- `CooldownButton` - Glow and marquee border layer
- `ModernSliderFloat` - Knob glow and input box glow layer
- `ModernSliderInt` - Knob glow and input box glow layer
- `ModernProgressBar` - Progress bar glow layer
