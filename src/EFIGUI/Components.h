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
    // Forward Declarations
    // =============================================

    struct NavItemConfig;
    struct SliderConfig;
    struct ToggleConfig;
    struct CardConfig;

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

    // Configuration for NavbarHeader
    struct NavbarHeaderConfig
    {
        const char* iconExpanded = "<<";                      // Icon when expanded
        const char* iconCollapsed = ">>";                     // Icon when collapsed
        bool collapsed = false;                               // Current collapse state
        float width = 0.0f;                                   // Header width (0 = Theme::NavbarWidth())
        bool* closeClicked = nullptr;                         // Output: close button clicked
        std::optional<ImU32> glowColor = std::nullopt;        // Glow color (omit = Theme::AccentCyan)
    };

    // Navbar header with title and collapse button (Config struct version - recommended)
    // Returns true if collapse button was clicked
    bool NavbarHeader(const char* title, const NavbarHeaderConfig& config);

    // Navbar header with title and collapse button (legacy API - for backward compatibility)
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

    // Left sidebar navigation item with icon (Config version - recommended)
    // config: per-instance configuration (omit for Theme defaults)
    bool NavItem(const char* icon, const char* label, bool selected, const NavItemConfig& config,
                 float width = 0, bool collapsed = false);

    // Left sidebar navigation item with icon (legacy API - for backward compatibility)
    // Returns true if clicked
    // collapsed: if true, only show icon (no label)
    // width: item width (0 = use Theme::NavbarWidth())
    // accentColor: selected/hover accent color (omit = use Theme::AccentCyan)
    bool NavItem(const char* icon, const char* label, bool selected, float width = 0,
                 bool collapsed = false, std::optional<ImU32> accentColor = std::nullopt);

    // Navigation section header (Config version - recommended)
    // config: per-instance configuration (omit for Theme defaults)
    void NavSectionHeader(const char* label, const NavItemConfig& config);

    // Navigation section header (legacy API - for backward compatibility)
    void NavSectionHeader(const char* label);

    // Collapse/Expand toggle button for navbar (uses NavItemConfig internally)
    // config: per-instance configuration (omit for Theme defaults)
    bool NavCollapseButton(bool collapsed, float width, const NavItemConfig& config);

    // Collapse/Expand toggle button for navbar (legacy API - for backward compatibility)
    bool NavCollapseButton(bool collapsed, float width);

    // =============================================
    // Buttons
    // =============================================

    // Configuration for GlowButton
    struct GlowButtonConfig
    {
        ImVec2 size = ImVec2(0, 0);                           // Button size (0 = auto-size)
        std::optional<ImU32> glowColor = std::nullopt;        // Glow color (omit = Theme::AccentCyan)
        bool forceHover = false;                              // Always show hover animation
        std::optional<Layer> layer = std::nullopt;            // Rendering layer (omit = default)
        std::optional<uint8_t> bgAlpha = std::nullopt;        // Background alpha (omit = default)
    };

    // Glowing button with neon effect (Config struct version - recommended)
    bool GlowButton(const char* label, const GlowButtonConfig& config);

    // Glowing button with neon effect (legacy API - for backward compatibility)
    // forceHover: if true, always show hover animation (marquee border)
    // layer: rendering layer for glow/marquee effects (omit = use LayerConfig default)
    // bgAlpha: background alpha (omit = use GlassOverlay defaults, 0-255 = custom alpha)
    bool GlowButton(const char* label, ImVec2 size = ImVec2(0, 0), std::optional<ImU32> glowColor = std::nullopt, bool forceHover = false, std::optional<Layer> layer = std::nullopt, std::optional<uint8_t> bgAlpha = std::nullopt);

    // Icon button (just an icon, no label)
    // uniqueId: unique ID to avoid collision when same icon used multiple times (omit = use icon as ID)
    // bgAlpha: background alpha on hover (omit = use default)
    bool IconButton(const char* icon, ImVec2 size = ImVec2(28, 28), std::optional<ImU32> color = std::nullopt, std::optional<uint8_t> bgAlpha = std::nullopt, const char* uniqueId = nullptr);

    // Danger button (red glow, always shows hover effect with marquee)
    // layer: rendering layer for glow/marquee effects (omit = use LayerConfig default)
    // bgAlpha: background alpha (omit = use GlassOverlay defaults, 0-255 = custom alpha)
    bool DangerButton(const char* label, ImVec2 size = ImVec2(0, 0), std::optional<Layer> layer = std::nullopt, std::optional<uint8_t> bgAlpha = std::nullopt);

    // Colored button - always shows colored border, no marquee effect
    // bgAlpha: background alpha (omit = use default)
    // layer: rendering layer for glow effects (omit = use LayerConfig default)
    bool ColoredButton(const char* label, ImVec2 size, ImU32 borderColor, std::optional<uint8_t> bgAlpha = std::nullopt, std::optional<Layer> layer = std::nullopt);

    // Configuration for CooldownButton
    struct CooldownButtonConfig
    {
        ImVec2 size = ImVec2(0, 0);                           // Button size (0 = auto-size)
        ImU32 glowColor = IM_COL32(0, 245, 255, 255);         // Glow color (default = cyan)
        float cooldownProgress = 0.0f;                        // 0.0 = no cooldown, 1.0 = full cooldown
        std::optional<Layer> layer = std::nullopt;            // Rendering layer (omit = default)
        std::optional<uint8_t> bgAlpha = std::nullopt;        // Background alpha (omit = default)
    };

    // Cooldown button - shows a cooldown progress overlay (Config struct version - recommended)
    bool CooldownButton(const char* label, const CooldownButtonConfig& config);

    // Cooldown button - shows a cooldown progress overlay (legacy API - for backward compatibility)
    // cooldownProgress: 0.0 = no cooldown, 1.0 = full cooldown (just started)
    // layer: rendering layer for glow/marquee effects (omit = use LayerConfig default)
    // bgAlpha: background alpha (omit = use GlassOverlay defaults, 0-255 = custom alpha)
    bool CooldownButton(const char* label, ImVec2 size, ImU32 glowColor, float cooldownProgress, std::optional<Layer> layer = std::nullopt, std::optional<uint8_t> bgAlpha = std::nullopt);

    // =============================================
    // Per-Instance Config Structures
    // =============================================
    // These structures allow per-instance customization of component appearance.
    // Use std::optional fields to selectively override Theme defaults.
    // Usage: SliderConfig cfg; cfg.height = 48.0f; ModernSliderFloat(..., cfg);

    // === SliderConfig ===
    struct SliderConfig
    {
        // Dimensions (nullopt = use Theme::Slider() default)
        std::optional<float> height = std::nullopt;
        std::optional<float> trackHeight = std::nullopt;
        std::optional<float> knobRadius = std::nullopt;
        std::optional<float> inputWidth = std::nullopt;
        std::optional<float> inputHeight = std::nullopt;
        std::optional<float> inputGap = std::nullopt;
        std::optional<float> inputRounding = std::nullopt;

        // Colors
        std::optional<ImU32> trackColor = std::nullopt;
        std::optional<ImU32> fillColor = std::nullopt;
        std::optional<ImU32> knobColor = std::nullopt;
        std::optional<ImU32> glowColor = std::nullopt;

        // Effects
        std::optional<int> glowLayers = std::nullopt;
        std::optional<float> knobGlowExpand = std::nullopt;
        std::optional<float> inputGlowExpand = std::nullopt;

        // Behavior
        bool showInput = true;
        std::optional<Layer> layer = std::nullopt;

        // Create config with all Theme defaults filled in
        static SliderConfig FromTheme();
    };

    // === ToggleConfig ===
    struct ToggleConfig
    {
        // Dimensions
        std::optional<float> width = std::nullopt;
        std::optional<float> height = std::nullopt;
        std::optional<float> knobSize = std::nullopt;
        std::optional<float> labelGap = std::nullopt;

        // Colors
        std::optional<ImU32> trackOnColor = std::nullopt;
        std::optional<ImU32> trackOffColor = std::nullopt;
        std::optional<ImU32> knobColor = std::nullopt;
        std::optional<ImU32> glowColor = std::nullopt;

        // Effects
        std::optional<float> animSpeed = std::nullopt;
        std::optional<float> glowRadius = std::nullopt;
        bool disabled = false;

        static ToggleConfig FromTheme();
    };

    // === CardConfig ===
    struct CardConfig
    {
        // Layout dimensions
        std::optional<float> height = std::nullopt;
        std::optional<float> iconSize = std::nullopt;
        std::optional<float> iconPadding = std::nullopt;
        std::optional<float> iconTextOffset = std::nullopt;
        std::optional<float> minTextWidth = std::nullopt;
        std::optional<float> nameOffsetY = std::nullopt;
        std::optional<float> descOffsetY = std::nullopt;

        // Inline toggle dimensions
        std::optional<float> toggleWidth = std::nullopt;
        std::optional<float> toggleHeight = std::nullopt;
        std::optional<float> toggleRounding = std::nullopt;
        std::optional<float> toggleRightMargin = std::nullopt;
        std::optional<float> knobRadius = std::nullopt;
        std::optional<float> knobPadding = std::nullopt;
        std::optional<float> knobTravel = std::nullopt;
        std::optional<float> toggleAnimSpeed = std::nullopt;

        // Section header
        std::optional<float> sectionLineOffset = std::nullopt;
        std::optional<float> sectionSpacing = std::nullopt;

        // Colors
        std::optional<ImU32> bgColor = std::nullopt;
        std::optional<uint8_t> bgAlpha = std::nullopt;

        static CardConfig FromTheme();
    };

    // === NavItemConfig ===
    struct NavItemConfig
    {
        // Dimensions
        std::optional<float> height = std::nullopt;
        std::optional<float> iconPadding = std::nullopt;
        std::optional<float> labelSpacing = std::nullopt;
        std::optional<float> accentBarWidth = std::nullopt;
        std::optional<float> accentBarHeightRatio = std::nullopt;
        std::optional<float> accentBarGlowRadius = std::nullopt;
        std::optional<float> rounding = std::nullopt;
        std::optional<float> collapsedIconScale = std::nullopt;

        // Section header
        std::optional<float> sectionPaddingX = std::nullopt;
        std::optional<float> sectionPaddingY = std::nullopt;
        std::optional<float> sectionHeight = std::nullopt;

        // Colors
        std::optional<ImU32> accentColor = std::nullopt;
        std::optional<ImU32> bgColor = std::nullopt;

        // Alpha values
        std::optional<int> accentGlowAlpha = std::nullopt;
        std::optional<int> bgAlphaMultiplier = std::nullopt;
        std::optional<int> hoverBgAlpha = std::nullopt;

        static NavItemConfig FromTheme();
    };

    // === WindowConfig ===
    struct WindowConfig
    {
        // Dimensions
        std::optional<float> titleBarHeight = std::nullopt;
        std::optional<float> titleTextPadding = std::nullopt;
        std::optional<float> titleButtonSize = std::nullopt;
        std::optional<float> rounding = std::nullopt;

        // Colors
        std::optional<ImU32> titleBarLeftColor = std::nullopt;
        std::optional<ImU32> titleBarRightColor = std::nullopt;
        std::optional<uint8_t> overlayAlpha = std::nullopt;

        static WindowConfig FromTheme();
    };

    // === PanelConfig ===
    struct PanelConfig
    {
        std::optional<float> rounding = std::nullopt;
        std::optional<uint8_t> bgAlpha = std::nullopt;
        std::optional<ImU32> borderColor = std::nullopt;
        std::optional<ImU32> glowColor = std::nullopt;
        bool showBorder = true;

        static PanelConfig FromTheme();
    };

    // === ProgressConfig ===
    struct ProgressConfig
    {
        std::optional<float> height = std::nullopt;
        std::optional<float> rounding = std::nullopt;
        std::optional<ImU32> trackColor = std::nullopt;
        std::optional<ImU32> fillColor = std::nullopt;
        std::optional<ImU32> glowColor = std::nullopt;
        std::optional<int> glowLayers = std::nullopt;
        std::optional<Layer> layer = std::nullopt;

        static ProgressConfig FromTheme();
    };

    // === InputConfig ===
    struct InputConfig
    {
        std::optional<float> rounding = std::nullopt;
        std::optional<ImU32> bgColor = std::nullopt;
        std::optional<ImU32> borderColor = std::nullopt;
        std::optional<ImU32> focusBorderColor = std::nullopt;
        std::optional<ImU32> textColor = std::nullopt;

        static InputConfig FromTheme();
    };

    // =============================================
    // Toggle / Checkbox
    // =============================================

    // Modern toggle switch (Config version - recommended)
    // config: per-instance configuration (omit for Theme defaults)
    bool ModernToggle(const char* label, bool* value, const ToggleConfig& config);

    // Toggle with description (Config version - recommended)
    bool ModernToggleWithDesc(const char* label, const char* description, bool* value, const ToggleConfig& config);

    // Modern toggle switch (legacy API - for backward compatibility)
    bool ModernToggle(const char* label, bool* value);

    // Toggle with description (legacy API - for backward compatibility)
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
    // NumericInput
    // =============================================

    // Text alignment options for NumericInput
    enum class TextAlignment
    {
        Left,
        Center,
        Right
    };

    // Configuration for NumericInput
    struct NumericInputConfig
    {
        float min = 0.0f;
        float max = 100.0f;
        int precision = 2;
        float width = 55.0f;
        TextAlignment alignment = TextAlignment::Center;  // Text alignment within the input box
    };

    // Standalone editable numeric value display box
    // Returns true if value was changed
    // layer: rendering layer for glow effects (omit = use LayerConfig default)
    bool NumericInput(const char* label, float* value, const NumericInputConfig& config, std::optional<Layer> layer = std::nullopt);

    // Cleanup stale numeric input buffers (called automatically from BeginFrame)
    void PruneNumericInputBuffers(int maxIdleFrames = 60);

    // =============================================
    // Slider
    // =============================================

    // Modern slider with glow (Config version - recommended)
    // config: per-instance configuration (omit for Theme defaults)
    bool ModernSliderFloat(const char* label, float* value, float min, float max,
                           const char* format, std::optional<ImU32> glowColor,
                           const SliderConfig& config);
    bool ModernSliderInt(const char* label, int* value, int min, int max,
                         const char* format, std::optional<ImU32> glowColor,
                         const SliderConfig& config);

    // Modern slider with glow (legacy API - for backward compatibility)
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

    // Feature card (for feature toggles with icon) - Config version (recommended)
    // config: per-instance configuration (omit for Theme defaults)
    bool FeatureCard(const char* icon, const char* name, const char* description, bool* enabled, const CardConfig& config);

    // Feature card (for feature toggles with icon) - legacy API (for backward compatibility)
    // bgAlpha: background alpha (omit = Theme default, 0-255 = custom alpha)
    bool FeatureCard(const char* icon, const char* name, const char* description, bool* enabled, std::optional<uint8_t> bgAlpha = std::nullopt);

    // Section header with optional collapse (Config version - recommended)
    // config: per-instance configuration (omit for Theme defaults)
    bool SectionHeader(const char* label, bool* collapsed, const CardConfig& config);

    // Section header with optional collapse (legacy API - for backward compatibility)
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

    // height: spacing height (0 = use Theme::ItemSpacing())
    void Spacing(float height = 0);
    void Separator(std::optional<ImU32> color = std::nullopt);  // omit = Theme::BorderDefault
    void SameLine(float offset = 0.0f, float spacing = -1.0f);
}
