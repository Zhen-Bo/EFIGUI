# Components Reference

> **v0.4.0 Update**: All Config structs now support fluent builder patterns. See [Migration Guide](migration-0.4.0.md) for details.

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
| `NavItem(icon, label, selected, config, width, collapsed)` | Nav item with config (v0.3.1+) |
| `NavbarHeader(title, config)` | Nav header (Config struct) |
| `NavbarHeader(title, iconExp, iconCol, collapsed, width, closeClicked, glowColor)` | Nav header (legacy) |
| `NavCollapseButton(collapsed, width)` | Collapse/expand button |
| `NavSectionHeader(label)` | Section divider |

### NavItem with Config (v0.3.1+)

```cpp
// Custom nav item configuration
EFIGUI::NavItemConfig config;
config.height = 48.0f;                      // Taller item
config.accentColor = Theme::AccentPurple(); // Purple accent bar
config.rounding = 12.0f;                    // More rounded

EFIGUI::NavItem(Icons::Home, "Home", isSelected, config);
```

**NavItemConfig fields:**
- `height` - Item height (default: Theme::Nav().itemHeight)
- `iconPadding` - Icon padding (default: Theme::Nav().iconPadding)
- `labelSpacing` - Label spacing (default: Theme::Nav().labelSpacing)
- `accentBarWidth` - Accent bar width (default: Theme::Nav().accentBarWidth)
- `rounding` - Corner rounding
- `accentColor` - Accent bar color
- `bgColor` - Background color
- `collapsedIconScale` - Icon scale when collapsed

**NavItem optional parameters:**
- `accentColor` - Highlight color when selected/hovered (default: Theme::AccentCyan())

```cpp
// Default cyan accent
EFIGUI::NavItem(Icons::Home, "Home", selected);

// Custom purple accent
EFIGUI::NavItem(Icons::Home, "Home", selected,
                Theme::NavbarWidth(), false, Theme::AccentPurple());
```

**NavbarHeader (Config struct - recommended):**

```cpp
// Using NavbarHeaderConfig struct
EFIGUI::NavbarHeaderConfig headerConfig;
headerConfig.iconExpanded = Icons::Bars;
headerConfig.iconCollapsed = Icons::Cog;
headerConfig.collapsed = isCollapsed;
headerConfig.width = navbarWidth;
headerConfig.glowColor = Theme::AccentPurple();

if (EFIGUI::NavbarHeader("My App", headerConfig))
{
    isCollapsed = !isCollapsed;  // Toggle collapse state
}
```

**NavbarHeaderConfig fields:**
- `iconExpanded` - Icon shown when expanded (default: "<<")
- `iconCollapsed` - Icon shown when collapsed (default: ">>")
- `collapsed` - Current collapse state (default: false)
- `width` - Header width, 0 = Theme::NavbarWidth() (default: 0)
- `closeClicked` - Output pointer for close button click (default: nullptr)
- `glowColor` - Glow color for icon (default: Theme::AccentCyan())

---

## Buttons

| Function | Description |
|----------|-------------|
| `GlowButton(label, config)` | Primary glowing button (ButtonConfig - v0.4.0+) |
| `GlowButton(label, config)` | Primary glowing button (GlowButtonConfig) |
| `GlowButton(label, size, glowColor, forceHover, layer, bgAlpha)` | Primary glowing button (legacy) |
| `IconButton(icon, size, color, bgAlpha, uniqueId)` | Icon-only button |
| `DangerButton(label, config)` | Red destructive button (ButtonConfig - v0.4.0+) |
| `DangerButton(label, size, layer, bgAlpha)` | Red destructive button (legacy) |
| `ColoredButton(label, borderColor, config)` | Custom colored button (ButtonConfig - v0.4.0+) |
| `ColoredButton(label, size, borderColor, bgAlpha, layer)` | Custom colored button (legacy) |
| `CooldownButton(label, config)` | Button with cooldown (Config struct) |
| `CooldownButton(label, size, glowColor, cooldownProgress, layer, bgAlpha)` | Button with cooldown (legacy) |

