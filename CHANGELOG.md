# Changelog

All notable changes to EFIGUI will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [0.4.0] - 2026-01-05

### Added

#### Core Layout Types
- **`EdgeInsets`** - Unified padding/margin type with named constructors (`All`, `Symmetric`, `Horizontal`, `Vertical`, `Only`) and utility methods
- **`Spacing`** - Represents spacing between elements (horizontal and vertical)
- **`CornerRadius`** - Per-corner rounding with presets (`All`, `Top`, `Bottom`, `Pill`, `None`)
- **`BorderStyle`** - Border width and color configuration
- **`SizeConstraints`** - Min/max size constraints with named constructors (`MinSize`, `MaxSize`, `Fixed`)

#### Builder Pattern for Config Structs
All Config structs now support fluent builder pattern for easier configuration:
```cpp
// Before (v0.3.x)
SliderConfig cfg;
cfg.height = 32.0f;
cfg.glowColor = Theme::StatusError();
ModernSliderFloat("Value", &val, 0, 100, "%.0f", std::nullopt, cfg);

// After (v0.4.0)
ModernSliderFloat("Value", &val, 0, 100, "%.0f", std::nullopt,
    SliderConfig().withHeight(32.0f).withGlowColor(Theme::StatusError()));
```

#### New Config Struct: ButtonConfig
- Full customization support for buttons including size, padding, rounding, colors, glow effects
- New function overloads: `GlowButton(label, ButtonConfig)`, `DangerButton(label, ButtonConfig)`, `ColoredButton(label, borderColor, ButtonConfig)`

#### Enhanced Theme Structs
- **ButtonTheme**: Added `padding` (EdgeInsets), `rounding`, `minWidth`, color properties (`bgDefault`, `bgHover`, `bgActive`, `textColor`, `glowColor`), and glow configuration
- **SliderTheme**: Added `padding` (EdgeInsets), `labelGap`, `trackRounding`, track and knob color properties
- **ToggleTheme**: Added `margin` (EdgeInsets), color properties (`trackOnColor`, `trackOffColor`, `knobColor`, `glowColor`)
- **CardTheme**: Added `padding` and `margin` (EdgeInsets), `rounding`, `contentSpacing`, color properties (`bgColor`, `borderColor`, `titleColor`, `descColor`)
- **NavTheme**: Added `itemPadding` (EdgeInsets), `itemHeight`, `itemRounding`, `sectionPadding` (EdgeInsets), color properties
- **WindowTheme**: Added `padding` (EdgeInsets), `rounding`, `titleBarPadding` (EdgeInsets), color properties
- **LayoutTheme**: Added `itemSpacing` and `innerSpacing` (Spacing), `contentPadding` (EdgeInsets), `separatorThickness`, tooltip configuration with colors

#### Config Struct Enhancements
- Added `padding` and/or `margin` fields (using `std::optional<EdgeInsets>`) to: `SliderConfig`, `ToggleConfig`, `CardConfig`, `NavItemConfig`, `ButtonConfig`, `WindowConfig`, `PanelConfig`, `ProgressConfig`, `InputConfig`
- Added builder methods to all Config structs (e.g., `withPadding()`, `withRounding()`, `withGlowColor()`)

#### Build System
- **ImGui include path auto-detection**: CMake now automatically detects ImGui headers via `IMGUI_INCLUDE_DIR` variable or local `imgui/` folder
- **Unit test infrastructure**: Added `EFIGUI_BUILD_TESTS` CMake option and test subdirectory support

#### Safety Improvements
- **Null device pointer validation**: `Init()` now validates device pointer for hardware backends with assert (debug) and graceful fallback (release)

#### Unit Testing
- **doctest integration**: Added doctest header-only testing framework
- **ThemeConfig tests**: Unit tests for `EdgeInsets`, `Spacing`, `CornerRadius`, `BorderStyle`, `SizeConstraints`, and `Resolve` helper

### Changed
- Theme structs now use `EdgeInsets` for padding/margin instead of separate float values (legacy accessors preserved for backward compatibility)
- `ButtonTheme.paddingX` deprecated in favor of `ButtonTheme.padding`
- **Button API unification**: Legacy `GlowButton`, `ColoredButton`, and `GlowButtonConfig` overloads now delegate to `ButtonConfig` version, eliminating code duplication and ensuring consistent behavior

