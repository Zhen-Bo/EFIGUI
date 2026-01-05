#pragma once
#include "imgui.h"
#include <cstdint>
#include <optional>

namespace EFIGUI
{
    // =============================================
    // Theme Configuration Structures
    // =============================================
    // Runtime-configurable theme settings.
    // All values have sensible defaults (Cyberpunk theme).
    // Users can modify individual values or use preset themes.
    //
    // Design: Clean Code compliant nested structures.
    // Each component has its own theme struct (5-15 fields, high cohesion).
    // Access via Theme::Slider().height, Theme::Toggle().width, etc.

    // =============================================
    // Core Layout Types
    // =============================================
    // Unified types for spacing, padding, and margin.
    // These provide a consistent way to specify layout properties.

    // EdgeInsets: Represents padding or margin on all four edges
    struct EdgeInsets
    {
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
        float left = 0.0f;

        // Convenience constructors
        constexpr EdgeInsets() = default;
        constexpr EdgeInsets(float all) : top(all), right(all), bottom(all), left(all) {}
        constexpr EdgeInsets(float vertical, float horizontal)
            : top(vertical), right(horizontal), bottom(vertical), left(horizontal) {}
        constexpr EdgeInsets(float t, float r, float b, float l)
            : top(t), right(r), bottom(b), left(l) {}

        // Named constructors for common patterns
        static constexpr EdgeInsets All(float value) { return EdgeInsets(value); }
        static constexpr EdgeInsets Symmetric(float vertical, float horizontal)
        {
            return EdgeInsets(vertical, horizontal);
        }
        static constexpr EdgeInsets Only(float top = 0, float right = 0, float bottom = 0, float left = 0)
        {
            return EdgeInsets(top, right, bottom, left);
        }
        static constexpr EdgeInsets Horizontal(float value) { return EdgeInsets(0, value); }
        static constexpr EdgeInsets Vertical(float value) { return EdgeInsets(value, 0); }

        // Utility methods
        constexpr float horizontal() const { return left + right; }
        constexpr float vertical() const { return top + bottom; }
        constexpr ImVec2 topLeft() const { return ImVec2(left, top); }
        constexpr ImVec2 bottomRight() const { return ImVec2(right, bottom); }
        constexpr ImVec2 size() const { return ImVec2(horizontal(), vertical()); }
    };

    // Spacing: Represents spacing between elements (horizontal and vertical)
    struct Spacing
    {
        float x = 0.0f;  // Horizontal spacing
        float y = 0.0f;  // Vertical spacing

        constexpr Spacing() = default;
        constexpr Spacing(float both) : x(both), y(both) {}
        constexpr Spacing(float horizontal, float vertical) : x(horizontal), y(vertical) {}

        static constexpr Spacing Both(float value) { return Spacing(value); }

        constexpr ImVec2 toVec2() const { return ImVec2(x, y); }
    };

    // CornerRadius: Represents corner rounding for rounded rectangles
    struct CornerRadius
    {
        float topLeft = 0.0f;
        float topRight = 0.0f;
        float bottomLeft = 0.0f;
        float bottomRight = 0.0f;

        constexpr CornerRadius() = default;
        constexpr CornerRadius(float all)
            : topLeft(all), topRight(all), bottomLeft(all), bottomRight(all) {}
        constexpr CornerRadius(float top, float bottom)
            : topLeft(top), topRight(top), bottomLeft(bottom), bottomRight(bottom) {}
        constexpr CornerRadius(float tl, float tr, float bl, float br)
            : topLeft(tl), topRight(tr), bottomLeft(bl), bottomRight(br) {}

        static constexpr CornerRadius All(float value) { return CornerRadius(value); }
        static constexpr CornerRadius Top(float value) { return CornerRadius(value, 0); }
        static constexpr CornerRadius Bottom(float value) { return CornerRadius(0, value); }
        static constexpr CornerRadius Left(float value) { return CornerRadius(value, 0, value, 0); }
        static constexpr CornerRadius Right(float value) { return CornerRadius(0, value, 0, value); }
        static constexpr CornerRadius None() { return CornerRadius(0); }
        static constexpr CornerRadius Pill() { return CornerRadius(999.0f); }

