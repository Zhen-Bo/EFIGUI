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
}
