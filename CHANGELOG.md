# Changelog

All notable changes to EFIGUI will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [0.2.4] - 2025-12-27

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