        // For ImGui compatibility (single value)
        constexpr float uniform() const { return topLeft; }
        constexpr bool isUniform() const
        {
            return topLeft == topRight && topLeft == bottomLeft && topLeft == bottomRight;
        }
    };

    // BorderStyle: Represents border configuration
    struct BorderStyle
    {
        float width = 1.0f;
        ImU32 color = IM_COL32(60, 60, 80, 255);

        constexpr BorderStyle() = default;
        constexpr BorderStyle(float w, ImU32 c) : width(w), color(c) {}

        static constexpr BorderStyle None() { return BorderStyle(0, IM_COL32(0, 0, 0, 0)); }
    };

    // SizeConstraints: Min/max size constraints
    struct SizeConstraints
    {
        float minWidth = 0.0f;
        float maxWidth = 0.0f;   // 0 = no max
        float minHeight = 0.0f;
        float maxHeight = 0.0f;  // 0 = no max

        constexpr SizeConstraints() = default;
        constexpr SizeConstraints(float minW, float maxW, float minH, float maxH)
            : minWidth(minW), maxWidth(maxW), minHeight(minH), maxHeight(maxH) {}

        static constexpr SizeConstraints MinSize(float w, float h)
        {
            return SizeConstraints(w, 0, h, 0);
        }
        static constexpr SizeConstraints MaxSize(float w, float h)
        {
            return SizeConstraints(0, w, 0, h);
        }
        static constexpr SizeConstraints Fixed(float w, float h)
        {
            return SizeConstraints(w, w, h, h);
        }
    };

    // =============================================
    // Color Configuration
    // =============================================

    struct ThemeColors
    {
        // Background colors
        ImU32 backgroundDark      = IM_COL32(10, 10, 15, 255);      // Main window background
        ImU32 backgroundPanel     = IM_COL32(20, 20, 35, 200);      // Semi-transparent panel
        ImU32 backgroundNavbar    = IM_COL32(15, 15, 25, 240);      // Left navbar
        ImU32 backgroundContent   = IM_COL32(25, 25, 45, 180);      // Content area (glass)

        // Accent colors (Neon)
        ImU32 accentPrimary       = IM_COL32(0, 245, 255, 255);     // Cyan
        ImU32 accentSecondary     = IM_COL32(184, 41, 255, 255);    // Purple
        ImU32 accentTertiary      = IM_COL32(255, 41, 117, 255);    // Pink
        ImU32 accentSuccess       = IM_COL32(57, 255, 20, 255);     // Green

        // Accent with alpha (for glow effects) - derived from accent colors
        ImU32 accentPrimaryGlow   = IM_COL32(0, 245, 255, 80);
        ImU32 accentSecondaryGlow = IM_COL32(184, 41, 255, 80);

        // Text colors
        ImU32 textPrimary         = IM_COL32(255, 255, 255, 255);   // White
        ImU32 textSecondary       = IM_COL32(180, 180, 200, 255);   // Light gray
        ImU32 textMuted           = IM_COL32(100, 100, 120, 255);   // Muted
        ImU32 textAccent          = IM_COL32(0, 245, 255, 255);     // Cyan

        // Border colors
        ImU32 borderDefault       = IM_COL32(60, 60, 80, 255);
        ImU32 borderHover         = IM_COL32(0, 245, 255, 180);
        ImU32 borderActive        = IM_COL32(0, 245, 255, 255);

        // Button colors
        ImU32 buttonDefault       = IM_COL32(40, 40, 60, 200);
        ImU32 buttonHover         = IM_COL32(50, 50, 80, 220);
        ImU32 buttonActive        = IM_COL32(0, 180, 200, 255);

        // Title bar gradient
        ImU32 titleBarLeft        = IM_COL32(30, 30, 50, 255);
        ImU32 titleBarRight       = IM_COL32(50, 30, 70, 255);

