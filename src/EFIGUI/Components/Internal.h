#pragma once
// =============================================
// EFIGUI Components - Internal Shared Header
// This file contains shared includes, state, and helpers
// used across all component implementation files.
// =============================================

#include "../Components.h"
#include "../Draw.h"
#include "../EFIGUI.h"
#include "imgui_internal.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <optional>

namespace EFIGUI
{
    // =============================================
    // Button Constants
    // =============================================
    // Centralized constants for button rendering.
    // Avoids magic numbers and enables easy tuning.

    namespace ButtonConstants
    {
        // Default button size
        constexpr float DefaultButtonPaddingX = 32.0f;    // Horizontal padding for auto-sized buttons
        constexpr float DefaultButtonHeight = 36.0f;      // Default button height

        // GlowButton constants
        constexpr float GlowIntensityBase = 0.6f;         // Base glow intensity when hovered
        constexpr float GlowIntensityActive = 0.4f;       // Additional glow intensity when active

        // CooldownButton constants
        constexpr float CooldownThreshold = 0.01f;        // Threshold for cooldown detection
        constexpr int CooldownOverlayAlpha = 180;         // Alpha for cooldown dark overlay
        constexpr int EdgeGlowLayers = 4;                 // Number of edge glow layers
        constexpr float EdgeGlowBaseWidth = 2.0f;         // Base width for edge glow
        constexpr float EdgeGlowExpandStep = 3.0f;        // Width expansion per layer
        constexpr float EdgeGlowBaseAlpha = 0.4f;         // Base alpha for edge glow
        constexpr float ParticleRadiusSmall = 3.0f;       // Small particle radius
        constexpr float ParticleRadiusLarge = 6.0f;       // Large particle radius
        constexpr int ParticleAlphaBright = 220;          // Bright particle alpha
        constexpr int ParticleAlphaDim = 100;             // Dim particle alpha
    }

    // =============================================
    // Window Constants
    // =============================================
    // Centralized constants for window rendering.
    // Avoids magic numbers and enables easy tuning.

    namespace WindowConstants
    {
        // Title bar layout
        constexpr float TitleTextPadding = 16.0f;         // Left padding for title text
        constexpr float TitleButtonSize = 24.0f;          // Size of close/minimize buttons
        constexpr float TitleButtonMargin = 12.0f;        // Right margin for close button
        constexpr float TitleButtonSpacing = 8.0f;        // Spacing between title bar buttons
        constexpr float ContentSpacingY = 8.0f;           // Vertical spacing after elements

        // NavbarHeader layout
        constexpr float NavbarButtonSize = 20.0f;         // Size of navbar collapse button
        constexpr float NavbarIconPadding = 8.0f;         // Padding around navbar icons
        constexpr float NavbarButtonPadding = 4.0f;       // Padding for collapse button

        // Glow effect
        constexpr float GlowRadiusBase = 4.0f;            // Base glow radius
        constexpr float GlowRadiusExpand = 2.0f;          // Glow radius expansion on animation
        constexpr int GlowAnimAlpha = 60;                 // Alpha for glow animation
    }

    // =============================================
    // Card Constants
    // =============================================
    // Centralized constants for card rendering.
    // Avoids magic numbers and enables easy tuning.

    namespace CardConstants
    {
        // Layout dimensions
        constexpr float IconSize = 32.0f;                 // Icon display size
        constexpr float IconPadding = 12.0f;              // Padding around icon
        constexpr float IconTextOffset = 6.0f;            // Icon text offset within icon area
        constexpr float BaseHeight = 56.0f;               // Base card height
        constexpr float MinTextWidth = 50.0f;             // Minimum width for text wrapping
        constexpr float NameOffsetY = 10.0f;              // Name text Y offset from top
        constexpr float DescOffsetY = 28.0f;              // Description text Y offset from top