### GlowButton

**GlowButton with ButtonConfig (v0.4.0+ - recommended):**

```cpp
// Using ButtonConfig with builder pattern
if (EFIGUI::GlowButton("Confirm", EFIGUI::ButtonConfig()
        .withSize(120, 36)
        .withGlowColor(Theme::StatusSuccess())
        .withRounding(12.0f)
        .withPadding(EFIGUI::EdgeInsets::Symmetric(8, 24))))
{
    // Button clicked
}

// ButtonConfig with full customization
EFIGUI::ButtonConfig cfg;
cfg.size = ImVec2(150, 40);
cfg.glowColor = Theme::AccentPurple();
cfg.rounding = 16.0f;
cfg.glowLayers = 6;
cfg.glowExpand = 3.0f;
cfg.forceHover = true;
EFIGUI::GlowButton("Custom", cfg);
```

**ButtonConfig fields:**
- `size` - Button size, ImVec2(0,0) = auto-size (default: ImVec2(0,0))
- `padding` - Padding around text (default: Theme::Button().padding)
- `rounding` - Corner rounding (default: Theme::Button().rounding)
- `minWidth` - Minimum button width (default: 0)
- `bgColor` - Background color (default: Theme)
- `bgHoverColor` - Hover background color
- `bgActiveColor` - Active/pressed background color
- `textColor` - Text color
- `glowColor` - Glow color (default: Theme::AccentCyan())
- `bgAlpha` - Background transparency 0-255
- `glowLayers` - Number of glow layers (default: 5)
- `glowExpand` - Glow expansion per layer (default: 2.0)
- `glowIntensity` - Glow intensity
- `forceHover` - Always show hover animation (default: false)
- `layer` - Rendering layer for glow/marquee effects

**GlowButton (GlowButtonConfig version):**

```cpp
// Using GlowButtonConfig struct
EFIGUI::GlowButtonConfig config;
config.size = ImVec2(120, 36);
config.glowColor = Theme::StatusSuccess();
config.forceHover = false;
config.bgAlpha = 180;

if (EFIGUI::GlowButton("Confirm", config))
{
    // Button clicked
}

// Compact initialization
EFIGUI::GlowButtonConfig dangerConfig{ ImVec2(100, 36), Theme::StatusError(), true };
EFIGUI::GlowButton("Delete", dangerConfig);
```

**GlowButtonConfig fields:**
- `size` - Button size, ImVec2(0,0) = auto-size (default: ImVec2(0,0))
- `glowColor` - Glow color (default: Theme::AccentCyan())
- `forceHover` - Always show hover animation (default: false)
- `layer` - Rendering layer for glow/marquee effects (default: LayerConfig default)
- `bgAlpha` - Background transparency 0-255 (default: GlassOverlay defaults)

**GlowButton (legacy API):**

```cpp
// Default cyan glow
EFIGUI::GlowButton("OK");

// Red glow for dangerous action
EFIGUI::GlowButton("Delete All", ImVec2(0,0), Theme::StatusError());

// Green glow for positive action
EFIGUI::GlowButton("Confirm", ImVec2(0,0), Theme::StatusSuccess());

// Custom background transparency (more transparent)
EFIGUI::GlowButton("Subtle", ImVec2(0,0), std::nullopt, false, std::nullopt, 100);

// Fully opaque background
EFIGUI::GlowButton("Solid", ImVec2(0,0), std::nullopt, false, std::nullopt, 255);
```

**IconButton optional parameters:**
- `color` - Icon color (default: Theme::TextSecondary())
- `bgAlpha` - Background alpha on hover (default: 30)
- `uniqueId` - Unique ID to avoid collision when same icon used multiple times

