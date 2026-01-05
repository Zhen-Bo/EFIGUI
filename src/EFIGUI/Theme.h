#pragma once
#include "imgui.h"
#include "ThemeConfig.h"
#include <cstdint>
#include <optional>

namespace EFIGUI
{
    // =============================================
    // Resolve Helper (Optional Fallback Utility)
    // =============================================
    // Simplifies optional-to-default fallback syntax.
    // Usage: Resolve(config.height, Theme::Slider().height)

    template<typename T>
    inline const T& Resolve(const std::optional<T>& opt, const T& defaultVal)
    {
        return opt.has_value() ? opt.value() : defaultVal;
    }

    // Non-optional overload (returns value directly)
    template<typename T>
    inline const T& Resolve(const T& value, const T& /*unused*/)
    {
        return value;
    }

    // =============================================
    // Configurable Icons
    // =============================================
    // Users can override these default icons before calling EFIGUI functions
    // Example: EFIGUI::Icons::Close = "\xef\x80\x8d"; // Font Awesome xmark

    namespace Icons
    {
        // Window control icons
        inline const char* Close       = "X";           // Window close button
        inline const char* Minimize    = "-";           // Window minimize button
        inline const char* Collapse    = "<<";          // Navbar collapse button
        inline const char* Expand      = ">>";          // Navbar expand button

        // Navigation icons
        inline const char* ChevronLeft  = "<";
        inline const char* ChevronRight = ">";
        inline const char* Bars         = "=";          // Menu/hamburger icon
        inline const char* Home         = "H";          // Home icon
        inline const char* Cog          = "*";          // Settings/gear icon
        inline const char* User         = "U";          // User/profile icon

        // Common UI icons
        inline const char* Check       = "v";           // Checkmark
        inline const char* Warning     = "!";           // Warning/exclamation
        inline const char* Info        = "i";           // Information
        inline const char* Error       = "x";           // Error/cross
        inline const char* Eye         = "o";           // Eye/visibility
        inline const char* Shield      = "S";           // Shield/protection
        inline const char* Bolt        = "B";           // Lightning bolt/performance
    }

    // =============================================
    // Theme Manager (Singleton)
    // =============================================
    // Provides runtime-configurable theme settings.
    // Access via Theme::GetConfig() or use convenience accessors.

    class ThemeManager
    {
    public:
        // Singleton access
        static ThemeManager& Instance();

        // Configuration access
        ThemeConfig& GetConfig() { return m_config; }
        const ThemeConfig& GetConfig() const { return m_config; }

        // Replace entire configuration
        void SetConfig(const ThemeConfig& config) { m_config = config; }

        // Reset to default (Cyberpunk theme)
        void ResetToDefault() { m_config = ThemeConfig{}; }

        // Load preset theme
        void LoadPreset(ThemePreset preset);

        // Convenience accessors for individual sections
        ThemeColors& Colors() { return m_config.colors; }
        ThemeDimensions& Dimensions() { return m_config.dimensions; }
        ThemeEffects& Effects() { return m_config.effects; }
        ThemeAnimation& Animation() { return m_config.animation; }

        const ThemeColors& Colors() const { return m_config.colors; }
        const ThemeDimensions& Dimensions() const { return m_config.dimensions; }
        const ThemeEffects& Effects() const { return m_config.effects; }
        const ThemeAnimation& Animation() const { return m_config.animation; }

    private:
        ThemeManager() = default;
        ThemeManager(const ThemeManager&) = delete;
        ThemeManager& operator=(const ThemeManager&) = delete;

        ThemeConfig m_config;
    };

    namespace Theme
    {
        // =============================================
        // Configuration Access
        // =============================================

        // Get the current theme configuration (mutable)
        inline ThemeConfig& GetConfig() { return ThemeManager::Instance().GetConfig(); }

        // Get the current theme configuration (const)
        inline const ThemeConfig& GetConfigConst() { return ThemeManager::Instance().GetConfig(); }

        // Set entire theme configuration
        inline void SetConfig(const ThemeConfig& config) { ThemeManager::Instance().SetConfig(config); }

