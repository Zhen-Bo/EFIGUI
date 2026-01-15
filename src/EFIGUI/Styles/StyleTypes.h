// src/EFIGUI/Styles/StyleTypes.h
#pragma once
#include "imgui.h"
#include <cstdint>

namespace EFIGUI {

/// Edge insets for padding/margin (top, right, bottom, left)
struct EdgeInsets {
    float top = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;
    float left = 0.0f;

    constexpr EdgeInsets() = default;
    constexpr EdgeInsets(float t, float r, float b, float l)
        : top(t), right(r), bottom(b), left(l) {}

    static constexpr EdgeInsets All(float v) { return {v, v, v, v}; }
    static constexpr EdgeInsets Symmetric(float vertical, float horizontal) {
        return {vertical, horizontal, vertical, horizontal};
    }
    static constexpr EdgeInsets Horizontal(float h) { return {0, h, 0, h}; }
    static constexpr EdgeInsets Vertical(float v) { return {v, 0, v, 0}; }
};

} // namespace EFIGUI