### Removed
- **Empty local namespaces**: Removed unused `ButtonLocal` and `WindowLocal` namespaces from Button.cpp and Window.cpp
- **Redundant code**: Simplified `RectGradientH` function by removing unnecessary if/else branch with identical content
- **Non-existent API references**: Removed documentation references to `ApplyRoundingStyle()` and `ApplySpeedPreset()` methods that were never implemented

### Breaking Changes
This is a major refactoring release. While legacy APIs are preserved for backward compatibility, the following may require attention:
- Theme struct field organization has changed (fields grouped by category)
- Some internal implementations now use `EdgeInsets` instead of individual float values

---

## [0.3.2] - 2025-12-29

### Fixed
- **FeatureCard height inconsistency at high DPI scales**: Fixed issue where card bottom margins were inconsistent at 150%/200% UI scale. Root cause was `CalcTextSize` returning different heights than `CalcWordWrapPositionA` used for actual rendering.

### Added
- **Semantic padding parameters for CardTheme** (recommended for new code):
  - `topPadding` - Space above title (default: 10.0f)
  - `bottomPadding` - Space below description (default: 12.0f)
  - `titleDescGap` - Gap between title and description (default: 2.0f)
- **CardConfig semantic fields**: Corresponding `std::optional<float>` fields in `CardConfig` for per-instance override.

### Changed
- **CardTheme.baseHeight default**: Changed from `56.0f` to `0.0f` to enable semantic height calculation by default.
- **Dual-path height calculation**: When `baseHeight > 0`, uses legacy fixed-height calculation for backward compatibility. When `baseHeight == 0` (new default), uses semantic padding-based calculation with `CalcWordWrapPositionA` for consistent line counting.

### Deprecated
- `CardTheme.baseHeight`, `nameOffsetY`, `descOffsetY` - These legacy parameters are kept for backward compatibility but semantic padding parameters are recommended for new code.

### Note
No action required for most users. The new semantic calculation is enabled by default and provides consistent card heights at all DPI scales.

To use legacy behavior:
```cpp
EFIGUI::Theme::CardMut().baseHeight = 56.0f;  // Restore legacy fixed-height mode
```

---

## [0.3.1] - 2025-12-29

### Added
- **Per-Instance Config Structures**: New config structs for per-instance customization of components using `std::optional` fields for selective override of Theme defaults.
  - `SliderConfig` - Slider dimensions, colors, and glow settings
  - `ToggleConfig` - Toggle dimensions, colors, and animation settings
  - `CardConfig` - Card dimensions and inline toggle settings
  - `NavItemConfig` - Navigation item dimensions and accent colors
  - `WindowConfig` - Title bar settings and overlay alpha
  - `PanelConfig` - Panel rounding, border, and glow settings
  - `ProgressConfig` - Progress bar dimensions and glow settings
  - `InputConfig` - Input field rounding and colors
- **FromTheme() Static Methods**: Each config struct has a `FromTheme()` method to create a config with all Theme defaults filled in, enabling easy modification of specific values.
- **Animation Smooth Transition Helpers**: New structures for dynamic real-time UI scaling with smooth animations.
  - `Animation::SmoothFloat` - Smooth floating point value with automatic interpolation
  - `Animation::SmoothVec2` - Smooth 2D vector with automatic interpolation
  - Both support `Update()`, `SetTarget()`, `SetImmediate()`, and `IsComplete()` methods
  - Implicit conversion operators for convenient usage
- **Resolve Template Function**: Added `Resolve(opt, def)` template in Theme.h for cleaner optional-to-default fallback syntax.
- **Component Theme Accessors**: New accessors for component-specific theme configurations.
  - `Theme::Slider()`, `Theme::Toggle()`, `Theme::Card()`, `Theme::Nav()`
  - `Theme::Button()`, `Theme::Window()`, `Theme::Draw()`, `Theme::Layout()`
  - `Theme::NumericInput()`
  - Mutable versions: `Theme::SliderMut()`, etc.

### Changed
- **ThemeConfig Structure**: Reorganized into component-grouped nested structures for better cohesion.
  - Added `SliderTheme`, `ToggleTheme`, `CardTheme`, `NavTheme`, `ButtonTheme`, `WindowTheme`, `DrawTheme`, `LayoutTheme`, `NumericInputTheme`
  - Migrated ~100 hardcoded `constexpr` constants from Internal.h into these theme structures
  - Original `ThemeDimensions` and `ThemeEffects` retained for backward compatibility