        // Reset to default Cyberpunk theme
        inline void ResetToDefault() { ThemeManager::Instance().ResetToDefault(); }

        // Load a preset theme
        inline void LoadPreset(ThemePreset preset) { ThemeManager::Instance().LoadPreset(preset); }

        // =============================================
        // Component Theme Accessors (Recommended)
        // =============================================
        // Access component-specific theme configurations.
        // Usage: Theme::Slider().height, Theme::Toggle().width

        inline const SliderTheme& Slider() { return GetConfigConst().slider; }
        inline const ToggleTheme& Toggle() { return GetConfigConst().toggle; }
        inline const CardTheme& Card() { return GetConfigConst().card; }
        inline const NavTheme& Nav() { return GetConfigConst().nav; }
        inline const ButtonTheme& Button() { return GetConfigConst().button; }
        inline const WindowTheme& Window() { return GetConfigConst().window; }
        inline const DrawTheme& Draw() { return GetConfigConst().draw; }
        inline const LayoutTheme& Layout() { return GetConfigConst().layout; }
        inline const NumericInputTheme& NumericInput() { return GetConfigConst().numericInput; }

        // Mutable accessors for runtime modification
        inline SliderTheme& SliderMut() { return GetConfig().slider; }
        inline ToggleTheme& ToggleMut() { return GetConfig().toggle; }
        inline CardTheme& CardMut() { return GetConfig().card; }
        inline NavTheme& NavMut() { return GetConfig().nav; }
        inline ButtonTheme& ButtonMut() { return GetConfig().button; }
        inline WindowTheme& WindowMut() { return GetConfig().window; }
        inline DrawTheme& DrawMut() { return GetConfig().draw; }
        inline LayoutTheme& LayoutMut() { return GetConfig().layout; }
        inline NumericInputTheme& NumericInputMut() { return GetConfig().numericInput; }

        // =============================================
        // Color Accessors (Backward Compatible)
        // =============================================
        // These accessors maintain API compatibility with existing code
        // that uses Theme::AccentCyan, Theme::BackgroundDark, etc.

        // Background colors
        inline ImU32 BackgroundDark()    { return GetConfigConst().colors.backgroundDark; }
        inline ImU32 BackgroundPanel()   { return GetConfigConst().colors.backgroundPanel; }
        inline ImU32 BackgroundNavbar()  { return GetConfigConst().colors.backgroundNavbar; }
        inline ImU32 BackgroundContent() { return GetConfigConst().colors.backgroundContent; }

        // Accent colors (Neon)
        inline ImU32 AccentCyan()        { return GetConfigConst().colors.accentPrimary; }
        inline ImU32 AccentPurple()      { return GetConfigConst().colors.accentSecondary; }
        inline ImU32 AccentPink()        { return GetConfigConst().colors.accentTertiary; }
        inline ImU32 AccentGreen()       { return GetConfigConst().colors.accentSuccess; }

        // Accent with alpha (for glow effects)
        inline ImU32 AccentCyanGlow()    { return GetConfigConst().colors.accentPrimaryGlow; }
        inline ImU32 AccentPurpleGlow()  { return GetConfigConst().colors.accentSecondaryGlow; }

        // Text colors
        inline ImU32 TextPrimary()       { return GetConfigConst().colors.textPrimary; }
        inline ImU32 TextSecondary()     { return GetConfigConst().colors.textSecondary; }
        inline ImU32 TextMuted()         { return GetConfigConst().colors.textMuted; }
        inline ImU32 TextAccent()        { return GetConfigConst().colors.textAccent; }

        // Border colors
        inline ImU32 BorderDefault()     { return GetConfigConst().colors.borderDefault; }
        inline ImU32 BorderHover()       { return GetConfigConst().colors.borderHover; }
        inline ImU32 BorderActive()      { return GetConfigConst().colors.borderActive; }

