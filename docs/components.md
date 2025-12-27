# Components Reference

## Windows

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

---

## Navigation

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

---

## Buttons

| Function | Description |
|----------|-------------|
| `GlowButton(label, size, glowColor, forceHover, layer, bgAlpha)` | Primary glowing button |
| `IconButton(icon, size, color, bgAlpha, uniqueId)` | Icon-only button |
| `DangerButton(label, size, layer, bgAlpha)` | Red destructive button |
| `ColoredButton(label, size, borderColor, bgAlpha, layer)` | Custom colored button |
| `CooldownButton(label, size, glowColor, cooldownProgress, layer, bgAlpha)` | Button with cooldown |

**GlowButton optional parameters:**
- `glowColor` - Glow color (default: Theme::AccentCyan)
- `layer` - Rendering layer for glow/marquee effects (default: LayerConfig::defaultWidgetGlow)
- `bgAlpha` - Background transparency 0-255 (default: uses GlassOverlay defaults)

```cpp
// Default cyan glow
EFIGUI::GlowButton("OK");

// Red glow for dangerous action
EFIGUI::GlowButton("Delete All", ImVec2(0,0), Theme::StatusError);

// Green glow for positive action
EFIGUI::GlowButton("Confirm", ImVec2(0,0), Theme::StatusSuccess);

// Custom background transparency (more transparent)
EFIGUI::GlowButton("Subtle", ImVec2(0,0), std::nullopt, false, std::nullopt, 100);

// Fully opaque background
EFIGUI::GlowButton("Solid", ImVec2(0,0), std::nullopt, false, std::nullopt, 255);
```

**IconButton optional parameters:**
- `color` - Icon color (default: Theme::TextSecondary)
- `bgAlpha` - Background alpha on hover (default: 30)
- `uniqueId` - Unique ID to avoid collision when same icon used multiple times

```cpp
// Default styling
EFIGUI::IconButton(Icons::Cog);

// Red icon with more visible hover background
EFIGUI::IconButton(Icons::Error, ImVec2(28,28), Theme::StatusError, 60);

// Multiple buttons with same icon - use uniqueId to avoid ID collision
EFIGUI::IconButton(Icons::Plus, ImVec2(28,28), std::nullopt, std::nullopt, "add_item_1");
EFIGUI::IconButton(Icons::Plus, ImVec2(28,28), std::nullopt, std::nullopt, "add_item_2");
```

**ColoredButton optional parameters:**
- `bgAlpha` - Background alpha (default: 200)

```cpp
// Cyan bordered button
EFIGUI::ColoredButton("Action", ImVec2(100,36), Theme::AccentCyan);

// Same but more transparent
EFIGUI::ColoredButton("Action", ImVec2(100,36), Theme::AccentCyan, 100);
```

**DangerButton optional parameters:**
- `layer` - Rendering layer for glow/marquee effects (default: LayerConfig::defaultWidgetGlow)
- `bgAlpha` - Background transparency 0-255 (default: uses GlassOverlay defaults)

```cpp
// Default danger button
EFIGUI::DangerButton("Delete");

// With custom background transparency
EFIGUI::DangerButton("Delete", ImVec2(0,0), std::nullopt, 150);
```

**CooldownButton optional parameters:**
- `layer` - Rendering layer for glow/marquee effects (default: LayerConfig::defaultWidgetGlow)
- `bgAlpha` - Background transparency 0-255 (default: uses GlassOverlay defaults)

```cpp
// Basic cooldown button
static float cooldown = 0.0f;
EFIGUI::CooldownButton("Fire", ImVec2(100,36), Theme::AccentCyan, cooldown);

// With custom background transparency
EFIGUI::CooldownButton("Fire", ImVec2(100,36), Theme::AccentCyan, cooldown, std::nullopt, 180);
```

---

## Glass Panel

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

---

## Input

| Function | Description |
|----------|-------------|
| `ModernToggle(label, value)` | Animated toggle switch |
| `ModernToggleWithDesc(label, desc, value)` | Toggle with description |
| `ModernInputText(label, buf, size, flags)` | Styled text input |
| `ModernInputText(label, str, flags)` | Auto-resizing string input |
| `ModernInputTextMultiline(label, buf, buf_size, size, flags)` | Multiline text input |
| `ModernInputTextMultiline(label, str, size, flags)` | Auto-resizing multiline input |
| `NumericInput(label, value, config, layer)` | Standalone editable numeric box |
| `ModernSliderFloat(label, value, min, max, format, layer)` | Float slider |
| `ModernSliderInt(label, value, min, max, format, layer)` | Integer slider |
| `ModernCombo(label, item, items, count, popupBgAlpha)` | Styled dropdown |

**ModernSliderFloat/Int optional parameters:**
- `layer` - Rendering layer for knob and input box glow effects (default: LayerConfig::defaultWidgetGlow)

```cpp
// Default glow layer
static float val = 0.5f;
EFIGUI::ModernSliderFloat("Value", &val, 0.0f, 1.0f);

// Slider with glow in popup layer (renders above popups)
EFIGUI::ModernSliderFloat("Value", &val, 0.0f, 1.0f, "%.1f", EFIGUI::Layer::PopupGlow);
```

**NumericInput:**

Standalone editable numeric value display box with glow effects. Can be used independently or composed into custom widgets.

**NumericInputConfig fields:**
- `min` - Minimum value (default: 0.0f)
- `max` - Maximum value (default: 100.0f)
- `precision` - Decimal places (default: 2)
- `width` - Input box width (default: 55.0f)
- `alignment` - Text alignment: `TextAlignment::Left`, `Center`, `Right` (default: Center)

```cpp
static float value = 50.0f;

// Basic usage with default config (text centered)
EFIGUI::NumericInputConfig config;
config.min = 0.0f;
config.max = 100.0f;
config.precision = 2;
config.width = 55.0f;
EFIGUI::NumericInput("##value", &value, config);

// Custom width, precision, and alignment
EFIGUI::NumericInputConfig wideConfig{ 0.0f, 1000.0f, 0, 80.0f, EFIGUI::TextAlignment::Right };
EFIGUI::NumericInput("##count", &value, wideConfig);

// Left-aligned text
EFIGUI::NumericInputConfig leftConfig;
leftConfig.alignment = EFIGUI::TextAlignment::Left;
EFIGUI::NumericInput("##left", &value, leftConfig);
```

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

---

## Display

| Function | Description |
|----------|-------------|
| `ModernProgressBar(fraction, size, overlay, layer)` | Progress indicator |
| `StatusIndicator(label, color, pulse, dotSize)` | Status dot with text |
| `FeatureCard(icon, name, desc, enabled, bgAlpha)` | Feature toggle card |
| `SectionHeader(label, collapsed, accentColor)` | Section divider |
| `ModernTooltip(text)` | Styled tooltip |
| `HelpMarker(text)` | Help icon with tooltip |

**ModernProgressBar optional parameters:**
- `layer` - Rendering layer for progress bar glow effects (default: LayerConfig::defaultWidgetGlow)

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

---

## Layout Helpers

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
