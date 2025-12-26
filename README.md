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

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Installation](#installation)
3. [Quick Start](#quick-start)
4. [Understanding Optional Parameters](#understanding-optional-parameters)
5. [Components Reference](#components-reference)
6. [Theme Customization](#theme-customization)
7. [Animation System](#animation-system)
8. [Blur Backend](#blur-backend)
9. [Building](#building)
10. [Icons](#icons)

---

## Prerequisites

Before using EFIGUI, ensure you have:

- **Working ImGui project** - EFIGUI builds on top of Dear ImGui
- **C++17 or later** - Required for `std::optional` support
- **Visual Studio 2019+** (Windows) or compatible C++17 compiler

---

## Installation

### Option 1: Git Submodule (Recommended)

Add EFIGUI as a submodule in your project:

```bash
cd your-project
git submodule add https://github.com/YourRepo/EFIGUI.git EFIGUI
git submodule update --init --recursive
```

### Option 2: Direct Copy

Copy the `src/EFIGUI` folder into your project directory.

### Visual Studio Project Setup

After adding EFIGUI to your project folder:

**Step 1: Add Include Path**

1. Right-click your project → Properties
2. Go to **C/C++** → **General** → **Additional Include Directories**
3. Add the path to EFIGUI's `src` folder (e.g., `$(ProjectDir)EFIGUI\src`)

**Step 2: Add Source Files**

Add these files to your Visual Studio project (right-click project → Add → Existing Item):

| File | Required |
|------|----------|
| `EFIGUI/EFIGUI.h` | Yes |
| `EFIGUI/EFIGUI.cpp` | Yes |
| `EFIGUI/Theme.h` | Yes |
| `EFIGUI/Theme.cpp` | Yes |
| `EFIGUI/Animation.h` | Yes |
| `EFIGUI/Animation.cpp` | Yes |
| `EFIGUI/Components.h` | Yes |
| `EFIGUI/Components.cpp` | Yes |
| `EFIGUI/Draw.h` | Yes |
| `EFIGUI/Draw.cpp` | Yes |
| `EFIGUI/Backend/IBlurBackend.h` | Only if using blur |
| `EFIGUI/Backend/BlurBackendDX11.h` | Only if using DX11 blur |
| `EFIGUI/Backend/BlurBackendDX11.cpp` | Only if using DX11 blur |

**Step 3: Enable C++17**

1. Right-click your project → Properties
2. Go to **C/C++** → **Language** → **C++ Language Standard**
3. Select **ISO C++17 Standard (/std:c++17)** or later

### Integrating with Your Existing ImGui Project

In your existing code where you initialize ImGui:

```cpp
#include <EFIGUI/EFIGUI.h>

// After ImGui initialization
ImGui::CreateContext();
// ... your ImGui setup ...

// Initialize EFIGUI theme (without blur)
EFIGUI::Initialize();

// OR with DX11 blur (optional)
// EFIGUI::Initialize(EFIGUI::BackendType::DX11, pDevice, width, height);
```

In your render loop, replace ImGui widgets with EFIGUI components:

```cpp
// Before (vanilla ImGui)
if (ImGui::Button("Click Me")) { /* ... */ }

// After (EFIGUI)
if (EFIGUI::GlowButton("Click Me")) { /* ... */ }
```

Before shutdown:

```cpp
EFIGUI::Shutdown();
ImGui::DestroyContext();
```

---

## Quick Start

### Minimal Example

```cpp
#include <EFIGUI/EFIGUI.h>

// Initialize (without blur)
EFIGUI::Initialize();

// In your render loop
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

// Cleanup on exit
EFIGUI::Shutdown();
```

### Complete Example with Navbar

```cpp
#include <EFIGUI/EFIGUI.h>

// Initialize with DX11 blur backend (optional)
EFIGUI::Initialize(EFIGUI::BackendType::DX11, pDevice, width, height);

// In your render loop
static int selected = 0;
static bool navCollapsed = false;

if (EFIGUI::BeginCustomWindow("My App", nullptr))
{
    // Navbar header with collapse button
    if (EFIGUI::NavbarHeader("Menu", EFIGUI::Icons::Expand, EFIGUI::Icons::Collapse,
                              navCollapsed, EFIGUI::Theme::NavbarWidth))
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

// Cleanup
EFIGUI::Shutdown();
```

---

## Understanding Optional Parameters

### Design Principle

EFIGUI uses `std::optional<T>` to distinguish between "use default" and "use specific value":

- **Omit parameter** → Uses Theme default value
- **Provide any value (including 0)** → Uses that exact value

```cpp
// Omit parameter = use Theme default cyan glow
GlowButton("OK");

// Provide color = use red glow
GlowButton("OK", ImVec2(0,0), Theme::StatusError);

// Provide 0 = fully transparent (alpha = 0)
BeginGlassPanel("panel", size, true, 0);
```

### How `std::optional` Works

`std::optional<T>` is a C++17 feature that represents "maybe a value, maybe nothing".

```cpp
#include <optional>

std::optional<int> maybeNumber;           // Empty (no value)
std::optional<int> definitelyNumber = 42; // Has value 42

if (maybeNumber.has_value()) { /* won't execute */ }
if (definitelyNumber.has_value()) { /* will execute */ }

int x = definitelyNumber.value();     // x = 42
int y = maybeNumber.value_or(100);    // y = 100 (fallback)
```

### Using Optional Parameters in EFIGUI

#### Pattern 1: Don't provide the parameter (use default)

```cpp
// All these use theme defaults:
EFIGUI::GlowButton("Click");                    // Default cyan glow
EFIGUI::BeginGlassPanel("panel");               // Default background alpha
EFIGUI::StatusIndicator("Online", greenColor);  // Default dot size (8.0f)
```

#### Pattern 2: Provide `std::nullopt` explicitly (same as not providing)

```cpp
// These are equivalent to Pattern 1:
EFIGUI::GlowButton("Click", ImVec2(0,0), std::nullopt);
EFIGUI::BeginGlassPanel("panel", ImVec2(0,0), true, std::nullopt);
```

#### Pattern 3: Provide a specific value

```cpp
// Custom red glow
EFIGUI::GlowButton("Delete", ImVec2(0,0), IM_COL32(255, 60, 60, 255));

// 50% transparent background
EFIGUI::BeginGlassPanel("panel", ImVec2(0,0), true, 128);

// Larger status dot (12 pixels)
EFIGUI::StatusIndicator("Online", greenColor, false, 12.0f);

// Fully transparent background (alpha = 0)
EFIGUI::BeginGlassPanel("invisible", ImVec2(0,0), true, 0);
```

### Quick Reference: Optional Parameter Types

| Type | What it controls | Example values |
|------|-----------------|----------------|
| `std::optional<uint8_t>` | Alpha/transparency | `0` (transparent) to `255` (opaque) |
| `std::optional<ImU32>` | Colors | `IM_COL32(r, g, b, a)` or `Theme::AccentCyan` |
| `std::optional<float>` | Sizes/dimensions | `8.0f`, `12.0f`, etc. |

### Real-World Examples

```cpp
// Example 1: A panel with custom transparency
EFIGUI::BeginGlassPanel("myPanel",
    ImVec2(300, 200),  // size
    true,              // show border
    180                // 70% opaque (180/255)
);
// ... content ...
EFIGUI::EndGlassPanel();

// Example 2: Navigation with custom accent color
ImU32 myAccent = IM_COL32(255, 100, 50, 255);  // Orange
if (EFIGUI::NavItem(Icons::Home, "Dashboard", isSelected,
                     Theme::NavbarWidth, false, myAccent))
{
    // clicked
}

// Example 3: Status indicator with larger dot
EFIGUI::StatusIndicator("Server Online",
    Theme::StatusSuccess,  // green
    true,                  // pulse animation
    12.0f                  // 12px dot (default is 8px)
);

// Example 4: Feature card with semi-transparent background
EFIGUI::FeatureCard(
    Icons::Shield,
    "God Mode",
    "Makes you invincible",
    &godModeEnabled,
    150  // Semi-transparent background
);
```

---

## Components Reference

### Windows

| Function | Description |
|----------|-------------|
| `BeginBorderlessWindow(name, p_open, flags, overlayAlpha)` | Frameless window with glassmorphism |
| `EndBorderlessWindow()` | End borderless window |
| `BeginCustomWindow(name, p_open, flags)` | Complete themed window with title bar |
| `EndCustomWindow()` | End custom window |

**BeginBorderlessWindow optional parameters:**
- `overlayAlpha` - Background overlay transparency (0-255, default: 200)

```cpp
// Default overlay
EFIGUI::BeginBorderlessWindow("Window", &open);

// More transparent overlay
EFIGUI::BeginBorderlessWindow("Window", &open, 0, 100);

// Almost opaque overlay
EFIGUI::BeginBorderlessWindow("Window", &open, 0, 240);
```

### Navigation

| Function | Description |
|----------|-------------|
| `NavItem(icon, label, selected, width, collapsed, accentColor)` | Sidebar nav item |
| `NavbarHeader(title, iconExp, iconCol, collapsed, width, closeClicked, glowColor)` | Nav header |
| `NavCollapseButton(collapsed, width)` | Collapse/expand button |
| `NavSectionHeader(label)` | Section divider |

**NavItem optional parameters:**
- `accentColor` - Highlight color when selected/hovered (default: Theme::AccentCyan)

```cpp
// Default cyan accent
EFIGUI::NavItem(Icons::Home, "Home", selected);

// Custom purple accent
EFIGUI::NavItem(Icons::Home, "Home", selected,
                Theme::NavbarWidth, false, Theme::AccentPurple);
```

### Buttons

| Function | Description |
|----------|-------------|
| `GlowButton(label, size, glowColor, forceHover)` | Primary glowing button |
| `IconButton(icon, size, color, bgAlpha)` | Icon-only button |
| `DangerButton(label, size)` | Red destructive button |
| `ColoredButton(label, size, borderColor, bgAlpha)` | Custom colored button |
| `CooldownButton(label, size, glowColor, cooldownProgress)` | Button with cooldown |

**GlowButton optional parameters:**
- `glowColor` - Glow color (default: Theme::AccentCyan)

```cpp
// Default cyan glow
EFIGUI::GlowButton("OK");

// Red glow for dangerous action
EFIGUI::GlowButton("Delete All", ImVec2(0,0), Theme::StatusError);

// Green glow for positive action
EFIGUI::GlowButton("Confirm", ImVec2(0,0), Theme::StatusSuccess);
```

**IconButton optional parameters:**
- `color` - Icon color (default: Theme::TextSecondary)
- `bgAlpha` - Background alpha on hover (default: 30)

```cpp
// Default styling
EFIGUI::IconButton(Icons::Cog);

// Red icon with more visible hover background
EFIGUI::IconButton(Icons::Error, ImVec2(28,28), Theme::StatusError, 60);
```

**ColoredButton optional parameters:**
- `bgAlpha` - Background alpha (default: 200)

```cpp
// Cyan bordered button
EFIGUI::ColoredButton("Action", ImVec2(100,36), Theme::AccentCyan);

// Same but more transparent
EFIGUI::ColoredButton("Action", ImVec2(100,36), Theme::AccentCyan, 100);
```

### Glass Panel

| Function | Description |
|----------|-------------|
| `BeginGlassPanel(id, size, border, bgAlpha, glowColor)` | Semi-transparent panel |
| `EndGlassPanel()` | End panel |

**Optional parameters:**
- `bgAlpha` - Background transparency (0-255)
- `glowColor` - Top edge glow color

```cpp
// Default styling
EFIGUI::BeginGlassPanel("panel1");
// content...
EFIGUI::EndGlassPanel();

// Custom: 50% transparent, purple glow
EFIGUI::BeginGlassPanel("panel2", ImVec2(300, 200), true,
                         128, Theme::AccentPurple);
// content...
EFIGUI::EndGlassPanel();
```

### Input

| Function | Description |
|----------|-------------|
| `ModernToggle(label, value)` | Animated toggle switch |
| `ModernToggleWithDesc(label, desc, value)` | Toggle with description |
| `ModernInputText(label, buf, size, flags)` | Styled text input |
| `ModernInputText(label, str, flags)` | Auto-resizing string input |
| `ModernInputTextMultiline(label, buf, buf_size, size, flags)` | Multiline text input |
| `ModernInputTextMultiline(label, str, size, flags)` | Auto-resizing multiline input |
| `ModernSliderFloat(label, value, min, max, format)` | Float slider |
| `ModernSliderInt(label, value, min, max, format)` | Integer slider |
| `ModernCombo(label, item, items, count, popupBgAlpha)` | Styled dropdown |

**ModernCombo optional parameters:**
- `popupBgAlpha` - Popup background transparency (0-255)

```cpp
const char* items[] = { "Option 1", "Option 2", "Option 3" };
static int current = 0;

// Default popup background
EFIGUI::ModernCombo("Select", &current, items, 3);

// More opaque popup
EFIGUI::ModernCombo("Select", &current, items, 3, 230);
```

### Display

| Function | Description |
|----------|-------------|
| `ModernProgressBar(fraction, size, overlay)` | Progress indicator |
| `StatusIndicator(label, color, pulse, dotSize)` | Status dot with text |
| `FeatureCard(icon, name, desc, enabled, bgAlpha)` | Feature toggle card |
| `SectionHeader(label, collapsed, accentColor)` | Section divider |
| `ModernTooltip(text)` | Styled tooltip |
| `HelpMarker(text)` | Help icon with tooltip |

**StatusIndicator optional parameters:**
- `dotSize` - Size of the status dot (default: 8.0f)

```cpp
// Default 8px dot
EFIGUI::StatusIndicator("Connected", Theme::StatusSuccess, true);

// Larger 14px dot
EFIGUI::StatusIndicator("Connected", Theme::StatusSuccess, true, 14.0f);
```

**FeatureCard optional parameters:**
- `bgAlpha` - Card background transparency (0-255)

```cpp
static bool featureOn = false;

// Default background
EFIGUI::FeatureCard(Icons::Shield, "Shield", "Protection mode", &featureOn);

// Semi-transparent background
EFIGUI::FeatureCard(Icons::Shield, "Shield", "Protection mode", &featureOn, 150);
```

**SectionHeader optional parameters:**
- `accentColor` - Header text color

```cpp
// Default cyan text
EFIGUI::SectionHeader("Settings");

// Purple header
EFIGUI::SectionHeader("Settings", nullptr, Theme::AccentPurple);
```

### Layout Helpers

| Function | Description |
|----------|-------------|
| `Spacing(height)` | Vertical space |
| `Separator(color)` | Horizontal line |
| `SameLine(offset, spacing)` | Same line layout |

**Separator optional parameters:**
- `color` - Line color (default: Theme::BorderDefault)

```cpp
// Default gray separator
EFIGUI::Separator();

// Cyan separator
EFIGUI::Separator(Theme::AccentCyan);
```

---

## Theme Customization

### Using Theme Constants

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

### Setting Custom Accent Colors

```cpp
// Change the global accent color scheme
EFIGUI::Theme::SetAccentColor(
    IM_COL32(255, 100, 100, 255),  // Primary (e.g., red)
    IM_COL32(255, 50, 150, 255)    // Secondary (e.g., pink)
);
```

### Available Theme Colors

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

### Color Utilities

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

### Default Value Constants

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

---

## Animation System

### Widget Animation State

```cpp
// Get animation state for any widget
ImGuiID id = ImGui::GetID("myWidget");
EFIGUI::Animation::WidgetState& state = EFIGUI::Animation::GetState(id);

// Update based on widget interaction
EFIGUI::Animation::UpdateWidgetState(state, isHovered, isActive, isSelected);

// Use the animated values (0.0 to 1.0)
float hoverProgress = state.hoverAnim;
float activeProgress = state.activeAnim;
float selectedProgress = state.selectedAnim;
```

### Periodic Effects

```cpp
// Pulsing effect (0 to 1 to 0...)
float pulse = EFIGUI::Animation::Pulse(2.0f);  // 2 Hz frequency

// Sweeping effect (0 to 1, then resets)
float sweep = EFIGUI::Animation::Sweep(0.8f);  // 0.8 seconds per sweep

// Smooth breathing (sine wave)
float breathe = EFIGUI::Animation::Breathe(1.0f);  // 1 Hz frequency
```

### Color Interpolation

```cpp
// Lerp between two colors based on animation
ImU32 color = EFIGUI::Animation::LerpColorU32(
    Theme::TextSecondary,  // Start color
    Theme::AccentCyan,     // End color
    state.hoverAnim        // 0.0 to 1.0
);
```

---

## Blur Backend

### Initialization

```cpp
// Without blur (works everywhere)
EFIGUI::Initialize();

// With DX11 blur (Windows + DirectX 11)
EFIGUI::Initialize(EFIGUI::BackendType::DX11, pDevice, width, height);
```

### Using Blur

```cpp
// Check if blur is available
if (EFIGUI::HasBlurBackend())
{
    // Apply blur to a texture
    void* blurredTexture = EFIGUI::ApplyBlur(sourceSRV, 2.0f, 2);
}

// Handle window resize
EFIGUI::ResizeBlurBackend(newWidth, newHeight);
```

---

## Building

### CMake Integration

```cmake
# Add EFIGUI to your project
add_subdirectory(EFIGUI)
target_link_libraries(YourProject PRIVATE EFIGUI)

# EFIGUI requires ImGui - link your imgui target
target_link_libraries(EFIGUI PUBLIC imgui)
```

### Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `EFIGUI_BUILD_EXAMPLES` | OFF | Build example applications |
| `EFIGUI_ENABLE_DX11` | ON | Enable DirectX 11 blur backend |
| `EFIGUI_ENABLE_DX12` | OFF | Enable DirectX 12 (future) |
| `EFIGUI_ENABLE_VULKAN` | OFF | Enable Vulkan (future) |
| `EFIGUI_ENABLE_OPENGL` | OFF | Enable OpenGL (future) |

### Dependencies

- [Dear ImGui](https://github.com/ocornut/imgui) (required)
- DirectX 11 SDK (for DX11 blur backend, Windows only)
- d3dcompiler_47.dll (runtime shader compilation)

---

## Icons

### Default Icons

EFIGUI provides default text icons that work without any font setup:

```cpp
EFIGUI::Icons::Close       // "X"
EFIGUI::Icons::Minimize    // "-"
EFIGUI::Icons::Home        // "H"
EFIGUI::Icons::Cog         // "*"
// ... etc
```

### Customizing Icons

```cpp
// Use any text
EFIGUI::Icons::Close = "[X]";
EFIGUI::Icons::Check = "OK";

// Use Font Awesome (requires loading the font)
EFIGUI::Icons::Close = "\xef\x80\x8d";  // FA xmark
EFIGUI::Icons::Home  = "\xef\x80\x95";  // FA home
```

### Font Awesome Setup

```cpp
// After adding your main font
ImGuiIO& io = ImGui::GetIO();
ImFontConfig config;
config.MergeMode = true;
static const ImWchar iconRanges[] = { 0xf000, 0xf8ff, 0 };
io.Fonts->AddFontFromFileTTF("fa-solid-900.ttf", 16.0f, &config, iconRanges);

// Then set EFIGUI icons
EFIGUI::Icons::Close = "\xef\x80\x8d";  // xmark
EFIGUI::Icons::Home  = "\xef\x80\x95";  // home
EFIGUI::Icons::Cog   = "\xef\x80\x93";  // cog
```

### Available Icon Slots

- **Window controls:** `Close`, `Minimize`, `Collapse`, `Expand`
- **Navigation:** `ChevronLeft`, `ChevronRight`, `Bars`, `Home`, `Cog`, `User`
- **Common UI:** `Check`, `Warning`, `Info`, `Error`, `Eye`, `Shield`, `Bolt`

---

## License

GNU General Public License v3.0 - See LICENSE file for details.
