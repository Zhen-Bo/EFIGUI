# Installation

## Prerequisites

Before using EFIGUI, ensure you have:

- **Working ImGui project** - EFIGUI builds on top of Dear ImGui
- **C++17 or later** - Required for `std::optional` support
- **Visual Studio 2019+** (Windows) or compatible C++17 compiler

---

## Option 1: Git Submodule (Recommended)

Add EFIGUI as a submodule in your project:

```bash
cd your-project
git submodule add https://github.com/YourRepo/EFIGUI.git EFIGUI
git submodule update --init --recursive
```

## Option 2: Direct Copy

Copy the `src/EFIGUI` folder into your project directory.

---

## Visual Studio Project Setup

After adding EFIGUI to your project folder:

### Step 1: Add Include Path

1. Right-click your project → Properties
2. Go to **C/C++** → **General** → **Additional Include Directories**
3. Add the path to EFIGUI's `src` folder (e.g., `$(ProjectDir)EFIGUI\src`)

### Step 2: Add Source Files

Add these files to your Visual Studio project (right-click project → Add → Existing Item):

| File | Required |
|------|----------|
| `EFIGUI/EFIGUI.h` | Yes |
| `EFIGUI/EFIGUI.cpp` | Yes |
| `EFIGUI/Theme.h` | Yes |
| `EFIGUI/Theme.cpp` | Yes |
| `EFIGUI/Animation.h` | Yes |
| `EFIGUI/Animation.cpp` | Yes |
| `EFIGUI/Layer.h` | Yes |
| `EFIGUI/Layer.cpp` | Yes |
| `EFIGUI/Components.h` | Yes |
| `EFIGUI/Components/Internal.h` | Yes |
| `EFIGUI/Components/Window.cpp` | Yes |
| `EFIGUI/Components/Panel.cpp` | Yes |
| `EFIGUI/Components/Navigation.cpp` | Yes |
| `EFIGUI/Components/Button.cpp` | Yes |
| `EFIGUI/Components/Toggle.cpp` | Yes |
| `EFIGUI/Components/Input.cpp` | Yes |
| `EFIGUI/Components/Slider.cpp` | Yes |
| `EFIGUI/Components/Combo.cpp` | Yes |
| `EFIGUI/Components/Progress.cpp` | Yes |
| `EFIGUI/Components/Card.cpp` | Yes |
| `EFIGUI/Components/Tooltip.cpp` | Yes |
| `EFIGUI/Components/Layout.cpp` | Yes |
| `EFIGUI/Draw.h` | Yes |
| `EFIGUI/Draw.cpp` | Yes |
| `EFIGUI/Backend/IBlurBackend.h` | Only if using blur |
| `EFIGUI/Backend/BlurBackendDX11.h` | Only if using DX11 blur |
| `EFIGUI/Backend/BlurBackendDX11.cpp` | Only if using DX11 blur |

### Step 3: Enable C++17

1. Right-click your project → Properties
2. Go to **C/C++** → **Language** → **C++ Language Standard**
3. Select **ISO C++17 Standard (/std:c++17)** or later

---

## Integration with ImGui

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

## CMake Integration

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
