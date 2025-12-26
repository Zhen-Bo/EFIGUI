#pragma once
#include "imgui.h"
#include "Theme.h"
#include "Animation.h"
#include "Layer.h"
#include <string>
#include <cstdint>
#include <optional>

namespace EFIGUI
{

    // =============================================
    // Custom Window (Borderless with navbar)
    // =============================================

    // Begin a borderless window with integrated navbar
    // Returns true if window is open
    // overlayAlpha: glassmorphism overlay alpha (omit = use default, 0-255 = custom alpha)
    bool BeginBorderlessWindow(const char* name, bool* p_open, ImGuiWindowFlags flags = 0, std::optional<uint8_t> overlayAlpha = std::nullopt);
    void EndBorderlessWindow();

    // =============================================
    // Custom Title Bar (Legacy - for backwards compatibility)
    // =============================================

    // Renders a custom title bar with drag support
    // Returns true if close button was clicked
    bool BeginCustomWindow(const char* name, bool* p_open, ImGuiWindowFlags flags = 0);
    void EndCustomWindow();

    // =============================================
    // Navbar Header (Title + Collapse)
    // =============================================

    // Navbar header with title and collapse button
    // Returns true if collapse button was clicked
    // glowColor: icon glow color when collapsed (omit = use Theme::AccentCyan)
    bool NavbarHeader(const char* title, const char* iconExpanded, const char* iconCollapsed,
                      bool collapsed, float width, bool* closeClicked = nullptr, std::optional<ImU32> glowColor = std::nullopt);

    // =============================================
    // Glass Panel
    // =============================================

    // Begin a glass-effect panel (semi-transparent with blur background)
    // bgAlpha: background alpha (omit = use Theme default, 0-255 = custom alpha)
    // glowColor: top edge glow color (omit = use Theme::AccentCyan)
    void BeginGlassPanel(const char* id, ImVec2 size = ImVec2(0, 0), bool border = true,
                         std::optional<uint8_t> bgAlpha = std::nullopt, std::optional<ImU32> glowColor = std::nullopt);
    void EndGlassPanel();

    // =============================================
    // Navigation
    // =============================================

    // Left sidebar navigation item with icon
    // Returns true if clicked
    // collapsed: if true, only show icon (no label)
    // accentColor: selected/hover accent color (omit = use Theme::AccentCyan)
    bool NavItem(const char* icon, const char* label, bool selected, float width = Theme::NavbarWidth,
                 bool collapsed = false, std::optional<ImU32> accentColor = std::nullopt);

    // Navigation section header
    void NavSectionHeader(const char* label);

    // Collapse/Expand toggle button for navbar
    bool NavCollapseButton(bool collapsed, float width);

    // =============================================
    // Buttons
    // =============================================

    // Glowing button with neon effect
    // forceHover: if true, always show hover animation (marquee border)
    // layer: rendering layer for glow/marquee effects (omit = use LayerConfig default)
    bool GlowButton(const char* label, ImVec2 size = ImVec2(0, 0), std::optional<ImU32> glowColor = std::nullopt, bool forceHover = false, std::optional<Layer> layer = std::nullopt);

    // Icon button (just an icon, no label)
    // bgAlpha: background alpha on hover (omit = use default)
    bool IconButton(const char* icon, ImVec2 size = ImVec2(28, 28), std::optional<ImU32> color = std::nullopt, std::optional<uint8_t> bgAlpha = std::nullopt);

    // Danger button (red glow, always shows hover effect with marquee)
    // layer: rendering layer for glow/marquee effects (omit = use LayerConfig default)
    bool DangerButton(const char* label, ImVec2 size = ImVec2(0, 0), std::optional<Layer> layer = std::nullopt);

    // Colored button - always shows colored border, no marquee effect
    // bgAlpha: background alpha (omit = use default)
    // layer: rendering layer for glow effects (omit = use LayerConfig default)
    bool ColoredButton(const char* label, ImVec2 size, ImU32 borderColor, std::optional<uint8_t> bgAlpha = std::nullopt, std::optional<Layer> layer = std::nullopt);

    // Cooldown button - shows a cooldown progress overlay
    // cooldownProgress: 0.0 = no cooldown, 1.0 = full cooldown (just started)
    // layer: rendering layer for glow/marquee effects (omit = use LayerConfig default)
    bool CooldownButton(const char* label, ImVec2 size, ImU32 glowColor, float cooldownProgress, std::optional<Layer> layer = std::nullopt);

    // =============================================
    // Toggle / Checkbox
    // =============================================

    // Modern toggle switch
    bool ModernToggle(const char* label, bool* value);

    // Toggle with description
    bool ModernToggleWithDesc(const char* label, const char* description, bool* value);

    // =============================================
    // Input
    // =============================================

    // Modern input text with glow border on focus
    bool ModernInputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0);

    // Modern multiline input
    bool ModernInputTextMultiline(const char* label, char* buf, size_t buf_size, ImVec2 size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0);

    // Modern input with std::string (auto-resizing)
    bool ModernInputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0);

    // Modern multiline input with std::string (auto-resizing, handles large paste operations)
    bool ModernInputTextMultiline(const char* label, std::string* str, ImVec2 size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0);

    // =============================================
    // Slider
    // =============================================

    // Modern slider with glow
    // layer: rendering layer for knob and input box glow effects (omit = use LayerConfig default)
    bool ModernSliderFloat(const char* label, float* value, float min, float max, const char* format = "%.1f", std::optional<Layer> layer = std::nullopt);
    bool ModernSliderInt(const char* label, int* value, int min, int max, const char* format = "%d", std::optional<Layer> layer = std::nullopt);

    // =============================================
    // Combo / Dropdown
    // =============================================

    // Modern dropdown
    // popupBgAlpha: popup background alpha (omit = Theme default, 0-255 = custom alpha)
    bool ModernCombo(const char* label, int* current_item, const char* const items[], int items_count, std::optional<uint8_t> popupBgAlpha = std::nullopt);

    // =============================================
    // Progress / Status
    // =============================================

    // Modern progress bar with glow
    // layer: rendering layer for progress bar glow effects (omit = use LayerConfig default)
    void ModernProgressBar(float fraction, ImVec2 size = ImVec2(-1, 0), const char* overlay = nullptr, std::optional<Layer> layer = std::nullopt);

    // Status indicator (colored dot + text)
    // dotSize: size of the dot (omit = Theme::DefaultDotSize)
    void StatusIndicator(const char* label, ImU32 color, bool pulse = false, std::optional<float> dotSize = std::nullopt);

    // =============================================
    // Cards / Sections
    // =============================================

    // Feature card (for feature toggles with icon)
    // bgAlpha: background alpha (omit = Theme default, 0-255 = custom alpha)
    bool FeatureCard(const char* icon, const char* name, const char* description, bool* enabled, std::optional<uint8_t> bgAlpha = std::nullopt);

    // Section header with optional collapse
    // accentColor: header text color (omit = Theme::TextAccent)
    bool SectionHeader(const char* label, bool* collapsed = nullptr, std::optional<ImU32> accentColor = std::nullopt);

    // =============================================
    // Tooltip
    // =============================================

    // Modern tooltip with styled background
    void ModernTooltip(const char* text);

    // Hover help icon with tooltip
    void HelpMarker(const char* text);

    // =============================================
    // Spacing / Layout Helpers
    // =============================================

    void Spacing(float height = Theme::ItemSpacing);
    void Separator(std::optional<ImU32> color = std::nullopt);  // omit = Theme::BorderDefault
    void SameLine(float offset = 0.0f, float spacing = -1.0f);
}
