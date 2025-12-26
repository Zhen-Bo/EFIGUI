#include "EFIGUI.h"
#include "Layer.h"
#include "Backend/IBlurBackend.h"
#include "Components/Internal.h"

#ifdef _WIN32
#include "Backend/BlurBackendDX11.h"
#endif

#include <memory>

namespace EFIGUI
{
    // =============================================
    // Global State
    // =============================================

    static bool s_initialized = false;
    static std::unique_ptr<IBlurBackend> s_blurBackend = nullptr;

    // Frame lifecycle tracking for debug validation
    static int s_lastBeginFrameCount = -1;
    static int s_lastEndFrameCount = -1;

    // =============================================
    // Initialization
    // =============================================

    bool Initialize()
    {
        if (s_initialized)
            return true;

        // Apply theme
        Theme::Apply();

        s_initialized = true;
        return true;
    }

    bool Initialize(BackendType backend, void* device, uint32_t width, uint32_t height)
    {
        if (s_initialized)
            return true;

        // Apply theme
        Theme::Apply();

        // Create blur backend based on type
        switch (backend)
        {
#ifdef _WIN32
        case BackendType::DX11:
            s_blurBackend = std::make_unique<BlurBackendDX11>();
            if (!s_blurBackend->Initialize(device, width, height))
            {
                s_blurBackend.reset();
                // Continue without blur - not a fatal error
            }
            break;
#endif

        case BackendType::DX12:
        case BackendType::Vulkan:
        case BackendType::OpenGL:
            // Future backends - not implemented yet
            break;

        case BackendType::None:
        default:
            // No blur backend
            break;
        }

        s_initialized = true;
        return true;
    }

    void Shutdown()
    {
        if (s_blurBackend)
        {
            s_blurBackend->Shutdown();
            s_blurBackend.reset();
        }

        s_initialized = false;
    }

    bool IsInitialized()
    {
        return s_initialized;
    }

    // =============================================
    // Per-Frame Operations
    // =============================================

    void BeginFrame()
    {
        int currentFrame = ImGui::GetFrameCount();

        // Debug validation: warn if EndFrame was not called last frame
        if (s_lastBeginFrameCount >= 0 && s_lastEndFrameCount < s_lastBeginFrameCount)
        {
            IM_ASSERT(false && "EFIGUI::EndFrame() was not called! Layer commands from previous frame were lost.");
        }

        s_lastBeginFrameCount = currentFrame;

        // Clear deferred drawing commands from previous frame
        LayerManager::Get().BeginFrame();

        // Periodically prune stale cached state (every ~1 second at 60fps)
        static int s_pruneCounter = 0;
        if (++s_pruneCounter >= 60)
        {
            s_pruneCounter = 0;
            Animation::PruneStaleStates(60);
            PruneSliderInputBuffers(60);
        }
    }

    void EndFrame(ImDrawList* targetDrawList)
    {
        int currentFrame = ImGui::GetFrameCount();

        // Debug validation: warn if BeginFrame was not called this frame
        if (s_lastBeginFrameCount != currentFrame)
        {
            IM_ASSERT(false && "EFIGUI::BeginFrame() was not called this frame!");
        }

        s_lastEndFrameCount = currentFrame;

        // Flush all deferred drawing commands in layer order
        LayerManager::Get().Flush(targetDrawList);
    }

    // =============================================
    // Layer System
    // =============================================

    LayerManager& GetLayerManager()
    {
        return LayerManager::Get();
    }

    // =============================================
    // Blur Backend Operations
    // =============================================

    bool ResizeBlurBackend(uint32_t width, uint32_t height)
    {
        if (!s_blurBackend)
            return false;

        return s_blurBackend->Resize(width, height);
    }

    void* ApplyBlur(void* sourceSRV, float blurRadius, int passes)
    {
        if (!s_blurBackend)
            return nullptr;

        return s_blurBackend->ApplyBlur(sourceSRV, blurRadius, passes);
    }

    void* GetBlurResult()
    {
        if (!s_blurBackend)
            return nullptr;

        return s_blurBackend->GetBlurResult();
    }

    bool HasBlurBackend()
    {
        return s_blurBackend != nullptr && s_blurBackend->IsInitialized();
    }

    // =============================================
    // Icon Helper
    // =============================================

    const char* Icon(uint32_t codepoint)
    {
        // Thread-local buffer to store the UTF-8 encoded icon
        // Using multiple buffers to allow multiple Icon() calls in one expression
        static thread_local char buffers[8][5];  // 8 buffers, each can hold up to 4 UTF-8 bytes + null
        static thread_local int bufferIndex = 0;

        char* buf = buffers[bufferIndex];
        bufferIndex = (bufferIndex + 1) % 8;

        // Encode Unicode codepoint to UTF-8
        if (codepoint < 0x80)
        {
            buf[0] = static_cast<char>(codepoint);
            buf[1] = '\0';
        }
        else if (codepoint < 0x800)
        {
            buf[0] = static_cast<char>(0xC0 | (codepoint >> 6));
            buf[1] = static_cast<char>(0x80 | (codepoint & 0x3F));
            buf[2] = '\0';
        }
        else if (codepoint < 0x10000)
        {
            buf[0] = static_cast<char>(0xE0 | (codepoint >> 12));
            buf[1] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
            buf[2] = static_cast<char>(0x80 | (codepoint & 0x3F));
            buf[3] = '\0';
        }
        else
        {
            buf[0] = static_cast<char>(0xF0 | (codepoint >> 18));
            buf[1] = static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
            buf[2] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
            buf[3] = static_cast<char>(0x80 | (codepoint & 0x3F));
            buf[4] = '\0';
        }

        return buf;
    }

    // =============================================
    // Font Loading Helpers
    // =============================================

    bool LoadFontAwesomeIcons(const void* fontData, size_t fontDataSize,
                               float fontSize,
                               ImWchar iconRangeStart,
                               ImWchar iconRangeEnd)
    {
        if (!fontData || fontDataSize == 0)
            return false;

        ImGuiIO& io = ImGui::GetIO();

        // Define icon glyph range
        static ImWchar iconRanges[3] = { 0, 0, 0 };
        iconRanges[0] = iconRangeStart;
        iconRanges[1] = iconRangeEnd;

        // Configure font
        ImFontConfig config;
        config.MergeMode = true;  // Merge with previous font
        config.PixelSnapH = true;
        config.FontDataOwnedByAtlas = false;  // We own the data

        // Add font
        io.Fonts->AddFontFromMemoryTTF(
            const_cast<void*>(fontData),
            static_cast<int>(fontDataSize),
            fontSize,
            &config,
            iconRanges
        );

        return true;
    }

    // Font data placeholder - user should provide their own FontAwesomeData.h
    // or these will return nullptr and the user can load fonts manually
    const void* GetFontAwesomeData()
    {
        // Override this by providing FontAwesomeData.h with the actual embedded data
        // Example:
        // #include "FontAwesomeData.h"
        // return fa_solid_900_data;
        return nullptr;
    }

    size_t GetFontAwesomeDataSize()
    {
        // Override this by providing FontAwesomeData.h with the actual embedded data
        // Example:
        // return fa_solid_900_data_size;
        return 0;
    }
}