        // Button colors
        inline ImU32 ButtonDefault()     { return GetConfigConst().colors.buttonDefault; }
        inline ImU32 ButtonHover()       { return GetConfigConst().colors.buttonHover; }
        inline ImU32 ButtonActive()      { return GetConfigConst().colors.buttonActive; }

        // Title bar gradient
        inline ImU32 TitleBarLeft()      { return GetConfigConst().colors.titleBarLeft; }
        inline ImU32 TitleBarRight()     { return GetConfigConst().colors.titleBarRight; }

        // Status colors
        inline ImU32 StatusSuccess()     { return GetConfigConst().colors.statusSuccess; }
        inline ImU32 StatusWarning()     { return GetConfigConst().colors.statusWarning; }
        inline ImU32 StatusError()       { return GetConfigConst().colors.statusError; }
        inline ImU32 StatusInfo()        { return GetConfigConst().colors.statusInfo; }

        // Glassmorphism colors
        inline ImU32 GlassBaseLayer()      { return GetConfigConst().colors.glassBaseLayer; }
        inline ImU32 GlassOverlayActive()  { return GetConfigConst().colors.glassOverlayActive; }
        inline ImU32 GlassOverlayDefault() { return GetConfigConst().colors.glassOverlayDefault; }
        inline ImU32 GlassOverlayHover()   { return GetConfigConst().colors.glassOverlayHover; }

        // =============================================
        // Dimension Accessors (Backward Compatible)
        // =============================================

        inline float WindowRounding()       { return GetConfigConst().dimensions.windowRounding; }
        inline float FrameRounding()        { return GetConfigConst().dimensions.frameRounding; }
        inline float ButtonRounding()       { return GetConfigConst().dimensions.buttonRounding; }
        inline float NavItemRounding()      { return GetConfigConst().dimensions.navItemRounding; }

        inline float TitleBarHeight()       { return GetConfigConst().dimensions.titleBarHeight; }
        inline float NavbarWidth()          { return GetConfigConst().dimensions.navbarWidth; }
        inline float NavbarWidthCollapsed() { return GetConfigConst().dimensions.navbarWidthCollapsed; }
        inline float NavItemHeight()        { return GetConfigConst().dimensions.navItemHeight; }
        inline float NavItemPadding()       { return GetConfigConst().dimensions.navItemPadding; }

        inline float ContentPadding()       { return GetConfigConst().dimensions.contentPadding; }
        inline float ItemSpacing()          { return GetConfigConst().dimensions.itemSpacing; }

        inline float WindowMinWidth()       { return GetConfigConst().dimensions.windowMinWidth; }
        inline float WindowMinHeight()      { return GetConfigConst().dimensions.windowMinHeight; }

        inline float ToggleWidth()          { return GetConfigConst().dimensions.toggleWidth; }
        inline float ToggleHeight()         { return GetConfigConst().dimensions.toggleHeight; }
        inline float ToggleKnobSize()       { return GetConfigConst().dimensions.toggleKnobSize; }
        inline float ToggleLabelGap()       { return GetConfigConst().dimensions.toggleLabelGap; }

        // Font sizes
        inline float FontSizeDefault()      { return GetConfigConst().dimensions.fontSizeDefault; }
        inline float FontSizeSmall()        { return GetConfigConst().dimensions.fontSizeSmall; }
        inline float FontSizeLarge()        { return GetConfigConst().dimensions.fontSizeLarge; }
        inline float FontSizeTitle()        { return GetConfigConst().dimensions.fontSizeTitle; }
        inline float FontSizeIcon()         { return GetConfigConst().dimensions.fontSizeIcon; }

        // =============================================
        // Effect Accessors (Backward Compatible)
        // =============================================

        inline float GlowIntensity()        { return GetConfigConst().effects.glowIntensity; }
        inline float GlowRadius()           { return GetConfigConst().effects.glowRadius; }

        // Button glow settings
        inline int   ButtonGlowLayers()     { return GetConfigConst().effects.buttonGlowLayers; }
        inline float ButtonGlowExpand()     { return GetConfigConst().effects.buttonGlowExpand; }

