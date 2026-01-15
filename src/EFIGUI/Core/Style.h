// src/EFIGUI/Core/Style.h
#pragma once
#include "../Themes/CyberpunkTheme.h"
#include "../Styles/CheckboxStyle.h"
#include "../Styles/RadioStyle.h"
#include "../Styles/SelectableStyle.h"
#include "../Styles/PopupStyle.h"
#include "../Styles/ColorEditStyle.h"
#include "../Styles/DragStyle.h"
#include "../Styles/VSliderStyle.h"
#include "../Styles/PlotStyle.h"
#include "../Styles/VectorInputStyle.h"
#include "imgui.h"
#include <any>
#include <atomic>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace EFIGUI {

/// Style System - manages theme and style stack per ImGuiContext
class StyleSystem {
public:
    /// Initialize with default CyberpunkTheme
    static void Init();

    /// Initialize with custom theme type
    /// NOTE: Currently this only marks the system as initialized.
    /// Custom theme support requires each style struct to have its
    /// own default values. For full custom themes, define your own
    /// style structs with different default values.
    template<typename Theme>
    static void Init() {
        s_initialized = true;
    }

    /// Execute function with temporary style override
    template<typename StyleT, typename Func>
    static void WithStyle(const StyleT& style, Func&& fn) {
        PushStyle<StyleT>(style);
        fn();
        PopStyle<StyleT>();
    }

    /// Push a style onto the stack for a component type
    template<typename StyleT>
    static void PushStyle(const StyleT& style) {
        auto& storage = GetContextStorage();
        auto key = std::type_index(typeid(StyleT));
        storage.styleStacks[key].push_back(style);
    }

    /// Pop a style from the stack for a component type
    template<typename StyleT>
    static void PopStyle() {
        auto& storage = GetContextStorage();
        auto key = std::type_index(typeid(StyleT));
        auto it = storage.styleStacks.find(key);
        if (it != storage.styleStacks.end() && !it->second.empty()) {
            it->second.pop_back();
        }
    }

    /// Get current style for a component type (top of stack or default)
    template<typename StyleT>
    static StyleT GetCurrentStyle() {
        auto& storage = GetContextStorage();
        auto key = std::type_index(typeid(StyleT));
        auto it = storage.styleStacks.find(key);
        if (it != storage.styleStacks.end() && !it->second.empty()) {
            return std::any_cast<StyleT>(it->second.back());
        }
        return StyleT{}; // Return default-constructed style
    }

    /// Check if initialized
    static bool IsInitialized() { return s_initialized; }

    /// Clean up storage for a destroyed ImGuiContext.
    /// Call this when an ImGuiContext is about to be destroyed to prevent memory leak.
    /// @param ctx The context to clean up, or nullptr for current context.
    static void CleanupContext(ImGuiContext* ctx = nullptr);

    /// Get per-context component state storage (for internal use by components)
    /// @tparam StateT The state type (e.g., std::unordered_map<ImGuiID, DragValueState>)
    /// @return Reference to the state, default-constructed if not exists
    template<typename StateT>
    static StateT& GetComponentState() {
        auto& storage = GetContextStorage();
        auto key = std::type_index(typeid(StateT));
        auto it = storage.componentStates.find(key);
        if (it == storage.componentStates.end()) {
            it = storage.componentStates.emplace(key, StateT{}).first;
        }
        return std::any_cast<StateT&>(it->second);
    }

private:
    /// Per-context storage for style stacks and component states
    struct ContextStorage {
        std::unordered_map<std::type_index, std::vector<std::any>> styleStacks;
        std::unordered_map<std::type_index, std::any> componentStates;  // For per-context component state
    };

    /// Get storage for current ImGuiContext (with thread_local caching + epoch invalidation)
    static ContextStorage& GetContextStorage() {
        ImGuiContext* ctx = ImGui::GetCurrentContext();
        IM_ASSERT(ctx != nullptr && "EFIGUI: No active ImGui context. Call ImGui::CreateContext() first.");

        // Thread-local cache to avoid mutex on hot path
        static thread_local ImGuiContext* t_lastCtx = nullptr;
        static thread_local ContextStorage* t_lastStorage = nullptr;
        static thread_local uint64_t t_lastEpoch = 0;

        // Read global epoch with relaxed ordering.
        // Note: relaxed is intentional - stale reads may cause one extra mutex lock
        // but never correctness issues. Using acquire would add unnecessary overhead
        // on every widget call.
        uint64_t currentEpoch = s_contextEpoch.load(std::memory_order_relaxed);

        // Fast path: cache hit with valid epoch
        if (t_lastCtx == ctx && t_lastStorage && t_lastEpoch == currentEpoch) {
            return *t_lastStorage;
        }

        // Slow path: look up or create storage
        {
            std::shared_lock lock(s_mutex);
            auto it = s_contextStorageMap.find(ctx);
            if (it != s_contextStorageMap.end()) {
                t_lastCtx = ctx;
                t_lastStorage = &it->second;
                t_lastEpoch = currentEpoch;
                return it->second;
            }
        }

        // Create new storage (write lock)
        std::unique_lock lock(s_mutex);
        // Double-check after acquiring write lock (another thread may have inserted)
        auto it = s_contextStorageMap.find(ctx);
        if (it != s_contextStorageMap.end()) {
            t_lastCtx = ctx;
            t_lastStorage = &it->second;
            t_lastEpoch = currentEpoch;
            return it->second;
        }
        auto& storage = s_contextStorageMap[ctx];
        t_lastCtx = ctx;
        t_lastStorage = &storage;
        t_lastEpoch = currentEpoch;
        return storage;
    }

    static std::atomic<bool> s_initialized;
    static std::atomic<uint64_t> s_contextEpoch;  // Epoch counter for cache invalidation
    static std::shared_mutex s_mutex;
    static std::unordered_map<ImGuiContext*, ContextStorage> s_contextStorageMap;
};

// =============================================
// Free function wrappers for cleaner API
// =============================================

inline void InitStyle() { StyleSystem::Init(); }

template<typename Theme>
inline void InitStyle() { StyleSystem::Init<Theme>(); }

template<typename StyleT, typename Func>
inline void WithStyle(const StyleT& style, Func&& fn) {
    StyleSystem::WithStyle<StyleT>(style, std::forward<Func>(fn));
}

template<typename StyleT>
inline void PushStyle(const StyleT& style) {
    StyleSystem::PushStyle<StyleT>(style);
}

template<typename StyleT>
inline void PopStyle() {
    StyleSystem::PopStyle<StyleT>();
}

} // namespace EFIGUI
