#pragma once
#include "imgui.h"
#include <unordered_map>

namespace EFIGUI
{
    namespace Animation
    {
        // =============================================
        // Widget State Storage
        // =============================================

        struct WidgetState
        {
            float hoverAnim = 0.0f;      // 0 = not hovered, 1 = fully hovered
            float activeAnim = 0.0f;     // 0 = not active, 1 = fully active
            float selectedAnim = 0.0f;   // 0 = not selected, 1 = fully selected
            float glowPhase = 0.0f;      // For pulsing glow effect
            float clickAnim = 0.0f;      // For click ripple effect
            float slideAnim = 0.0f;      // For toggle slide
        };

        // Get or create widget state for a given ID
        WidgetState& GetState(ImGuiID id);

        // Clear all cached states (call on frame start if needed)
        void ClearStates();

        // =============================================
        // Easing Functions
        // =============================================

        // Linear interpolation with delta-time smoothing
        float Lerp(float current, float target, float speed);

        // Color lerp
        ImVec4 LerpColor(const ImVec4& current, const ImVec4& target, float speed);
        ImU32 LerpColorU32(ImU32 current, ImU32 target, float t);

        // Easing curves
        float EaseOutQuad(float t);       // Decelerating
        float EaseInQuad(float t);        // Accelerating
        float EaseInOutQuad(float t);     // Smooth in-out
        float EaseOutBack(float t);       // Overshoot
        float EaseOutElastic(float t);    // Bouncy

        // =============================================
        // Periodic Functions
        // =============================================

        // Pulse effect (0 to 1 oscillation)
        float Pulse(float frequency = 1.0f);

        // Breathing glow (smoother sine wave)
        float Breathe(float frequency = 1.0f);

        // Sweeping/flowing light effect (0 to 1 position along perimeter)
        // Returns a value that moves from 0 to 1 continuously over the cycle
        float Sweep(float frequency = 0.8f);

        // Shimmer effect for multiple flowing lines
        float Shimmer(float frequency = 1.0f, float offset = 0.0f);

        // =============================================
        // Utility
        // =============================================

        // Get current time for animations
        float GetTime();

        // Get delta time
        float GetDeltaTime();

        // Update widget state animations
        void UpdateWidgetState(WidgetState& state, bool isHovered, bool isActive, bool isSelected, float speed = 8.0f);
    }
}