```cpp
// Default styling
EFIGUI::IconButton(Icons::Cog);

// Red icon with more visible hover background
EFIGUI::IconButton(Icons::Error, ImVec2(28,28), Theme::StatusError(), 60);

// Multiple buttons with same icon - use uniqueId to avoid ID collision
EFIGUI::IconButton(Icons::Plus, ImVec2(28,28), std::nullopt, std::nullopt, "add_item_1");
EFIGUI::IconButton(Icons::Plus, ImVec2(28,28), std::nullopt, std::nullopt, "add_item_2");
```

**ColoredButton optional parameters:**
- `bgAlpha` - Background alpha (default: 200)

```cpp
// Cyan bordered button
EFIGUI::ColoredButton("Action", ImVec2(100,36), Theme::AccentCyan());

// Same but more transparent
EFIGUI::ColoredButton("Action", ImVec2(100,36), Theme::AccentCyan(), 100);
```

**DangerButton with ButtonConfig (v0.4.0+):**

```cpp
// DangerButton with full customization
if (EFIGUI::DangerButton("Delete", EFIGUI::ButtonConfig()
        .withSize(100, 36)
        .withRounding(8.0f)))
{
    // Delete action
}
```

**DangerButton optional parameters (legacy):**
- `layer` - Rendering layer for glow/marquee effects (default: LayerConfig::defaultWidgetGlow)
- `bgAlpha` - Background transparency 0-255 (default: uses GlassOverlay defaults)

```cpp
// Default danger button
EFIGUI::DangerButton("Delete");

// With custom background transparency
EFIGUI::DangerButton("Delete", ImVec2(0,0), std::nullopt, 150);
```

**ColoredButton with ButtonConfig (v0.4.0+):**

```cpp
// ColoredButton with full customization
if (EFIGUI::ColoredButton("Custom", Theme::AccentPurple(), EFIGUI::ButtonConfig()
        .withSize(150, 40)
        .withGlowLayers(6)
        .withGlowExpand(3.0f)))
{
    // Action
}
```

**CooldownButton (Config struct - recommended):**

```cpp
// Using CooldownButtonConfig struct
static float cooldownProgress = 0.0f;

EFIGUI::CooldownButtonConfig config;
config.size = ImVec2(120, 36);
config.glowColor = Theme::AccentPurple();
config.cooldownProgress = cooldownProgress;
config.bgAlpha = 200;

if (EFIGUI::CooldownButton("Ability", config))
{
    cooldownProgress = 1.0f;  // Start cooldown
}

// Update cooldown over time
cooldownProgress = std::max(0.0f, cooldownProgress - deltaTime);
```

**CooldownButtonConfig fields:**
- `size` - Button size, ImVec2(0,0) = auto-size (default: ImVec2(0,0))
- `glowColor` - Glow color (default: cyan IM_COL32(0, 245, 255, 255))
- `cooldownProgress` - 0.0 = ready, 1.0 = full cooldown (default: 0.0f)
- `layer` - Rendering layer for glow/marquee effects (default: LayerConfig default)
- `bgAlpha` - Background transparency 0-255 (default: GlassOverlay defaults)

**CooldownButton (legacy API):**

```cpp
// Basic cooldown button
static float cooldown = 0.0f;
EFIGUI::CooldownButton("Fire", ImVec2(100,36), Theme::AccentCyan(), cooldown);

// With custom background transparency
EFIGUI::CooldownButton("Fire", ImVec2(100,36), Theme::AccentCyan(), cooldown, std::nullopt, 180);
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
                         128, Theme::AccentPurple());
// content...
EFIGUI::EndGlassPanel();
```

---

## Input

