#pragma once
// =============================================
// EFIGUI Components - Internal Shared Header
// This file contains shared includes, state, and helpers
// used across all component implementation files.
// =============================================
//
// NOTE: All component constants have been moved to ThemeConfig.h
// and are accessed via Theme::Xxx() accessors (e.g., Theme::Button(),
// Theme::Card(), Theme::Nav(), etc.). See Theme.h for full list.

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
