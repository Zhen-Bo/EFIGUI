# EFIGUI v0.5.0 Migration Guide

This guide helps you migrate from v0.4.x to v0.5.0.

---

## Breaking Changes

### 1. C++ Version Upgrade

v0.5.0 requires **C++23** (previously C++17).

**CMakeLists.txt update:**
```cmake
# Old
target_compile_features(YourProject PUBLIC cxx_std_17)

# New
target_compile_features(YourProject PUBLIC cxx_std_23)
```

**Visual Studio:** Project Properties > C/C++ > Language > C++ Language Standard > ISO C++23

---

### 2. New StyleSystem API

v0.5.0 introduces a new StyleSystem for component styling. The old Config structs are replaced with Style structs.

**Renamed structs:**
| Old (v0.4.x) | New (v0.5.0) |
|--------------|--------------|
| `ToggleConfig` | `CheckboxStyle` (for Checkbox) |
| `SliderConfig` | `SliderStyle` (legacy) / per-component styles |
| `ButtonConfig` | `ButtonStyle` (legacy) |
| `CardConfig` | `CardStyle` (legacy) |

**New component-specific styles:**
- `CheckboxStyle`, `RadioStyle`, `SelectableStyle`, `PopupStyle`
- `TabBarStyle`, `TreeNodeStyle`, `TableStyle`, `ListBoxStyle`, `MenuStyle`
- `ColorEditStyle`, `DragStyle`, `VSliderStyle`, `PlotStyle`
- `VectorInputStyle`, `VectorSliderStyle`, `ImageButtonStyle`

---

### 3. Theme Access Pattern

Theme values are now accessed via `CyberpunkTheme::Colors::*` and `CyberpunkTheme::Dimensions::*`:

```cpp
// Old (v0.4.x)
ImU32 color = Theme::AccentCyan();
float rounding = Theme::FrameRounding();

// New (v0.5.0)
ImU32 color = CyberpunkTheme::Colors::AccentCyan;
float rounding = CyberpunkTheme::Dimensions::Rounding;
```

Note: The old `Theme::*()` function syntax still works for backward compatibility.

---

### 4. New Component API Pattern

New v0.5.0 components follow this pattern:
- `Component()` - Uses default style from StyleSystem
- `ComponentEx()` - Accepts custom style parameter

```cpp
// Default style
EFIGUI::Checkbox("Enable", &value);

// Custom style
CheckboxStyle style;
style.checkColor = CyberpunkTheme::Colors::AccentPurple;
EFIGUI::CheckboxEx("Enable", &value, style);

// Scoped style override (affects all widgets in scope)
EFIGUI::WithStyle(style, [&] {
    EFIGUI::Checkbox("Option A", &a);
    EFIGUI::Checkbox("Option B", &b);
});
```

---

## New Components in v0.5.0

### Basic Components
- `Checkbox(label, *v)` - Traditional checkbox with glow
- `RadioButton(label, active)` / `RadioButton(label, *v, v_button)` - Radio buttons
- `Selectable(label, selected, flags, size)` - Selectable list item

### Popup/Modal
- `OpenPopup(str_id, flags)` - Open popup
- `BeginPopup(str_id, flags)` / `EndPopup()` - Popup container
- `BeginPopupModal(name, *p_open, flags)` / `EndPopupModal()` - Modal dialog

### Tab System
- `BeginTabBar(str_id, flags)` / `EndTabBar()` - Tab container
- `BeginTabItem(label, *p_open, flags)` / `EndTabItem()` - Tab content
- `TabItem(label)` - Simple tab button

### Tree/Collapsing
- `TreeNode(label)` / `TreePop()` - Tree node
- `CollapsingHeader(label, flags)` - Collapsible section

### Table
- `BeginTable(str_id, columns, flags, size)` / `EndTable()`
- `TableSetupColumn(label, flags, width)`
- `TableHeadersRow()`, `TableNextRow()`, `TableNextColumn()`

### ListBox
- `BeginListBox(label, size)` / `EndListBox()` - Manual item rendering
- `ListBox(label, *current, items, count, height)` - Simple list

### Menu
- `BeginMainMenuBar()` / `EndMainMenuBar()` - Main menu bar
- `BeginMenuBar()` / `EndMenuBar()` - Window menu bar
- `BeginMenu(label, enabled)` / `EndMenu()` - Menu dropdown
- `MenuItem(label, shortcut, selected, enabled)` - Menu item

### Color
- `ColorEdit3(label, col, flags)` / `ColorEdit4(label, col, flags)`
- `ColorPicker3(label, col, flags)` / `ColorPicker4(label, col, flags)`
- `ColorButton(desc_id, col, flags, size)`

### Drag Input
- `DragFloat(label, *v, speed, min, max, format, flags)`
- `DragFloat2/3/4(label, v[], ...)` - Vector drag
- `DragInt(label, *v, ...)` / `DragInt2/3/4(label, v[], ...)`
- `DragFloatRange2(label, *v_min, *v_max, ...)`
- `DragIntRange2(label, *v_min, *v_max, ...)`

### Vertical Slider
- `VSliderFloat(label, size, *v, min, max, format, flags)`
- `VSliderInt(label, size, *v, min, max, format, flags)`

### Plot
- `PlotLines(label, values, count, overlay, scale_min, scale_max, size)`
- `PlotHistogram(label, values, count, overlay, scale_min, scale_max, size)`
- Callback versions also available

### Vector Input
- `InputFloat2/3/4(label, v[], format, flags)` - X/Y/Z/W color-coded inputs
- `InputInt2/3/4(label, v[], flags)`

### Vector Slider
- `SliderFloat2/3/4(label, v[], min, max, format, flags)` - X/Y/Z/W color-coded sliders
- `SliderInt2/3/4(label, v[], min, max, format, flags)`

### Image Button
- `ImageButton(str_id, texture_id, size, uv0, uv1)` - Image button with glow
- `ImageButtonWithBg(str_id, texture_id, size, uv0, uv1, bg_col, tint_col)`

---

## Quick Migration Checklist

- [ ] Upgrade compiler to C++23
- [ ] Update CMakeLists.txt with `cxx_std_23`
- [ ] Replace `Theme::Color()` with `CyberpunkTheme::Colors::Color` (optional)
- [ ] Use new component APIs (Checkbox, RadioButton, etc.)
- [ ] Update custom styles from Config to Style structs
- [ ] Test all UI components

---

## FAQ

**Q: Do I need to change all my code at once?**
A: No. Legacy APIs (v0.4.0 style) are preserved. You can migrate gradually.

**Q: What happened to GlowButtonConfig?**
A: Legacy config structs still work. New components use the StyleSystem pattern.

**Q: How do I use StyleSystem for multiple components?**
A: Use `WithStyle<T>(style, fn)` for scoped overrides:
```cpp
EFIGUI::WithStyle(myCheckboxStyle, [&] {
    EFIGUI::Checkbox("A", &a);
    EFIGUI::Checkbox("B", &b);
});
```
