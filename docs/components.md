# Components Reference

Complete reference for all EFIGUI components. All components follow the v0.5.0 StyleSystem pattern.

---

## StyleSystem Pattern

All v0.5.0 components follow this pattern:
- `Component()` - Uses default style from StyleSystem
- `ComponentEx()` - Accepts custom style parameter

```cpp
// Default style
EFIGUI::Checkbox("Enable", &value);

// Custom style
CheckboxStyle style{ .checkColor = CyberpunkTheme::Colors::AccentPurple };
EFIGUI::CheckboxEx("Enable", &value, style);

// Scoped style override
EFIGUI::WithStyle(style, [&] {
    EFIGUI::Checkbox("A", &a);
    EFIGUI::Checkbox("B", &b);
});
```

---

## Windows

| Function | Description |
|----------|-------------|
| `BeginBorderlessWindow(name, p_open, flags, overlayAlpha)` | Frameless window with glassmorphism |
| `EndBorderlessWindow()` | End borderless window |
| `BeginCustomWindow(name, p_open, flags)` | Complete themed window with title bar |
| `EndCustomWindow()` | End custom window |

```cpp
// Default overlay
EFIGUI::BeginBorderlessWindow("Window", &open);

// More transparent overlay
EFIGUI::BeginBorderlessWindow("Window", &open, 0, 100);
```

---

## Navigation

| Function | Description |
|----------|-------------|
| `NavItem(icon, label, selected, ...)` | Sidebar nav item |
| `NavbarHeader(title, config)` | Nav header |
| `NavCollapseButton(collapsed, width)` | Collapse/expand button |
| `NavSectionHeader(label)` | Section divider |

```cpp
EFIGUI::NavItem(Icons::Home, "Home", isSelected);
EFIGUI::NavSectionHeader("Settings");
```

---

## Buttons

| Function | Description |
|----------|-------------|
| `GlowButton(label, size, glowColor)` | Primary glowing button |
| `IconButton(icon, size, color)` | Icon-only button |
| `DangerButton(label, size)` | Red destructive button |
| `ColoredButton(label, borderColor, size)` | Custom colored button |
| `CooldownButton(label, cooldownProgress, ...)` | Button with cooldown |
| `ImageButton(str_id, texture_id, size, uv0, uv1)` | Image button with glow |

```cpp
if (EFIGUI::GlowButton("Confirm")) {
    // Clicked
}

EFIGUI::DangerButton("Delete");
EFIGUI::ColoredButton("Custom", CyberpunkTheme::Colors::AccentPurple);

// Image button
EFIGUI::ImageButton("##icon", textureId, ImVec2(32, 32));
```

---

## Selection

| Function | Description |
|----------|-------------|
| `Checkbox(label, *v)` | Traditional checkbox with glow |
| `CheckboxEx(label, *v, style)` | Checkbox with custom style |
| `RadioButton(label, active)` | Radio button (bool version) |
| `RadioButton(label, *v, v_button)` | Radio button (int version) |
| `RadioButtonEx(...)` | Radio with custom style |
| `Selectable(label, selected, flags, size)` | Selectable list item |
| `SelectableEx(...)` | Selectable with custom style |

```cpp
static bool enabled = false;
EFIGUI::Checkbox("Enable Feature", &enabled);

static int choice = 0;
EFIGUI::RadioButton("Option A", &choice, 0);
EFIGUI::RadioButton("Option B", &choice, 1);
EFIGUI::RadioButton("Option C", &choice, 2);

if (EFIGUI::Selectable("Item 1", currentItem == 0)) {
    currentItem = 0;
}
```

---

## Popup & Modal

| Function | Description |
|----------|-------------|
| `OpenPopup(str_id, flags)` | Open popup |
| `BeginPopup(str_id, flags)` | Begin popup container |
| `EndPopup()` | End popup |
| `BeginPopupModal(name, *p_open, flags)` | Begin modal dialog |
| `EndPopupModal()` | End modal (NOT EndPopup!) |