| Function | Description |
|----------|-------------|
| `ModernToggle(label, value)` | Animated toggle switch |
| `ModernToggle(label, value, config)` | Toggle with config (v0.3.1+) |
| `ModernToggleWithDesc(label, desc, value)` | Toggle with description |
| `ModernToggleWithDesc(label, desc, value, config)` | Toggle with description and config (v0.3.1+) |
| `ModernInputText(label, buf, size, flags)` | Styled text input |
| `ModernInputText(label, str, flags)` | Auto-resizing string input |
| `ModernInputTextMultiline(label, buf, buf_size, size, flags)` | Multiline text input |
| `ModernInputTextMultiline(label, str, size, flags)` | Auto-resizing multiline input |
| `NumericInput(label, value, config, layer)` | Standalone editable numeric box |
| `ModernSliderFloat(label, value, min, max, format, layer)` | Float slider |
| `ModernSliderFloat(label, value, min, max, format, config, layer)` | Float slider with config (v0.3.1+) |
| `ModernSliderInt(label, value, min, max, format, layer)` | Integer slider |
| `ModernSliderInt(label, value, min, max, format, config, layer)` | Integer slider with config (v0.3.1+) |
| `ModernCombo(label, item, items, count, popupBgAlpha)` | Styled dropdown |

### ModernSliderFloat/Int with Config (v0.3.1+)

Per-instance configuration allows customizing individual sliders:

```cpp
// Using builder pattern (v0.4.0+ - recommended)
static float val = 0.5f;
EFIGUI::ModernSliderFloat("Custom", &val, 0.0f, 1.0f, "%.2f", std::nullopt,
    EFIGUI::SliderConfig()
        .withHeight(32.0f)
        .withKnobRadius(12.0f)
        .withPadding(EFIGUI::EdgeInsets::Symmetric(4, 8))
        .withGlowColor(Theme::AccentPurple()));

// Create a custom slider config (legacy style)
EFIGUI::SliderConfig config;
config.height = 32.0f;              // Taller slider
config.knobRadius = 12.0f;          // Larger knob
config.glowColor = Theme::AccentPurple();  // Purple glow

EFIGUI::ModernSliderFloat("Custom", &val, 0.0f, 1.0f, "%.2f", std::nullopt, config);

// Use FromTheme() to start from defaults and modify
auto scaledConfig = EFIGUI::SliderConfig::FromTheme();
scaledConfig.height = scaledConfig.height.value() * 1.5f;  // 50% larger
EFIGUI::ModernSliderFloat("Scaled", &val, 0.0f, 1.0f, "%.2f", std::nullopt, scaledConfig);
```

**SliderConfig fields:**
- `height` - Overall slider height (default: Theme::Slider().height)
- `trackHeight` - Track thickness (default: Theme::Slider().trackHeight)
- `knobRadius` - Knob radius (default: Theme::Slider().knobRadius)
- `inputWidth` - Input box width (default: Theme::Slider().inputWidth)
- `inputHeight` - Input box height (default: Theme::Slider().inputHeight)
- `labelGap` - Gap between label and slider (default: Theme::Slider().labelGap)
- `padding` - Padding around slider area (v0.4.0+, `EdgeInsets`)
- `trackColor` - Track background color (v0.4.0+)
- `fillColor` - Track fill color (v0.4.0+)
- `knobColor` - Knob color (v0.4.0+)
- `glowColor` - Glow effect color (default: Theme::AccentCyan())
- `showInput` - Show numeric input box (default: true)
- `layer` - Rendering layer for glow effects

### ModernToggle with Config (v0.3.1+)

```cpp
// Using builder pattern (v0.4.0+ - recommended)
static bool enabled = false;
EFIGUI::ModernToggle("Feature", &enabled, EFIGUI::ToggleConfig()
    .withWidth(56.0f)
    .withHeight(28.0f)
    .withMargin(EFIGUI::EdgeInsets::All(4))
    .withGlowColor(Theme::StatusSuccess()));

// Custom toggle configuration (legacy style)
EFIGUI::ToggleConfig config;
config.width = 56.0f;               // Wider toggle
config.height = 28.0f;              // Taller toggle
config.glowColor = Theme::StatusSuccess();  // Green glow

EFIGUI::ModernToggle("Feature", &enabled, config);
```