        // Status colors
        ImU32 statusSuccess       = IM_COL32(57, 255, 20, 255);     // Green
        ImU32 statusWarning       = IM_COL32(255, 200, 0, 255);     // Yellow
        ImU32 statusError         = IM_COL32(255, 60, 60, 255);     // Red
        ImU32 statusInfo          = IM_COL32(0, 200, 255, 255);     // Cyan

        // Glassmorphism colors
        ImU32 glassBaseLayer      = IM_COL32(20, 20, 35, 0);        // Base layer (transparent)
        ImU32 glassOverlayActive  = IM_COL32(30, 30, 50, 200);      // Active state overlay
        ImU32 glassOverlayDefault = IM_COL32(20, 20, 35, 200);      // Default state overlay
        ImU32 glassOverlayHover   = IM_COL32(30, 30, 55, 200);      // Hover state overlay
    };

    // =============================================
    // Dimension Configuration
    // =============================================

    struct ThemeDimensions
    {
        // Rounding
        float windowRounding      = 12.0f;
        float frameRounding       = 8.0f;
        float buttonRounding      = 8.0f;
        float navItemRounding     = 10.0f;

        // Layout dimensions
        float titleBarHeight      = 36.0f;
        float navbarWidth         = 200.0f;
        float navbarWidthCollapsed = 65.0f;
        float navItemHeight       = 40.0f;
        float navItemPadding      = 8.0f;

        // Spacing
        float contentPadding      = 18.0f;
        float itemSpacing         = 8.0f;

        // Window constraints
        float windowMinWidth      = 700.0f;
        float windowMinHeight     = 550.0f;

        // Toggle dimensions
        float toggleWidth         = 48.0f;
        float toggleHeight        = 24.0f;
        float toggleKnobSize      = 18.0f;
        float toggleLabelGap      = 12.0f;

        // Font sizes
        float fontSizeDefault     = 16.0f;
        float fontSizeSmall       = 14.0f;
        float fontSizeLarge       = 18.0f;
        float fontSizeTitle       = 20.0f;
        float fontSizeIcon        = 18.0f;
    };

    // =============================================
    // Effect Configuration
    // =============================================

    struct ThemeEffects
    {
        // Glow parameters
        float glowIntensity       = 0.6f;
        float glowRadius          = 8.0f;

        // Button glow settings
        int   buttonGlowLayers    = 5;
        float buttonGlowExpand    = 2.0f;

        // Colored button glow (more subtle)
        int   coloredButtonGlowLayers = 4;
        float coloredButtonGlowExpand = 1.5f;

        // Slider glow settings
        int   sliderGlowLayers    = 4;
        float sliderKnobGlowExpand = 2.0f;
        float sliderInputGlowExpand = 1.5f;

        // Progress bar glow
        int   progressGlowLayers  = 4;
        float progressGlowExpand  = 1.5f;

        // Default alpha values
        uint8_t defaultOverlayAlpha     = 200;      // Borderless window overlay
        uint8_t defaultBgAlpha          = 30;       // Icon button hover background
        uint8_t defaultButtonBgAlpha    = 200;      // Colored button background
        uint8_t defaultButtonHoverAlpha = 220;      // Colored button hover
        float   defaultDotSize          = 8.0f;     // Status indicator dot size
        float   defaultGlowMaxAlpha     = 75.0f;    // Glow animation max alpha delta
        float   defaultGlowBaseAlpha    = 180.0f;   // Glow animation base alpha
    };

    // =============================================
    // Animation Configuration
    // =============================================

    struct ThemeAnimation
    {
        float defaultSpeed        = 8.0f;           // Default animation speed
        float pulseFrequency      = 2.0f;           // Pulse effect frequency
        float breatheFrequency    = 1.0f;           // Breathing glow frequency
        float sweepFrequency      = 0.8f;           // Marquee sweep frequency

        // Per-widget animation speeds (multiplier applied to defaultSpeed)
        float toggleSpeedMultiplier = 1.5f;         // Toggle animation (12.0 / 8.0)
        float clickDecaySpeed     = 3.0f;           // Click animation decay