```cpp
if (EFIGUI::GlowButton("Open Settings")) {
    EFIGUI::OpenPopup("settings_popup");
}

if (EFIGUI::BeginPopup("settings_popup")) {
    EFIGUI::Checkbox("Dark Mode", &darkMode);
    EFIGUI::EndPopup();
}

// Modal
if (EFIGUI::BeginPopupModal("Confirm Delete", &showModal)) {
    ImGui::Text("Are you sure?");
    if (EFIGUI::DangerButton("Yes, Delete")) {
        // Delete action
        ImGui::CloseCurrentPopup();
    }
    EFIGUI::EndPopupModal();
}
```

---

## Tab System

| Function | Description |
|----------|-------------|
| `BeginTabBar(str_id, flags)` | Begin tab container |
| `EndTabBar()` | End tab container |
| `BeginTabItem(label, *p_open, flags)` | Begin tab content |
| `EndTabItem()` | End tab content |
| `TabItem(label)` | Simple tab button (no content) |

```cpp
if (EFIGUI::BeginTabBar("MainTabs")) {
    if (EFIGUI::BeginTabItem("General")) {
        // General settings content
        EFIGUI::EndTabItem();
    }
    if (EFIGUI::BeginTabItem("Advanced")) {
        // Advanced settings content
        EFIGUI::EndTabItem();
    }
    EFIGUI::EndTabBar();
}
```

---

## Tree & Collapsing

| Function | Description |
|----------|-------------|
| `TreeNode(label)` | Expandable tree node |
| `TreePop()` | End tree node scope |
| `CollapsingHeader(label, flags)` | Collapsible section header |

```cpp
if (EFIGUI::TreeNode("Parent")) {
    EFIGUI::Checkbox("Child Option", &childOpt);
    if (EFIGUI::TreeNode("Nested")) {
        // Nested content
        EFIGUI::TreePop();
    }
    EFIGUI::TreePop();
}

if (EFIGUI::CollapsingHeader("Advanced Options")) {
    // Content shown when expanded
}
```

---

## Table

| Function | Description |
|----------|-------------|
| `BeginTable(str_id, columns, flags, size)` | Begin table |
| `EndTable()` | End table |
| `TableSetupColumn(label, flags, width)` | Setup column |
| `TableHeadersRow()` | Create header row |
| `TableNextRow(flags, height)` | Move to next row |
| `TableNextColumn()` | Move to next column |
| `TableSetColumnIndex(column)` | Set specific column |

```cpp
if (EFIGUI::BeginTable("data", 3)) {
    EFIGUI::TableSetupColumn("Name");
    EFIGUI::TableSetupColumn("Value");
    EFIGUI::TableSetupColumn("Actions");
    EFIGUI::TableHeadersRow();

    for (auto& item : items) {
        EFIGUI::TableNextRow();
        EFIGUI::TableNextColumn();
        ImGui::Text("%s", item.name);
        EFIGUI::TableNextColumn();
        ImGui::Text("%.2f", item.value);
        EFIGUI::TableNextColumn();
        if (EFIGUI::GlowButton("Edit")) { /* ... */ }
    }
    EFIGUI::EndTable();
}
```

---

## ListBox

| Function | Description |
|----------|-------------|
| `BeginListBox(label, size)` | Begin manual list |
| `EndListBox()` | End list |
| `ListBox(label, *current, items, count, height)` | Simple list |

```cpp
// Manual rendering
if (EFIGUI::BeginListBox("##list", ImVec2(200, 150))) {
    for (int i = 0; i < itemCount; i++) {
        if (EFIGUI::Selectable(items[i], selected == i)) {
            selected = i;
        }
    }
    EFIGUI::EndListBox();
}

// Simple array-based
const char* items[] = { "Item 1", "Item 2", "Item 3" };
static int current = 0;
EFIGUI::ListBox("Select", &current, items, 3);
```

---

## Menu

| Function | Description |
|----------|-------------|
| `BeginMainMenuBar()` | Main menu bar |
| `EndMainMenuBar()` | End main menu |
| `BeginMenuBar()` | Window menu bar |
| `EndMenuBar()` | End window menu |
| `BeginMenu(label, enabled)` | Menu dropdown |
| `EndMenu()` | End menu |
| `MenuItem(label, shortcut, selected, enabled)` | Menu item |
| `MenuSeparator()` | Menu separator |
| `MenuHeader(label)` | Menu section header |

