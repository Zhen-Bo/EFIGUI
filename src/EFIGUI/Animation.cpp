#include "Animation.h"
#include <cmath>

namespace EFIGUI
{
    namespace Animation
    {
        // Static storage for widget states
        static std::unordered_map<ImGuiID, WidgetState> s_widgetStates;

        WidgetState& GetState(ImGuiID id)
        {
            return s_widgetStates[id];
        }

        void ClearStates()
        {
            s_widgetStates.clear();
        }

        void PruneStaleStates(int maxIdleFrames)
        {
            int currentFrame = ImGui::GetFrameCount();
            for (auto it = s_widgetStates.begin(); it != s_widgetStates.end(); )
            {
                if (currentFrame - it->second.lastUpdateFrame > maxIdleFrames)
                {
                    it = s_widgetStates.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        // =============================================
        // Interpolation
        // =============================================

        float Lerp(float current, float target, float speed)
        {
            float dt = GetDeltaTime();
            float t = 1.0f - std::pow(0.5f, dt * speed);
            return current + (target - current) * t;
        }

        ImVec4 LerpColor(const ImVec4& current, const ImVec4& target, float speed)
        {
            return ImVec4(
                Lerp(current.x, target.x, speed),
                Lerp(current.y, target.y, speed),
                Lerp(current.z, target.z, speed),
                Lerp(current.w, target.w, speed)
            );
        }

        ImU32 LerpColorU32(ImU32 current, ImU32 target, float t)
        {
            int r1 = (current >> 0) & 0xFF;
            int g1 = (current >> 8) & 0xFF;
            int b1 = (current >> 16) & 0xFF;
            int a1 = (current >> 24) & 0xFF;

            int r2 = (target >> 0) & 0xFF;
            int g2 = (target >> 8) & 0xFF;
            int b2 = (target >> 16) & 0xFF;
            int a2 = (target >> 24) & 0xFF;

            int r = r1 + (int)((r2 - r1) * t);
            int g = g1 + (int)((g2 - g1) * t);
            int b = b1 + (int)((b2 - b1) * t);
            int a = a1 + (int)((a2 - a1) * t);

            return IM_COL32(r, g, b, a);
        }

        // =============================================
        // Easing Functions
        // =============================================

        float EaseOutQuad(float t)
        {
            return 1.0f - (1.0f - t) * (1.0f - t);
        }

        float EaseInQuad(float t)
        {
            return t * t;
        }

        float EaseInOutQuad(float t)
        {
            return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
        }

        float EaseOutBack(float t)
        {
            const float c1 = 1.70158f;
            const float c3 = c1 + 1.0f;
            return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
        }

        float EaseOutElastic(float t)
        {
            const float c4 = (2.0f * 3.14159f) / 3.0f;
            if (t == 0.0f) return 0.0f;
            if (t == 1.0f) return 1.0f;
            return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
        }

        // =============================================
        // Periodic Functions
        // =============================================

        float Pulse(float frequency)
        {
            float t = GetTime() * frequency;
            return (std::sin(t * 2.0f * 3.14159f) + 1.0f) * 0.5f;
        }

        float Breathe(float frequency)
        {
            float t = GetTime() * frequency;
            // Smoother breathing curve
            float val = (std::sin(t * 2.0f * 3.14159f - 1.5708f) + 1.0f) * 0.5f;
            return val * val;  // Square for more natural breathing
        }

        float Sweep(float frequency)
        {
            float t = GetTime() * frequency;
            // Simple sawtooth wave from 0 to 1, continuous and smooth
            float phase = std::fmod(t, 1.0f);
            if (phase < 0.0f) phase += 1.0f;  // Handle negative time
            return phase;
        }

        float Shimmer(float frequency, float offset)
        {
            float t = GetTime() * frequency + offset;
            float phase = std::fmod(t, 1.0f);
            return phase;
        }

        // =============================================
        // Utility
        // =============================================

        float GetTime()
        {
            return (float)ImGui::GetTime();
        }

        float GetDeltaTime()
        {
            return ImGui::GetIO().DeltaTime;
        }

        void UpdateWidgetState(WidgetState& state, bool isHovered, bool isActive, bool isSelected, float speed)
        {
            float dt = GetDeltaTime();
            float t = 1.0f - std::pow(0.5f, dt * speed);

            // Track when this state was last updated
            state.lastUpdateFrame = ImGui::GetFrameCount();

            // Update hover
            float targetHover = isHovered ? 1.0f : 0.0f;
            state.hoverAnim += (targetHover - state.hoverAnim) * t;

            // Update active
            float targetActive = isActive ? 1.0f : 0.0f;
            state.activeAnim += (targetActive - state.activeAnim) * t;

            // Update selected
            float targetSelected = isSelected ? 1.0f : 0.0f;
            state.selectedAnim += (targetSelected - state.selectedAnim) * t;

            // Update glow phase (continuous)
            state.glowPhase = Breathe(2.0f);

            // Decay click animation
            if (state.clickAnim > 0.0f)
            {
                state.clickAnim -= dt * 3.0f;
                if (state.clickAnim < 0.0f) state.clickAnim = 0.0f;
            }
        }
    }
}
