# Changelog

All notable changes to EFIGUI will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [0.2.3] - 2025-12-26

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