        // Colored button glow settings
        inline int   ColoredButtonGlowLayers() { return GetConfigConst().effects.coloredButtonGlowLayers; }
        inline float ColoredButtonGlowExpand() { return GetConfigConst().effects.coloredButtonGlowExpand; }

        // Slider glow settings
        inline int   SliderGlowLayers()     { return GetConfigConst().effects.sliderGlowLayers; }
        inline float SliderKnobGlowExpand() { return GetConfigConst().effects.sliderKnobGlowExpand; }
        inline float SliderInputGlowExpand(){ return GetConfigConst().effects.sliderInputGlowExpand; }

        // Progress bar glow settings
        inline int   ProgressGlowLayers()   { return GetConfigConst().effects.progressGlowLayers; }
        inline float ProgressGlowExpand()   { return GetConfigConst().effects.progressGlowExpand; }

        // Default alpha values
        inline uint8_t DefaultOverlayAlpha()     { return GetConfigConst().effects.defaultOverlayAlpha; }
        inline uint8_t DefaultBgAlpha()          { return GetConfigConst().effects.defaultBgAlpha; }
        inline uint8_t DefaultButtonBgAlpha()    { return GetConfigConst().effects.defaultButtonBgAlpha; }
        inline uint8_t DefaultButtonHoverAlpha() { return GetConfigConst().effects.defaultButtonHoverAlpha; }
        inline float   DefaultDotSize()          { return GetConfigConst().effects.defaultDotSize; }
        inline float   DefaultGlowMaxAlpha()     { return GetConfigConst().effects.defaultGlowMaxAlpha; }
        inline float   DefaultGlowBaseAlpha()    { return GetConfigConst().effects.defaultGlowBaseAlpha; }

        // =============================================
        // Animation Accessors (Backward Compatible)
        // =============================================

        inline float AnimationSpeed()       { return GetConfigConst().animation.defaultSpeed; }
        inline float PulseFrequency()       { return GetConfigConst().animation.pulseFrequency; }

        // =============================================
        // ImVec4 Conversion Utility
        // =============================================

        inline ImVec4 ToVec4(ImU32 color) {
            return ImGui::ColorConvertU32ToFloat4(color);
        }

        // =============================================
        // Color Utilities
        // =============================================

        // RGB component structure for color manipulation
        struct ColorRGB
        {
            uint8_t r, g, b;
        };

        // Extract RGB components from ImU32 color (ignores alpha)
        inline ColorRGB ExtractRGB(ImU32 color)
        {
            return {
                static_cast<uint8_t>((color >> 0) & 0xFF),
                static_cast<uint8_t>((color >> 8) & 0xFF),
                static_cast<uint8_t>((color >> 16) & 0xFF)
            };
        }

        // Create ImU32 color from RGB components and alpha
        inline ImU32 MakeColor(const ColorRGB& rgb, uint8_t alpha)
        {
            return IM_COL32(rgb.r, rgb.g, rgb.b, alpha);
        }

        // Apply new alpha to existing color (preserves RGB)
        inline ImU32 ApplyAlpha(ImU32 color, uint8_t alpha)
        {
            return (color & 0x00FFFFFF) | (static_cast<ImU32>(alpha) << 24);
        }

        // Extract alpha from ImU32 color
        inline uint8_t ExtractAlpha(ImU32 color)
        {
            return static_cast<uint8_t>((color >> 24) & 0xFF);
        }

        // =============================================
        // Theme initialization
        // =============================================

        // Apply EFIGUI theme to ImGui
        void Apply();

        // Apply custom colors (override defaults) - Legacy API
        // Note: Prefer using GetConfig().colors.accentPrimary = ... directly
        void SetAccentColor(ImU32 primary, ImU32 secondary = 0);

        // =============================================
        // Debug Tools
        // =============================================

        // Show Theme Style Editor window for debugging and live tuning
        // Usage: if (showEditor) Theme::ShowThemeStyleEditor(&showEditor);
        void ShowThemeStyleEditor(bool* p_open = nullptr);
    }
}
