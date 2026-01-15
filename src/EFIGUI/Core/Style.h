// src/EFIGUI/Core/Style.h
#pragma once
#include "../Themes/CyberpunkTheme.h"
#include "../Styles/CheckboxStyle.h"
#include "../Styles/RadioStyle.h"
#include "../Styles/SelectableStyle.h"
#include "../Styles/PopupStyle.h"
#include "imgui.h"
#include <functional>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <any>

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

private:
    /// Per-context storage for style stacks
    struct ContextStorage {
        std::unordered_map<std::type_index, std::vector<std::any>> styleStacks;
    };

    /// Get storage for current ImGuiContext
    static ContextStorage& GetContextStorage() {
        // Use global map keyed by ImGuiContext pointer
        // This supports single-thread multi-context scenarios
        static std::unordered_map<ImGuiContext*, ContextStorage> s_contextStorageMap;
        ImGuiContext* ctx = ImGui::GetCurrentContext();
        return s_contextStorageMap[ctx];
    }

    static bool s_initialized;
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
