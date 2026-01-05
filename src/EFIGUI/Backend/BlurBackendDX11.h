#pragma once
#include "IBlurBackend.h"

#ifdef _WIN32
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace EFIGUI
{
    using Microsoft::WRL::ComPtr;

    // =============================================
    // DX11 Blur Backend Implementation
    // =============================================

    class BlurBackendDX11 : public IBlurBackend
    {
    public:
        BlurBackendDX11() = default;
        ~BlurBackendDX11() override { Shutdown(); }

        // IBlurBackend interface
        bool Initialize(void* device, uint32_t width, uint32_t height) override;
        void Shutdown() override;
        bool Resize(uint32_t width, uint32_t height) override;
        void* ApplyBlur(void* sourceTexture, float blurRadius, int passes) override;
        void* GetBlurResult() override;
        bool IsInitialized() const override { return m_initialized; }
        uint32_t GetWidth() const override { return m_textureWidth; }
        uint32_t GetHeight() const override { return m_textureHeight; }

    private:
        // =============================================
        // D3D11 State Backup (for proper state restoration)
        // =============================================

        struct D3D11StateBackup
        {
            // Input Assembler
            ComPtr<ID3D11InputLayout> inputLayout;
            D3D11_PRIMITIVE_TOPOLOGY primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
            ComPtr<ID3D11Buffer> vertexBuffer;
            UINT vertexStride = 0;
            UINT vertexOffset = 0;
            ComPtr<ID3D11Buffer> indexBuffer;
            DXGI_FORMAT indexFormat = DXGI_FORMAT_UNKNOWN;
            UINT indexOffset = 0;

            // Shaders
            ComPtr<ID3D11VertexShader> vertexShader;
            ComPtr<ID3D11PixelShader> pixelShader;

            // Shader Resources
            ComPtr<ID3D11ShaderResourceView> psShaderResource;
            ComPtr<ID3D11SamplerState> psSampler;
            ComPtr<ID3D11Buffer> psConstantBuffer;

            // Output Merger
            ComPtr<ID3D11RenderTargetView> renderTargetView;
            ComPtr<ID3D11DepthStencilView> depthStencilView;
            ComPtr<ID3D11BlendState> blendState;
            FLOAT blendFactor[4] = { 0, 0, 0, 0 };
            UINT sampleMask = 0xFFFFFFFF;
            ComPtr<ID3D11DepthStencilState> depthStencilState;
            UINT stencilRef = 0;

            // Rasterizer
            ComPtr<ID3D11RasterizerState> rasterizerState;
            D3D11_VIEWPORT viewport = {};
            UINT numViewports = 0;

            void Save(ID3D11DeviceContext* context);
            void Restore(ID3D11DeviceContext* context);
        };

        // =============================================
        // Blur Parameters (constant buffer)
        // =============================================

        struct BlurParams
        {
            float texelSizeX;
            float texelSizeY;
            float blurDirX;
            float blurDirY;
            float blurRadius;
            float padding[3];
        };

        // =============================================
        // Internal Methods
        // =============================================

        bool CompileShaders();
        bool CreateRenderTargets(uint32_t width, uint32_t height);
        bool CreateQuadBuffer();
        void RenderBlurPass(bool horizontal, float radius);

        // =============================================
        // Resources
        // =============================================

        // Device references
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;

        // Shaders
        ComPtr<ID3D11VertexShader> m_vertexShader;
        ComPtr<ID3D11PixelShader> m_blurPixelShader;
        ComPtr<ID3D11PixelShader> m_copyPixelShader;
        ComPtr<ID3D11InputLayout> m_inputLayout;

        // Constant buffer
        ComPtr<ID3D11Buffer> m_constantBuffer;

        // Render targets for ping-pong blur
        ComPtr<ID3D11Texture2D> m_blurTexture[2];
        ComPtr<ID3D11RenderTargetView> m_blurRTV[2];
        ComPtr<ID3D11ShaderResourceView> m_blurSRV[2];

        // Fullscreen quad
        ComPtr<ID3D11Buffer> m_quadVertexBuffer;

        // Sampler
        ComPtr<ID3D11SamplerState> m_linearSampler;

        // State
        bool m_initialized = false;
        uint32_t m_textureWidth = 0;
        uint32_t m_textureHeight = 0;
        int m_lastResultIndex = 0;
    };
}

#endif // _WIN32
