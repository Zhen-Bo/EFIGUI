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
            int lastUpdateFrame = 0;     // Frame number when last updated (for pruning)
        };

        // Get or create widget state for a given ID
        WidgetState& GetState(ImGuiID id);

        // Clear all cached states (call on frame start if needed)
        void ClearStates();

        // Prune stale widget states that haven't been updated recently
        // maxIdleFrames: Number of frames after which unused states are pruned
        void PruneStaleStates(int maxIdleFrames = 60);

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
        // Smooth Transition Helpers
        // =============================================
        // For dynamic real-time UI scaling with smooth animations.
        // Usage: SmoothFloat height; height.SetTarget(48.0f); ... height.Update();

        // Smooth floating point value with automatic interpolation
        struct SmoothFloat
        {
            float current = 0.0f;
            float target = 0.0f;

            // Update and return the smoothed value
            float Update(float speed = 8.0f)
            {
                current = Lerp(current, target, speed);
                return current;
            }

            // Set new target value (will animate towards it)
            void SetTarget(float newTarget) { target = newTarget; }

            // Set immediately without animation
            void SetImmediate(float value) { current = target = value; }

            // Check if animation is complete (within threshold)
            bool IsComplete(float threshold = 0.001f) const
            {
                return (current > target - threshold) && (current < target + threshold);
            }

            // Implicit conversion to float for convenience
            operator float() const { return current; }
        };

        // Smooth 2D vector with automatic interpolation
        struct SmoothVec2
        {
            ImVec2 current = ImVec2(0.0f, 0.0f);
            ImVec2 target = ImVec2(0.0f, 0.0f);

            // Update and return the smoothed value
            ImVec2 Update(float speed = 8.0f)
            {
                current.x = Lerp(current.x, target.x, speed);
                current.y = Lerp(current.y, target.y, speed);
                return current;
            }

            // Set new target value (will animate towards it)
            void SetTarget(const ImVec2& newTarget) { target = newTarget; }
            void SetTarget(float x, float y) { target = ImVec2(x, y); }

            // Set immediately without animation
            void SetImmediate(const ImVec2& value) { current = target = value; }
            void SetImmediate(float x, float y) { current = target = ImVec2(x, y); }

            // Check if animation is complete (within threshold)
            bool IsComplete(float threshold = 0.001f) const
            {
                float dx = current.x - target.x;
                float dy = current.y - target.y;
                return (dx > -threshold) && (dx < threshold) &&
                       (dy > -threshold) && (dy < threshold);
            }

            // Implicit conversion to ImVec2 for convenience
            operator ImVec2() const { return current; }
        };

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
