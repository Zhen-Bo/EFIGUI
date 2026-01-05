#pragma once
#include <cstdint>

namespace EFIGUI
{
    // =============================================
    // Abstract Blur Backend Interface
    // =============================================
    //
    // This interface allows EFIGUI to support multiple graphics APIs
    // for the glassmorphism blur effect. Implement this interface
    // for your target platform (DX11, DX12, Vulkan, OpenGL, etc.)
    //

    class IBlurBackend
    {
    public:
        virtual ~IBlurBackend() = default;

        // Initialize the blur backend with platform-specific device
        // device: Platform-specific device (ID3D11Device*, VkDevice, etc.)
        // width, height: Initial render target dimensions
        // Returns true on success
        virtual bool Initialize(void* device, uint32_t width, uint32_t height) = 0;

        // Cleanup and release all resources
        virtual void Shutdown() = 0;

        // Resize internal render targets when window size changes
        virtual bool Resize(uint32_t width, uint32_t height) = 0;

        // Apply blur effect to the source texture
        // sourceTexture: Platform-specific source texture (SRV, image view, etc.)
        // blurRadius: Blur strength (typically 1.0 - 5.0)
        // passes: Number of blur passes (more = smoother but slower)
        // Returns: Platform-specific blurred result texture (SRV, image view, etc.)
        virtual void* ApplyBlur(void* sourceTexture, float blurRadius, int passes) = 0;

        // Get the last blur result (for reuse without re-applying)
        virtual void* GetBlurResult() = 0;

        // Check if backend is initialized and ready
        virtual bool IsInitialized() const = 0;

        // Get the current render target dimensions
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
    };
}
