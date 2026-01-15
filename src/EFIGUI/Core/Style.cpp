// src/EFIGUI/Core/Style.cpp
#include "Style.h"

namespace EFIGUI {

std::atomic<bool> StyleSystem::s_initialized{false};
std::atomic<uint64_t> StyleSystem::s_contextEpoch{1};  // Start at 1 so 0 is always invalid
std::shared_mutex StyleSystem::s_mutex;
std::unordered_map<ImGuiContext*, StyleSystem::ContextStorage> StyleSystem::s_contextStorageMap;

void StyleSystem::Init() {
    Init<CyberpunkTheme>();
}

void StyleSystem::CleanupContext(ImGuiContext* ctx) {
    std::unique_lock lock(s_mutex);
    ImGuiContext* target = ctx ? ctx : ImGui::GetCurrentContext();
    if (target)
        s_contextStorageMap.erase(target);

    // Advance epoch to invalidate all thread-local caches
    // This prevents dangling pointer issues when a context is destroyed
    s_contextEpoch.fetch_add(1, std::memory_order_release);
}

} // namespace EFIGUI