- Internal code organization: Added `Components/ConfigImpl.cpp` for centralized FromTheme() implementations.

### Usage Examples

**Per-Instance Config (partial override):**
```cpp
SliderConfig cfg;
cfg.height = 48.0f;  // Only override height, rest uses Theme defaults
ModernSliderFloat("Volume", &vol, 0, 100, "%.0f", std::nullopt, cfg);
```

**FromTheme with modification:**
```cpp
auto cfg = SliderConfig::FromTheme();
cfg.height *= 1.5f;  // Scale up 50%
cfg.knobRadius *= 1.5f;
```

**Dynamic real-time scaling:**
```cpp
static Animation::SmoothFloat height;
height.SetTarget(someCondition ? 48.0f : 24.0f);
SliderConfig cfg;
cfg.height = height.Update();  // Smoothly animates to target
```

---

## [0.3.0] - 2025-12-28

### Added
- **Runtime Theme Customization System**: Complete overhaul of the theme system from compile-time `constexpr` to runtime-configurable settings.
  - `ThemeConfig` struct containing `ThemeColors`, `ThemeDimensions`, `ThemeEffects`, and `ThemeAnimation` sub-structs
  - `ThemeManager` singleton for centralized theme configuration
  - `ThemePreset` enum for preset themes (currently: `Cyberpunk`)
  - `RoundingStyle` enum for quick rounding presets (`Sharp`, `Subtle`, `Rounded`, `Pill`)
  - `AnimSpeed` enum for animation speed presets (`Instant`, `Fast`, `Normal`, `Slow`, `Relaxed`)
- **New API Functions**:
  - `Theme::GetConfig()` - Access mutable theme configuration
  - `Theme::SetConfig()` - Replace entire theme configuration
  - `Theme::ResetToDefault()` - Reset to default Cyberpunk theme
  - `Theme::LoadPreset()` - Load a preset theme
  - `ThemeManager::Instance()` - Access the singleton
- **ThemeConfig.h**: New header file defining all configuration structures with sensible defaults.
- **Config Struct API**: New config struct versions for components with many parameters (recommended over legacy positional APIs):
  - `GlowButtonConfig` - Config struct for `GlowButton()` with fields: `size`, `glowColor`, `forceHover`, `layer`, `bgAlpha`
  - `CooldownButtonConfig` - Config struct for `CooldownButton()` with fields: `size`, `glowColor`, `cooldownProgress`, `layer`, `bgAlpha`
  - `NavbarHeaderConfig` - Config struct for `NavbarHeader()` with fields: `iconExpanded`, `iconCollapsed`, `collapsed`, `width`, `closeClicked`, `glowColor`

### Changed
- **BREAKING**: Theme values are now accessed via function calls instead of direct constants:
  - `Theme::AccentCyan` → `Theme::AccentCyan()`
  - `Theme::WindowRounding` → `Theme::WindowRounding()`
  - All other theme accessors follow the same pattern
- **BREAKING**: Default parameter values in `Components.h` changed:
  - `NavItem(..., float width = Theme::NavbarWidth, ...)` → `NavItem(..., float width = 0, ...)` (0 = use theme default)
  - `Spacing(float height = Theme::ItemSpacing)` → `Spacing(float height = 0)` (0 = use theme default)
- All component implementations updated to use function-style theme accessors.
- Theme system refactored from 40+ `constexpr` values to hierarchical configuration structure.
- Theme accessor functions now use `GetConfigConst()` for const correctness.

### Migration Guide
To migrate from v0.2.x:
1. If you were using `Theme::AccentCyan` directly in your code, change to `Theme::AccentCyan()`
2. If you were passing `Theme::NavbarWidth` to `NavItem()`, change to `0` or remove the parameter
3. If you were passing `Theme::ItemSpacing` to `Spacing()`, change to `0` or remove the parameter
4. To customize theme at runtime:
   ```cpp
   auto& config = EFIGUI::Theme::GetConfig();
   config.colors.accentPrimary = IM_COL32(255, 100, 50, 255);  // Custom orange accent
   config.dimensions.windowRounding = 16.0f;  // More rounded windows
   ```

---

## [0.2.6] - 2025-12-27 [`2e31a04`]