```cpp
if (EFIGUI::BeginMainMenuBar()) {
    if (EFIGUI::BeginMenu("File")) {
        if (EFIGUI::MenuItem("New", "Ctrl+N")) { /* ... */ }
        if (EFIGUI::MenuItem("Open", "Ctrl+O")) { /* ... */ }
        EFIGUI::MenuSeparator();
        if (EFIGUI::MenuItem("Exit")) { /* ... */ }
        EFIGUI::EndMenu();
    }
    if (EFIGUI::BeginMenu("Edit")) {
        if (EFIGUI::MenuItem("Undo", "Ctrl+Z")) { /* ... */ }
        if (EFIGUI::MenuItem("Redo", "Ctrl+Y")) { /* ... */ }
        EFIGUI::EndMenu();
    }
    EFIGUI::EndMainMenuBar();
}
```

---

## Color

| Function | Description |
|----------|-------------|
| `ColorEdit3(label, col, flags)` | RGB color editor |
| `ColorEdit4(label, col, flags)` | RGBA color editor |
| `ColorPicker3(label, col, flags)` | RGB color picker |
| `ColorPicker4(label, col, flags)` | RGBA color picker |
| `ColorButton(desc_id, col, flags, size)` | Color swatch button |

```cpp
static float color3[3] = {1.0f, 0.5f, 0.0f};
static float color4[4] = {0.0f, 0.8f, 1.0f, 1.0f};

EFIGUI::ColorEdit3("Ambient", color3);
EFIGUI::ColorEdit4("Diffuse", color4);

// Full picker
EFIGUI::ColorPicker4("Pick Color", color4);

// Clickable swatch
if (EFIGUI::ColorButton("##swatch", ImVec4(color4[0], color4[1], color4[2], color4[3]))) {
    // Open picker popup
}
```

---

## Drag Input

| Function | Description |
|----------|-------------|
| `DragFloat(label, *v, speed, min, max, format, flags)` | Drag float |
| `DragFloat2/3/4(label, v[], ...)` | Drag float vector |
| `DragInt(label, *v, speed, min, max, format, flags)` | Drag int |
| `DragInt2/3/4(label, v[], ...)` | Drag int vector |
| `DragFloatRange2(label, *v_min, *v_max, ...)` | Float range |
| `DragIntRange2(label, *v_min, *v_max, ...)` | Int range |

```cpp
static float speed = 1.0f;
EFIGUI::DragFloat("Speed", &speed, 0.1f, 0.0f, 100.0f, "%.1f");

static float pos[3] = {0, 0, 0};
EFIGUI::DragFloat3("Position", pos, 0.1f);

static float minVal = 0.0f, maxVal = 100.0f;
EFIGUI::DragFloatRange2("Range", &minVal, &maxVal, 0.5f, 0.0f, 200.0f);
```

---

## Vertical Slider

| Function | Description |
|----------|-------------|
| `VSliderFloat(label, size, *v, min, max, format, flags)` | Vertical float slider |
| `VSliderInt(label, size, *v, min, max, format, flags)` | Vertical int slider |

```cpp
static float volume = 0.5f;
EFIGUI::VSliderFloat("##volume", ImVec2(30, 150), &volume, 0.0f, 1.0f, "%.2f");

static int level = 50;
EFIGUI::VSliderInt("##level", ImVec2(30, 150), &level, 0, 100);
```

---

## Vector Input

| Function | Description |
|----------|-------------|
| `InputFloat2/3/4(label, v[], format, flags)` | Float vector input |
| `InputInt2/3/4(label, v[], flags)` | Int vector input |
| `SliderFloat2/3/4(label, v[], min, max, format, flags)` | Float vector slider |
| `SliderInt2/3/4(label, v[], min, max, format, flags)` | Int vector slider |

Vector inputs feature X/Y/Z/W color coding (Red/Green/Blue/Yellow).

```cpp
static float position[3] = {0, 0, 0};
static float scale[3] = {1, 1, 1};
static int gridSize[2] = {10, 10};

EFIGUI::InputFloat3("Position", position);
EFIGUI::SliderFloat3("Scale", scale, 0.1f, 10.0f);
EFIGUI::InputInt2("Grid Size", gridSize);
```

---

## Plot

| Function | Description |
|----------|-------------|
| `PlotLines(label, values, count, overlay, min, max, size)` | Line chart |
| `PlotHistogram(label, values, count, overlay, min, max, size)` | Bar chart |

Both support callback versions for dynamic data.