        // Marquee border settings
        float marqueeSpeed        = 0.12f;          // Sweep position increment per frame
        float marqueeSweepLength  = 0.22f;          // Fraction of perimeter that glows
    };

    // =============================================
    // Component-Specific Theme Structures
    // =============================================
    // Each struct contains only the parameters for one component type.
    // High cohesion: 5-15 related fields per struct.

    struct SliderTheme
    {
        // Layout
        float height = 24.0f;               // Total slider height
        EdgeInsets padding = EdgeInsets();  // Padding around slider area
        float labelGap = 12.0f;             // Gap between label and slider

        // Track
        float trackHeight = 6.0f;           // Track bar height
        float trackRounding = 3.0f;         // Track corner rounding (half of trackHeight)

        // Knob
        float knobRadius = 8.0f;            // Knob radius

        // Colors
        ImU32 trackBgColor = IM_COL32(40, 40, 60, 200);
        ImU32 trackFillStart = IM_COL32(184, 41, 255, 255);   // Purple
        ImU32 trackFillEnd = IM_COL32(0, 245, 255, 255);      // Cyan
        ImU32 knobDefaultColor = IM_COL32(255, 255, 255, 255);
        ImU32 knobActiveColor = IM_COL32(0, 245, 255, 255);

        // Input box (for sliders with numeric input)
        float inputWidth = 55.0f;           // Input box width
        float inputHeight = 22.0f;          // Input box height
        float inputGap = 8.0f;              // Gap between slider and input
        float inputRounding = 6.0f;         // Input box corner rounding

        // Effects
        int glowLayers = 4;                 // Number of glow layers
        float knobGlowExpand = 2.0f;        // Knob glow expansion per layer
        float inputGlowExpand = 1.5f;       // Input glow expansion per layer
    };

    struct ToggleTheme
    {
        // Layout
        float width = 48.0f;                // Toggle track width
        float height = 24.0f;               // Toggle track height
        float knobSize = 18.0f;             // Knob diameter
        float labelGap = 12.0f;             // Gap between toggle and label
        EdgeInsets margin = EdgeInsets();   // Margin around toggle

        // Colors
        ImU32 trackOnColor = IM_COL32(0, 245, 255, 255);
        ImU32 trackOffColor = IM_COL32(60, 60, 80, 255);
        ImU32 knobColor = IM_COL32(255, 255, 255, 255);
        ImU32 glowColor = IM_COL32(0, 245, 255, 80);

        // Effects
        float animSpeed = 12.0f;            // Slide animation speed
        float glowThreshold = 0.1f;         // Min slideAnim to show glow
        float glowRadius = 4.0f;            // Glow effect radius
        float disabledAlpha = 0.4f;         // Alpha for disabled state

        // Description text positioning
        float descOffsetX = 60.0f;          // Description X offset
        float descSpacingY = 4.0f;          // Spacing after description
    };

    struct CardTheme
    {
        // Layout - Unified padding approach
        EdgeInsets padding = EdgeInsets(10.0f, 12.0f, 12.0f, 12.0f);  // Top, Right, Bottom, Left
        EdgeInsets margin = EdgeInsets();   // External margin around card
        float rounding = 8.0f;              // Corner rounding

        // Content spacing
        float titleDescGap = 2.0f;          // Gap between title and description
        float contentSpacing = 8.0f;        // Spacing between content elements

        // Colors
        ImU32 bgColor = IM_COL32(30, 30, 50, 200);
        ImU32 borderColor = IM_COL32(60, 60, 80, 255);
        ImU32 titleColor = IM_COL32(255, 255, 255, 255);
        ImU32 descColor = IM_COL32(180, 180, 200, 255);

        // Icon configuration
        float iconSize = 32.0f;             // Icon display size
        float iconPadding = 12.0f;          // Padding around icon
        float iconTextOffset = 6.0f;        // Icon text offset within icon area
        float minTextWidth = 50.0f;         // Minimum width for text