### Fixed
- **Glassmorphism alpha stacking**: Changed `GlassOverlay*` alpha values from 250-255 to 200 and `GlassBaseLayer` to transparent (alpha 0) to prevent transparency stacking issues where multiple layers would accumulate incorrectly.
- **NumericInput text centering**: Text in `NumericInput` now properly centers by default. Added `TextAlignment` enum for configurable alignment (Left, Center, Right).

### Added
- **bgAlpha parameter**: New optional `bgAlpha` parameter for controlling background transparency:
  - `GlassmorphismBg()` - Core glassmorphism drawing function
  - `GlowButton()`, `DangerButton()`, `CooldownButton()` - Button components
- **TextAlignment enum**: New `TextAlignment` enum with `Left`, `Center`, `Right` values for `NumericInputConfig`.
- **LayoutConstants namespace**: Centralized constants for layout helpers (`SeparatorOffsetY`, `SeparatorSpacing`).
- **TooltipConstants namespace**: Centralized constants for tooltip rendering (`PaddingX`, `PaddingY`).

### Changed
- `NumericInputConfig` now includes `alignment` field (default: `TextAlignment::Center`).
- Layout.cpp and Tooltip.cpp now use named constants instead of magic numbers.

---

## [0.2.5] - 2025-12-27 [`211bee3`]

### Fixed
- **Button transparency stacking**: Fixed issue where multiple semi-transparent layers would accumulate, causing buttons to appear more transparent than intended. Added opaque base layer (`GlassBaseLayer`) to ensure consistent alpha 200 appearance.
- **Slider input focus**: Completely refactored slider value input to fix focus management issues. Removed `InvisibleButton` that was blocking `InputText` focus.

### Added
- **NumericInput component**: New standalone editable numeric value display box. Can be used independently or as part of slider components.
  - `NumericInputConfig` struct for configuration (min, max, precision, width)
  - `NumericInput()` function with glow effects and smooth animations
  - `PruneNumericInputBuffers()` for cleanup

### Changed
- **Slider architecture refactored**: Decoupled slider into independent components for better maintainability.
  - `SliderTrack` (internal) - Handles track, knob, and drag interaction
  - `NumericInput` (public) - Standalone value input box
  - `ModernSliderFloat` now composes these two components
  - Removed ~150 lines of complex state machine code
  - Value synchronization through shared pointer (no explicit sync logic needed)
- **GlassmorphismBg constants**: Adjusted overlay alpha values (180→255, 160→250, 180→252) to work with new base layer.

---

## [0.2.4] - 2025-12-27 [`daaec1c`]

### Fixed
- **Slider input state machine**: Replaced implicit state tracking with explicit three-state enum (`Idle → WaitingForFocus → Editing`) for reliable focus management.
- **IconButton ID collision**: Added optional `uniqueId` parameter to avoid ID collisions when using the same icon multiple times.
- **FeatureCard initial state sync**: Fixed visual transition on first render when `enabled=true` by syncing animation state on first frame.

### Added
- **Animation state cleanup**: Added `PruneStaleStates()` to automatically clean up unused widget animation states (called every ~1 second).
- **Slider input buffer cleanup**: Added `PruneSliderInputBuffers()` to prevent memory growth from stale slider input buffers.
- **Frame lifecycle validation**: Added debug assertions in `BeginFrame()`/`EndFrame()` to detect missing calls during development.

### Changed
- `WidgetState` now tracks `lastUpdateFrame` for pruning stale entries.
- `SliderInputData` struct now stores both buffer content and last update frame.
- Periodic cleanup runs in `BeginFrame()` every 60 frames (~1 second at 60fps).
- **Internal refactoring**: Extracted magic numbers to centralized constant namespaces for improved maintainability.
  - `CardConstants` (19 constants) - Card and section header rendering
  - `NavConstants` (12 constants) - Navigation item rendering
  - `ToggleConstants` (6 constants) - Toggle switch rendering
- **Card.cpp**: Refactored `FeatureCard` from ~146 lines into focused helper functions:
  - `CalculateCardLayout()` - Layout dimension calculation
  - `DrawCardBackground()` - Background rendering with alpha override
  - `DrawWrappedDescription()` - Multi-line text wrapping
  - `DrawCardToggle()` - Inline toggle switch rendering
- **Navigation.cpp**: Updated `NavItem`, `NavCollapseButton`, `NavSectionHeader` to use `NavConstants`.
- **Toggle.cpp**: Updated `ModernToggle`, `ModernToggleWithDesc` to use `ToggleConstants`.

