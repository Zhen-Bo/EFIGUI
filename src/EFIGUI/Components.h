#pragma once
#include "imgui.h"
#include "Theme.h"
#include "Animation.h"
#include <string>

namespace EFIGUI
{
    // =============================================
    // Custom Window (Borderless with navbar)
    // =============================================

    // Begin a borderless window with integrated navbar
    // Returns true if window is open
    bool BeginBorderlessWindow(const char* name, bool* p_open, ImGuiWindowFlags flags = 0);
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
    bool NavbarHeader(const char* title, const char* iconExpanded, const char* iconCollapsed,
                      bool collapsed, float width, bool* closeClicked = nullptr);

    // =============================================
    // Glass Panel
    // =============================================

    // Begin a glass-effect panel (semi-transparent with blur background)
    void BeginGlassPanel(const char* id, ImVec2 size = ImVec2(0, 0), bool border = true);
    void EndGlassPanel();

    // =============================================
    // Navigation
    // =============================================

    // Left sidebar navigation item with icon
    // Returns true if clicked
    // collapsed: if true, only show icon (no label)
    bool NavItem(const char* icon, const char* label, bool selected, float width = Theme::NavbarWidth, bool collapsed = false);

    // Navigation section header
    void NavSectionHeader(const char* label);

    // Collapse/Expand toggle button for navbar
    bool NavCollapseButton(bool collapsed, float width);

    // =============================================
    // Buttons
    // =============================================

    // Glowing button with neon effect
    // forceHover: if true, always show hover animation (marquee border)
    bool GlowButton(const char* label, ImVec2 size = ImVec2(0, 0), ImU32 glowColor = Theme::AccentCyan, bool forceHover = false);

    // Icon button (just an icon, no label)
    bool IconButton(const char* icon, ImVec2 size = ImVec2(28, 28), ImU32 color = Theme::TextPrimary);

    // Danger button (red glow, always shows hover effect with marquee)
    bool DangerButton(const char* label, ImVec2 size = ImVec2(0, 0));

    // Colored button - always shows colored border, no marquee effect
    bool ColoredButton(const char* label, ImVec2 size, ImU32 borderColor);

    // Cooldown button - shows a cooldown progress overlay
    // cooldownProgress: 0.0 = no cooldown, 1.0 = full cooldown (just started)
    bool CooldownButton(const char* label, ImVec2 size, ImU32 glowColor, float cooldownProgress);

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
    bool ModernSliderFloat(const char* label, float* value, float min, float max, const char* format = "%.1f");
    bool ModernSliderInt(const char* label, int* value, int min, int max, const char* format = "%d");

    // =============================================
    // Combo / Dropdown
    // =============================================

    // Modern dropdown
    bool ModernCombo(const char* label, int* current_item, const char* const items[], int items_count);

    // =============================================
    // Progress / Status
    // =============================================

    // Modern progress bar with glow
    void ModernProgressBar(float fraction, ImVec2 size = ImVec2(-1, 0), const char* overlay = nullptr);

    // Status indicator (colored dot + text)
    void StatusIndicator(const char* label, ImU32 color, bool pulse = false);

    // =============================================
    // Cards / Sections
    // =============================================

    // Feature card (for feature toggles with icon)
    bool FeatureCard(const char* icon, const char* name, const char* description, bool* enabled);

    // Section header with optional collapse
    bool SectionHeader(const char* label, bool* collapsed = nullptr);

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
    void Separator();
    void SameLine(float offset = 0.0f, float spacing = -1.0f);
}
