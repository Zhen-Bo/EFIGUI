// src/EFIGUI/Core/Style.cpp
#include "Style.h"

namespace EFIGUI {

std::atomic<bool> StyleSystem::s_initialized{false};
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
}

} // namespace EFIGUI
