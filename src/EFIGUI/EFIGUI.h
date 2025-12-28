#pragma once

// =============================================
// EFIGUI - Escape From ImGui
// A modern, cyberpunk-themed UI framework for ImGui
// Supports glassmorphism effects with cross-platform backend abstraction
// =============================================

// Version
#define EFIGUI_VERSION_MAJOR 0
#define EFIGUI_VERSION_MINOR 3
#define EFIGUI_VERSION_PATCH 0
#define EFIGUI_VERSION_STRING "0.3.0"

// Core headers
#include "Theme.h"
#include "Animation.h"
#include "Layer.h"
#include "Draw.h"
#include "Components.h"

// Icons are configurable via EFIGUI::Icons namespace in Theme.h
// Users can override the default icons before calling EFIGUI functions:
//   EFIGUI::Icons::Close = "\xef\x80\x8d";  // Font Awesome xmark

// Backend headers (include separately as needed)
// #include "Backend/IBlurBackend.h"
// #include "Backend/BlurBackendDX11.h"

namespace EFIGUI
{
    // =============================================
    // Backend Types
    // =============================================

    enum class BackendType
    {
        None,       // No blur backend (fallback to solid colors)
        DX11,       // DirectX 11
        DX12,       // DirectX 12 (future)
        Vulkan,     // Vulkan (future)
        OpenGL      // OpenGL (future)
    };

    // =============================================
    // Initialization
    // =============================================

    // Initialize EFIGUI (call after ImGui initialization)
    // Returns true on success
    bool Initialize();

    // Initialize with blur backend
    // device: Graphics device pointer (ID3D11Device*, etc.)
    // width, height: Render target dimensions
    bool Initialize(BackendType backend, void* device, uint32_t width, uint32_t height);

    // Shutdown EFIGUI (call before ImGui shutdown)
    void Shutdown();

    // Check if EFIGUI is initialized
    bool IsInitialized();

    // =============================================
    // Per-Frame Operations
    // =============================================

    // Call at the beginning of each frame (required for Layer system)
    // Clears deferred drawing commands from previous frame
    void BeginFrame();

    // Call at the end of each frame (required for Layer system)
    // Flushes all deferred drawing commands in layer order
    // targetDrawList: Optional custom draw list (nullptr = use ForegroundDrawList)
    void EndFrame(ImDrawList* targetDrawList = nullptr);

    // =============================================
    // Layer System
    // =============================================

    // Get the Layer Manager for advanced configuration
    LayerManager& GetLayerManager();

    // =============================================
    // Blur Backend Operations
    // =============================================

    // Resize blur textures (call when window resizes)
    bool ResizeBlurBackend(uint32_t width, uint32_t height);

    // Apply blur effect to source texture
    // Returns blurred texture SRV for rendering
    void* ApplyBlur(void* sourceSRV, float blurRadius = 2.0f, int passes = 2);

    // Get the last blur result
    void* GetBlurResult();

    // Check if blur backend is available
    bool HasBlurBackend();

    // =============================================
    // Icon Helper
    // =============================================

    // Convert a Unicode codepoint to UTF-8 string
    // Usage: ImGui::Text("%s Home", EFIGUI::Icon(0xf015));
    // Alternative: set icons directly via EFIGUI::Icons namespace in Theme.h
    const char* Icon(uint32_t codepoint);

    // =============================================
    // Font Loading Helpers
    // =============================================

    // Load Font Awesome icons (call after adding your main font)
    // Returns true on success
    // fontData: Pointer to embedded font data (use GetFontAwesomeData())
    // fontDataSize: Size of font data in bytes
    // iconRangeStart: First icon codepoint (default: 0xf000)
    // iconRangeEnd: Last icon codepoint (default: 0xf8ff)
    bool LoadFontAwesomeIcons(const void* fontData, size_t fontDataSize,
                               float fontSize = 16.0f,
                               ImWchar iconRangeStart = 0xf000,
                               ImWchar iconRangeEnd = 0xf8ff);

    // Get embedded Font Awesome data (defined in FontAwesomeData.h)
    const void* GetFontAwesomeData();
    size_t GetFontAwesomeDataSize();
}
