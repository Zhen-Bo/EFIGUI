// src/EFIGUI/Core/Style.h
#pragma once
#include "../Themes/CyberpunkTheme.h"
#include <functional>
#include <type_traits>

namespace EFIGUI {

/// Style System - manages theme and style stack
class StyleSystem {
public:
    /// Initialize with default CyberpunkTheme
    static void Init();

    /// Initialize with custom theme
    template<typename Theme>
    static void Init();

    /// Execute function with temporary theme override
    template<typename Theme, typename Func>
    static void WithTheme(Func&& fn);

    /// Execute function with temporary style override
    template<typename StyleT, typename Func>
    static void WithStyle(const StyleT& style, Func&& fn);

    /// Get current style for a component type
    template<typename StyleT>
    static const StyleT& GetCurrentStyle();

private:
    static bool s_initialized;
};

// Free function wrappers for cleaner API
inline void Init() { StyleSystem::Init(); }

template<typename Theme>
inline void Init() { StyleSystem::Init<Theme>(); }

template<typename Theme, typename Func>
inline void WithTheme(Func&& fn) { StyleSystem::WithTheme<Theme>(std::forward<Func>(fn)); }

template<typename StyleT, typename Func>
inline void WithStyle(const StyleT& style, Func&& fn) {
    StyleSystem::WithStyle<StyleT>(style, std::forward<Func>(fn));
}

} // namespace EFIGUI
