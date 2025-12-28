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