---

## [0.2.3] - 2025-12-26 [`856db2e`]

### Fixed
- **Slider input focus issue**: Fixed bug where clicking the value input box next to a slider would briefly enter edit mode but immediately exit, making it impossible to type values manually.

### Changed
- Replaced frame counting approach with state machine for slider input focus management.
- Input focus now uses `IsItemDeactivated()` instead of `!IsItemActive()` for reliable exit detection.
- Refactored slider input editing code following Clean Code principles (single-responsibility functions, meaningful names).

---

## [0.2.2] - 2025-12-26 [`791f03a`]

### Changed
- **Refactored Components.cpp**: Split monolithic 1800+ line file into modular component files for better maintainability.

### Added
- New component files structure:
  - `Components/Window.cpp` - Window components
  - `Components/Panel.cpp` - Glass panel components
  - `Components/Navigation.cpp` - Navbar and navigation items
  - `Components/Button.cpp` - Button variants (GlowButton, DangerButton, etc.)
  - `Components/Toggle.cpp` - Toggle switch components
  - `Components/Input.cpp` - Text input components
  - `Components/Slider.cpp` - Slider components
  - `Components/Combo.cpp` - Dropdown/combo components
  - `Components/Progress.cpp` - Progress bar components
  - `Components/Card.cpp` - Feature card components
  - `Components/Tooltip.cpp` - Tooltip and help marker
  - `Components/Layout.cpp` - Layout helpers (Spacing, Separator, SameLine)
  - `Components/Internal.h` - Shared internal utilities

---

## [0.2.1] - 2025-12-26 [`32555c4`]

### Added
- **Layer System**: Fine-grained z-order control for rendering.
- Layer hierarchy: Background, Content, Widget, WidgetGlow, Popup, PopupGlow, Overlay, Debug.
- `BeginFrame()` / `EndFrame()` functions for deferred drawing management.
- `LayerConfig` for global layer configuration.
- Optional `layer` parameter for widgets: GlowButton, DangerButton, ColoredButton, CooldownButton, ModernSliderFloat, ModernSliderInt, ModernProgressBar.

### Fixed
- Z-order issues where glow effects could cover popup/dropdown content.

---

## [0.2.0] - 2025-12-26 [`c43228a`]

### Added
- **Flexible customization system**: `std::optional<T>` parameters for per-instance styling without breaking defaults.
- Optional parameters for transparency, colors, and sizes across components.
- Theme utility functions: `ExtractRGB()`, `ApplyAlpha()`, `ExtractAlpha()`, `MakeColor()`.
- Default value constants in Theme: `DefaultOverlayAlpha`, `DefaultBgAlpha`, `DefaultButtonBgAlpha`, etc.

### Changed
- **BREAKING**: Components now use `std::optional` for optional parameters instead of default values.
- Requires C++17 or later.

---

## [0.1.1] - 2025-12-26 [`151b13f`]

### Removed
- Removed `Icons.h` dependency.

### Added
- Configurable icons system via `EFIGUI::Icons` namespace.

---

## [0.1.0] - 2025-12-25 [`44890e3`]

### Changed
- Updated license from MIT to GPLv3.

---

## [0.0.1] - 2025-12-24 [`50b422e`]

### Added
- Initial release of EFIGUI framework.
- Cyberpunk theme with neon cyan/purple accents.
- Glassmorphism effects with real-time blur (DX11 backend).
- Complete widget set:
  - Windows: `BeginBorderlessWindow`, `BeginCustomWindow`
  - Navigation: `NavItem`, `NavbarHeader`, `NavCollapseButton`, `NavSectionHeader`
  - Buttons: `GlowButton`, `IconButton`, `DangerButton`, `ColoredButton`, `CooldownButton`
  - Panels: `BeginGlassPanel`, `EndGlassPanel`
  - Input: `ModernToggle`, `ModernInputText`, `ModernSliderFloat`, `ModernSliderInt`, `ModernCombo`
  - Display: `ModernProgressBar`, `StatusIndicator`, `FeatureCard`, `SectionHeader`, `ModernTooltip`, `HelpMarker`
  - Layout: `Spacing`, `Separator`, `SameLine`
- Animation system with hover, active, and pulse effects.
- Abstract blur backend interface for multiple graphics APIs.
- DX11 blur backend implementation.
- Font Awesome icons support.