        // Legacy parameters (deprecated, kept for backward compatibility)
        float topPadding = 10.0f;           // deprecated: use padding.top
        float bottomPadding = 12.0f;        // deprecated: use padding.bottom
        float baseHeight = 0.0f;            // deprecated: set to 0 to use dynamic calculation
        float nameOffsetY = 10.0f;          // deprecated: used when baseHeight > 0
        float descOffsetY = 28.0f;          // deprecated: used when baseHeight > 0

        // Inline toggle dimensions
        float toggleWidth = 40.0f;          // Toggle track width
        float toggleHeight = 24.0f;         // Toggle track height
        float toggleRounding = 12.0f;       // Toggle corner rounding
        float toggleRightMargin = 48.0f;    // Toggle right margin from card edge
        float knobRadius = 9.0f;            // Toggle knob radius
        float knobPadding = 3.0f;           // Padding between knob and track
        float knobTravel = 16.0f;           // Knob travel distance
        float toggleAnimSpeed = 12.0f;      // Toggle animation speed

        // Section header
        float sectionLineOffset = 4.0f;     // Line offset below section text
        float sectionSpacing = 12.0f;       // Spacing below section header
    };

    struct NavTheme
    {
        // Layout
        EdgeInsets itemPadding = EdgeInsets::Symmetric(0, 12.0f);  // Vertical, horizontal padding
        float itemHeight = 40.0f;           // Nav item height
        float itemRounding = 10.0f;         // Nav item corner rounding
        float iconPadding = 12.0f;          // Horizontal icon padding (legacy)
        float labelSpacing = 12.0f;         // Spacing between icon and label
        float collapsedIconScale = 1.25f;   // Icon scale when collapsed

        // Colors
        ImU32 itemBgHover = IM_COL32(50, 50, 80, 60);
        ImU32 itemBgSelected = IM_COL32(0, 245, 255, 30);
        ImU32 textDefault = IM_COL32(180, 180, 200, 255);
        ImU32 textHover = IM_COL32(255, 255, 255, 255);
        ImU32 textSelected = IM_COL32(0, 245, 255, 255);

        // Accent bar (selected indicator)
        float accentBarWidth = 3.0f;        // Width of left accent bar
        float accentBarHeightRatio = 0.6f;  // Height ratio of item height
        float accentBarGlowRadius = 4.0f;   // Glow radius for accent bar

        // Section header
        EdgeInsets sectionPadding = EdgeInsets::Symmetric(4.0f, 12.0f);
        float sectionHeight = 24.0f;        // Section header total height
        ImU32 sectionColor = IM_COL32(100, 100, 120, 255);

        // Alpha values
        int accentGlowAlpha = 80;           // Alpha for accent glow
        int bgAlphaMultiplier = 60;         // Multiplier for bg alpha
        int hoverBgAlpha = 30;              // Alpha for hover background

        // Legacy accessors (deprecated)
        float sectionPaddingX = 12.0f;
        float sectionPaddingY = 4.0f;
    };

    struct ButtonTheme
    {
        // Layout
        EdgeInsets padding = EdgeInsets::Symmetric(0, 32.0f);  // Vertical, horizontal padding
        float height = 36.0f;               // Default button height
        float rounding = 8.0f;              // Corner rounding
        float minWidth = 0.0f;              // Minimum button width (0 = auto)

        // Legacy accessor for backward compatibility
        float paddingX = 32.0f;             // Horizontal padding for auto-sized (deprecated, use padding)

        // Colors
        ImU32 bgDefault = IM_COL32(40, 40, 60, 200);
        ImU32 bgHover = IM_COL32(50, 50, 80, 220);
        ImU32 bgActive = IM_COL32(0, 180, 200, 255);
        ImU32 textColor = IM_COL32(255, 255, 255, 255);
        ImU32 glowColor = IM_COL32(0, 245, 255, 255);  // Default glow color

        // Glow effects
        float glowIntensityBase = 0.6f;     // Base glow intensity when hovered
        float glowIntensityActive = 0.4f;   // Additional glow when active
        int glowLayers = 5;                 // Number of glow layers
        float glowExpand = 2.0f;            // Glow expansion per layer