```cpp
static float data[100];
// Fill data...

EFIGUI::PlotLines("Signal", data, 100, "FPS", 0.0f, 120.0f, ImVec2(0, 80));
EFIGUI::PlotHistogram("Usage", data, 100, "CPU %", 0.0f, 100.0f, ImVec2(0, 80));
```

---

## Glass Panel

| Function | Description |
|----------|-------------|
| `BeginGlassPanel(id, size, border, bgAlpha, glowColor)` | Semi-transparent panel |
| `EndGlassPanel()` | End panel |

```cpp
EFIGUI::BeginGlassPanel("panel1", ImVec2(300, 200));
// Panel content...
EFIGUI::EndGlassPanel();

// Custom styling
EFIGUI::BeginGlassPanel("panel2", ImVec2(300, 200), true, 128,
                         CyberpunkTheme::Colors::AccentPurple);
// Content...
EFIGUI::EndGlassPanel();
```

---

## Toggle

| Function | Description |
|----------|-------------|
| `ModernToggle(label, *value)` | Animated toggle switch |
| `ModernToggleWithDesc(label, desc, *value)` | Toggle with description |

```cpp
static bool enabled = false;
EFIGUI::ModernToggle("Dark Mode", &enabled);
EFIGUI::ModernToggleWithDesc("Auto Save", "Save changes automatically", &autoSave);
```

---

## Text Input

| Function | Description |
|----------|-------------|
| `ModernInputText(label, buf, size, flags)` | Styled text input |
| `ModernInputText(label, &str, flags)` | Auto-resizing string input |
| `ModernInputTextMultiline(...)` | Multiline text input |
| `NumericInput(label, *value, config)` | Editable numeric box |

```cpp
static char buffer[256] = "";
EFIGUI::ModernInputText("Name", buffer, sizeof(buffer));

static std::string text;
EFIGUI::ModernInputText("Description", &text);
```

---

## Slider

| Function | Description |
|----------|-------------|
| `ModernSliderFloat(label, *v, min, max, format)` | Float slider |
| `ModernSliderInt(label, *v, min, max, format)` | Int slider |

```cpp
static float volume = 0.5f;
static int count = 10;

EFIGUI::ModernSliderFloat("Volume", &volume, 0.0f, 1.0f, "%.2f");
EFIGUI::ModernSliderInt("Count", &count, 0, 100);
```

---

## Combo

| Function | Description |
|----------|-------------|
| `ModernCombo(label, *current, items, count)` | Styled dropdown |

```cpp
const char* items[] = { "Low", "Medium", "High", "Ultra" };
static int quality = 2;
EFIGUI::ModernCombo("Quality", &quality, items, 4);
```

---

## Display

| Function | Description |
|----------|-------------|
| `ModernProgressBar(fraction, size, overlay)` | Progress indicator |
| `StatusIndicator(label, color, pulse, dotSize)` | Status dot with text |
| `FeatureCard(icon, name, desc, *enabled)` | Feature toggle card |
| `SectionHeader(label, *collapsed, accentColor)` | Section divider |
| `ModernTooltip(text)` | Styled tooltip |
| `HelpMarker(text)` | Help icon with tooltip |

```cpp
EFIGUI::ModernProgressBar(0.65f, ImVec2(0, 0), "65%");

EFIGUI::StatusIndicator("Online", CyberpunkTheme::Colors::StatusSuccess, true);
EFIGUI::StatusIndicator("Offline", CyberpunkTheme::Colors::StatusError, false);

static bool shieldEnabled = false;
EFIGUI::FeatureCard(Icons::Shield, "Shield Mode", "Enable protection", &shieldEnabled);

EFIGUI::SectionHeader("Advanced Options");
EFIGUI::HelpMarker("This setting controls...");
```

---

## Layout Helpers

| Function | Description |
|----------|-------------|
| `Spacing(height)` | Vertical space |
| `Separator(color)` | Horizontal line |
| `SameLine(offset, spacing)` | Same line layout |

```cpp
EFIGUI::GlowButton("Button 1");
EFIGUI::SameLine();
EFIGUI::GlowButton("Button 2");

EFIGUI::Spacing(20.0f);
EFIGUI::Separator();
EFIGUI::Separator(CyberpunkTheme::Colors::AccentCyan);
```