        // Toggle dimensions (inline toggle within card)
        constexpr float ToggleWidth = 40.0f;              // Toggle track width
        constexpr float ToggleHeight = 24.0f;             // Toggle track height
        constexpr float ToggleRounding = 12.0f;           // Toggle corner rounding
        constexpr float ToggleRightMargin = 48.0f;        // Toggle right margin from card edge
        constexpr float KnobRadius = 9.0f;                // Toggle knob radius
        constexpr float KnobPadding = 3.0f;               // Padding between knob and track edge
        constexpr float KnobTravel = 16.0f;               // Knob travel distance

        // Animation
        constexpr float ToggleAnimSpeed = 12.0f;          // Toggle slide animation speed

        // Section header
        constexpr float SectionLineOffset = 4.0f;         // Line offset below section header text
        constexpr float SectionSpacing = 12.0f;           // Spacing below section header
    }

    // =============================================
    // Navigation Constants
    // =============================================
    // Centralized constants for navigation item rendering.
    // Avoids magic numbers and enables easy tuning.

    namespace NavConstants
    {
        // Layout
        constexpr float IconPadding = 12.0f;              // Horizontal padding for icon
        constexpr float LabelSpacing = 12.0f;             // Spacing between icon and label

        // Collapsed state
        constexpr float CollapsedIconScale = 1.25f;       // Icon scale when sidebar collapsed

        // Accent bar (selected indicator)
        constexpr float AccentBarWidth = 3.0f;            // Width of left accent bar
        constexpr float AccentBarHeightRatio = 0.6f;      // Height as ratio of item height
        constexpr float AccentBarGlowRadius = 4.0f;       // Glow radius for accent bar

        // Colors and alpha
        constexpr int AccentGlowAlpha = 80;               // Alpha for accent glow color
        constexpr int BgAlphaMultiplier = 60;             // Multiplier for background alpha

        // Section header
        constexpr float SectionPaddingX = 12.0f;          // Section header horizontal padding
        constexpr float SectionPaddingY = 4.0f;           // Section header vertical padding
        constexpr float SectionHeight = 24.0f;            // Section header total height

        // Collapse button
        constexpr int HoverBgAlpha = 30;                  // Alpha for hover background
    }

    // =============================================
    // Toggle Constants
    // =============================================
    // Centralized constants for toggle/checkbox rendering.
    // Avoids magic numbers and enables easy tuning.

    namespace ToggleConstants
    {
        // Animation
        constexpr float AnimationSpeed = 12.0f;           // Toggle slide animation speed

        // Disabled state
        constexpr float DisabledAlpha = 0.4f;             // Alpha multiplier for disabled state

        // Glow effect
        constexpr float GlowThreshold = 0.1f;             // Minimum slideAnim to show glow
        constexpr float GlowRadius = 4.0f;                // Glow radius

        // Description offset
        constexpr float DescOffsetX = 60.0f;              // Description horizontal offset
        constexpr float DescSpacingY = 4.0f;              // Spacing after description
    }

    // =============================================
    // Layout Constants
    // =============================================
    // Centralized constants for layout helpers.
    // Avoids magic numbers and enables easy tuning.

    namespace LayoutConstants
    {
        // Separator dimensions
        constexpr float SeparatorOffsetY = 4.0f;         // Y offset for separator line
        constexpr float SeparatorSpacing = 8.0f;         // Total vertical spacing for separator
    }

    // =============================================
    // Tooltip Constants
    // =============================================
    // Centralized constants for tooltip rendering.
    // Avoids magic numbers and enables easy tuning.

    namespace TooltipConstants
    {
        // Padding
        constexpr float PaddingX = 8.0f;                 // Horizontal padding
        constexpr float PaddingY = 6.0f;                 // Vertical padding
    }

    // =============================================
    // Internal State (Window dragging)
    // =============================================

    inline bool& GetIsDraggingTitleBar()
    {
        static bool s_isDraggingTitleBar = false;
        return s_isDraggingTitleBar;
    }

    inline ImVec2& GetDragOffset()
    {
        static ImVec2 s_dragOffset = ImVec2(0, 0);
        return s_dragOffset;
    }

    // =============================================
    // Component Cleanup Functions
    // =============================================

    // Prune stale slider input buffers (called from BeginFrame)
    void PruneSliderInputBuffers(int maxIdleFrames = 60);
}