        // Colored button glow (more subtle)
        int coloredButtonGlowLayers = 4;
        float coloredButtonGlowExpand = 1.5f;

        // Cooldown button specific
        float cooldownThreshold = 0.01f;    // Threshold for cooldown detection
        int cooldownOverlayAlpha = 180;     // Alpha for cooldown overlay
        int edgeGlowLayers = 4;             // Number of edge glow layers
        float edgeGlowBaseWidth = 2.0f;     // Base width for edge glow
        float edgeGlowExpandStep = 3.0f;    // Width expansion per layer
        float edgeGlowBaseAlpha = 0.4f;     // Base alpha for edge glow
        float particleRadiusSmall = 3.0f;   // Small particle radius
        float particleRadiusLarge = 6.0f;   // Large particle radius
        int particleAlphaBright = 220;      // Bright particle alpha
        int particleAlphaDim = 100;         // Dim particle alpha

        // Icon button specific
        EdgeInsets iconPadding = EdgeInsets::All(4.0f);  // Padding around icon
        float iconBgAlpha = 30;             // Background alpha on hover
    };

    struct WindowTheme
    {
        // Layout
        float rounding = 12.0f;             // Window corner rounding
        EdgeInsets padding = EdgeInsets::All(0);  // Window content padding

        // Title bar
        float titleBarHeight = 36.0f;       // Title bar height
        EdgeInsets titleBarPadding = EdgeInsets::Symmetric(0, 16.0f);
        float titleButtonSize = 24.0f;      // Close/minimize button size
        float titleButtonMargin = 12.0f;    // Right margin for close button
        float titleButtonSpacing = 8.0f;    // Spacing between title buttons

        // Colors
        ImU32 titleBarLeft = IM_COL32(30, 30, 50, 255);
        ImU32 titleBarRight = IM_COL32(50, 30, 70, 255);
        ImU32 titleTextColor = IM_COL32(255, 255, 255, 255);
        ImU32 overlayColor = IM_COL32(20, 20, 35, 200);

        // Legacy accessors (deprecated)
        float titleTextPadding = 16.0f;
        float contentSpacingY = 8.0f;       // Vertical spacing after elements

        // Navbar header
        float navbarButtonSize = 20.0f;     // Navbar collapse button size
        float navbarIconPadding = 8.0f;     // Padding around navbar icons
        float navbarButtonPadding = 4.0f;   // Padding for collapse button

        // Glow effect
        float glowRadiusBase = 4.0f;        // Base glow radius
        float glowRadiusExpand = 2.0f;      // Glow radius expansion on animation
        int glowAnimAlpha = 60;             // Alpha for glow animation
    };

    struct DrawTheme
    {
        // RectGlow
        float glowMinIntensity  = 0.01f;            // Minimum intensity to draw glow
        int   glowLayerCount    = 4;                // Number of glow layers
        float glowAlphaMultiplier = 0.2f;           // Alpha multiplier per layer
        float glowLineThickness = 2.0f;             // Line thickness for glow

        // NeonBorder
        float neonGlowIntensity = 0.5f;             // Neon border glow intensity
        float neonGlowRadius    = 4.0f;             // Neon border glow radius

        // GlowLayers
        float glowLayersRectAlpha = 0.1f;           // Rect glow layer alpha
        float glowLayersCircleAlpha = 0.15f;        // Circle glow layer alpha

        // MarqueeBorder
        int   marqueeNumSegments = 80;              // Total number of segments
        int   marqueeCornerSegments = 8;            // Segments per corner
        int   marqueeMinEdgeSegments = 2;           // Minimum segments per edge
        int   marqueeMinAlpha   = 25;               // Minimum alpha value
        float marqueeHoverThreshold = 0.1f;         // Hover intensity threshold

        // Glassmorphism
        int   glassBlurAlpha    = 200;              // Blur alpha value
    };

    struct LayoutTheme
    {
        // Global spacing
        Spacing itemSpacing = Spacing(8.0f);          // Default spacing between items
        Spacing innerSpacing = Spacing(4.0f);         // Inner spacing within components

