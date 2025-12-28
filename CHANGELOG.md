# Changelog

All notable changes to EFIGUI will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
- Initial release with Polyform Noncommercial License 1.0.0.

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