**ToggleConfig fields:**
- `width` - Toggle width (default: Theme::Toggle().width)
- `height` - Toggle height (default: Theme::Toggle().height)
- `knobSize` - Knob diameter (default: Theme::Toggle().knobSize)
- `labelGap` - Gap between toggle and label
- `margin` - Margin around toggle (v0.4.0+, `EdgeInsets`)
- `trackOnColor` - Track color when on (v0.4.0+)
- `trackOffColor` - Track color when off (v0.4.0+)
- `knobColor` - Knob color (v0.4.0+)
- `glowColor` - Glow effect color
- `animSpeed` - Animation speed
- `glowRadius` - Glow radius
- `disabled` - Disable interaction (default: false)

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
| `ModernProgressBar(fraction, size, overlay, config)` | Progress with config (v0.3.1+) |
| `StatusIndicator(label, color, pulse, dotSize)` | Status dot with text |
| `FeatureCard(icon, name, desc, enabled, bgAlpha)` | Feature toggle card |
| `FeatureCard(icon, name, desc, enabled, config)` | Feature card with config (v0.3.1+) |
| `SectionHeader(label, collapsed, accentColor)` | Section divider |
| `ModernTooltip(text)` | Styled tooltip |
| `HelpMarker(text)` | Help icon with tooltip |

### FeatureCard with Config (v0.3.1+)

```cpp
// Using builder pattern (v0.4.0+ - recommended)
static bool featureOn = false;
EFIGUI::FeatureCard(Icons::Shield, "Shield", "Protection mode", &featureOn,
    EFIGUI::CardConfig()
        .withPadding(EFIGUI::EdgeInsets::Symmetric(12, 16))
        .withRounding(12.0f)
        .withBgAlpha(180));

// Custom feature card configuration (legacy style)
EFIGUI::CardConfig config;
config.padding = EFIGUI::EdgeInsets(15, 16, 15, 16);  // v0.4.0+
config.rounding = 12.0f;            // v0.4.0+
config.iconSize = 40.0f;            // Larger icon
config.bgAlpha = 180;               // Custom transparency

EFIGUI::FeatureCard(Icons::Shield, "Shield", "Protection mode", &featureOn, config);
```

**CardConfig fields:**
- `padding` - Padding inside card (v0.4.0+, `EdgeInsets`)
- `margin` - Margin around card (v0.4.0+, `EdgeInsets`)
- `rounding` - Corner rounding (v0.4.0+)
- `titleDescGap` - Gap between title and description (v0.3.2+)
- `bgColor` - Background color (v0.4.0+)
- `borderColor` - Border color (v0.4.0+)
- `titleColor` - Title text color (v0.4.0+)
- `descColor` - Description text color (v0.4.0+)
- `topPadding` - Space above title (deprecated, use `padding.top`)
- `bottomPadding` - Space below description (deprecated, use `padding.bottom`)
- `height` - Card height (deprecated, use semantic padding instead)
- `iconSize` - Icon size (default: Theme::Card().iconSize)
- `iconPadding` - Icon padding (default: Theme::Card().iconPadding)
- `toggleWidth` - Embedded toggle width
- `toggleHeight` - Embedded toggle height
- `bgAlpha` - Background transparency (0-255)

**ModernProgressBar optional parameters:**
- `layer` - Rendering layer for progress bar glow effects (default: LayerConfig::defaultWidgetGlow)

**StatusIndicator optional parameters:**
- `dotSize` - Size of the status dot (default: 8.0f)

```cpp
// Default 8px dot
EFIGUI::StatusIndicator("Connected", Theme::StatusSuccess(), true);

// Larger 14px dot
EFIGUI::StatusIndicator("Connected", Theme::StatusSuccess(), true, 14.0f);
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
EFIGUI::SectionHeader("Settings", nullptr, Theme::AccentPurple());
```

---

## Layout Helpers

| Function | Description |
|----------|-------------|
| `Spacing(height)` | Vertical space |
| `Separator(color)` | Horizontal line |
| `SameLine(offset, spacing)` | Same line layout |

**Separator optional parameters:**
- `color` - Line color (default: Theme::BorderDefault())

```cpp
// Default gray separator
EFIGUI::Separator();

// Cyan separator
EFIGUI::Separator(Theme::AccentCyan());
```