        // Separator
        float separatorOffsetY = 4.0f;      // Y offset for separator line
        float separatorSpacing = 8.0f;      // Total vertical spacing
        float separatorThickness = 1.0f;    // Separator line thickness
        ImU32 separatorColor = IM_COL32(60, 60, 80, 255);

        // Tooltip
        EdgeInsets tooltipPadding = EdgeInsets::Symmetric(6.0f, 8.0f);
        float tooltipRounding = 4.0f;       // Tooltip corner rounding
        ImU32 tooltipBgColor = IM_COL32(30, 30, 50, 240);
        ImU32 tooltipBorderColor = IM_COL32(60, 60, 80, 255);
        ImU32 tooltipTextColor = IM_COL32(255, 255, 255, 255);

        // Content area
        EdgeInsets contentPadding = EdgeInsets::All(18.0f);

        // Legacy accessors (deprecated)
        float tooltipPaddingX = 8.0f;
        float tooltipPaddingY = 6.0f;
    };

    struct NumericInputTheme
    {
        // Dimensions
        float defaultWidth      = 55.0f;            // Default input width
        float defaultHeight     = 22.0f;            // Default input height
        float rounding          = 6.0f;             // Corner rounding
        float inset             = 2.0f;             // Inset for screen area
        float paddingX          = 6.0f;             // Horizontal text padding

        // Effects
        float glowExpandSize    = 4.0f;             // Glow expansion size
        float hoverThreshold    = 0.1f;             // Threshold for hover effects
        float glowThreshold     = 0.05f;            // Threshold for glow effects
        float editingGlowIntensity = 0.7f;          // Glow intensity when editing

        // Bezel/Frame colors
        ImU32 bezelColor        = IM_COL32(25, 25, 35, 255);       // Bezel/frame color

        // Screen background colors
        ImU32 screenBgDefault   = IM_COL32(15, 15, 25, 255);       // Default background
        ImU32 screenBgHover     = IM_COL32(20, 25, 35, 255);       // Hover background
        ImU32 screenBgEditing   = IM_COL32(20, 30, 40, 255);       // Editing background

        // Inner border (bevel effect)
        ImU32 innerBorderTop    = IM_COL32(10, 10, 15, 255);       // Top inner border
        ImU32 innerBorderBottom = IM_COL32(50, 50, 70, 100);       // Bottom inner border

        // Outer border
        ImU32 outerBorderDefault = IM_COL32(60, 60, 80, 255);      // Default border
        ImU32 outerBorderHover  = IM_COL32(80, 100, 120, 255);     // Hover border

        // Text
        ImU32 textDefault       = IM_COL32(180, 200, 210, 255);    // Default text color
    };

    // =============================================
    // Complete Theme Configuration
    // =============================================

    struct ThemeConfig
    {
        ThemeColors colors;
        ThemeDimensions dimensions;
        ThemeEffects effects;
        ThemeAnimation animation;

        // Component-specific theme structures
        SliderTheme slider;
        ToggleTheme toggle;
        CardTheme card;
        NavTheme nav;
        ButtonTheme button;
        WindowTheme window;
        DrawTheme draw;
        LayoutTheme layout;
        NumericInputTheme numericInput;
    };

    // =============================================
    // Theme Presets (Extensible)
    // =============================================
    // Currently only Cyberpunk is implemented.
    // Designed for future expansion without modifying core architecture.

    enum class ThemePreset
    {
        Cyberpunk       // Default neon cyan/purple theme
        // Future: Dark, Light, Ocean, etc.
    };

    // =============================================
    // Convenience Enums
    // =============================================

    enum class RoundingStyle
    {
        Sharp,          // 0px - No rounding
        Subtle,         // 4px - Subtle rounding
        Rounded,        // 8px - Default rounded
        Pill            // 999px - Pill-shaped
    };

    enum class AnimSpeed
    {
        Instant,        // 0x multiplier (immediate)
        Fast,           // 2x multiplier
        Normal,         // 1x multiplier (default)
        Slow,           // 0.5x multiplier
        Relaxed         // 0.25x multiplier
    };
}
