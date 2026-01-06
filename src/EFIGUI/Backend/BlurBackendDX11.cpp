#include "BlurBackendDX11.h"

#ifdef _WIN32

#include <cstring>
#include <utility>

namespace EFIGUI
{
    // =============================================
    // Embedded Shader Code (compiled at runtime)
    // NOTE: Uses runtime D3DCompile which requires d3dcompiler_47.dll.
    //       If users report injection crashes, consider switching to
    //       precompiled bytecode embedded directly.
    // =============================================

    static const char* g_shaderCode = R"(
// Constant buffer for blur parameters
cbuffer BlurParams : register(b0)
{
    float2 texelSize;
    float2 blurDirection;
    float blurRadius;
    float3 padding;
};

Texture2D sourceTexture : register(t0);
SamplerState linearSampler : register(s0);

struct VS_INPUT
{
    float2 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

PS_INPUT VS_Main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = float4(input.pos * 2.0 - 1.0, 0.0, 1.0);
    output.pos.y = -output.pos.y;
    output.uv = input.uv;
    return output;
}

// 9-tap Gaussian weights
static const float weights[9] = {
    0.0162162162, 0.0540540541, 0.1216216216, 0.1945945946,
    0.2270270270,
    0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162
};

static const float offsets[9] = {
    -4.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0
};

float4 PS_Blur(PS_INPUT input) : SV_TARGET
{
    float4 result = float4(0, 0, 0, 0);
    float2 direction = blurDirection * texelSize * blurRadius;

    [unroll]
    for (int i = 0; i < 9; i++)
    {
        float2 sampleUV = input.uv + direction * offsets[i];
        result += sourceTexture.Sample(linearSampler, sampleUV) * weights[i];
    }

    return result;
}

float4 PS_Copy(PS_INPUT input) : SV_TARGET
{
    return sourceTexture.Sample(linearSampler, input.uv);
}
)";

    // =============================================
    // D3D11 State Backup Implementation
    // =============================================

    void BlurBackendDX11::D3D11StateBackup::Save(ID3D11DeviceContext* context)
    {
        // Input Assembler
        context->IAGetInputLayout(inputLayout.GetAddressOf());
        context->IAGetPrimitiveTopology(&primitiveTopology);
        context->IAGetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &vertexStride, &vertexOffset);
        context->IAGetIndexBuffer(indexBuffer.GetAddressOf(), &indexFormat, &indexOffset);

        // Shaders
        context->VSGetShader(vertexShader.GetAddressOf(), nullptr, nullptr);
        context->PSGetShader(pixelShader.GetAddressOf(), nullptr, nullptr);

        // Shader Resources
        context->PSGetShaderResources(0, 1, psShaderResource.GetAddressOf());
        context->PSGetSamplers(0, 1, psSampler.GetAddressOf());
        context->PSGetConstantBuffers(0, 1, psConstantBuffer.GetAddressOf());

        // Output Merger
        context->OMGetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.GetAddressOf());
        context->OMGetBlendState(blendState.GetAddressOf(), blendFactor, &sampleMask);
        context->OMGetDepthStencilState(depthStencilState.GetAddressOf(), &stencilRef);

        // Rasterizer
        context->RSGetState(rasterizerState.GetAddressOf());
        numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);
    }

    void BlurBackendDX11::D3D11StateBackup::Restore(ID3D11DeviceContext* context)
    {
        // Input Assembler
        context->IASetInputLayout(inputLayout.Get());
        context->IASetPrimitiveTopology(primitiveTopology);
        context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &vertexStride, &vertexOffset);
        context->IASetIndexBuffer(indexBuffer.Get(), indexFormat, indexOffset);

        // Shaders
        context->VSSetShader(vertexShader.Get(), nullptr, 0);
        context->PSSetShader(pixelShader.Get(), nullptr, 0);

        // Shader Resources
        context->PSSetShaderResources(0, 1, psShaderResource.GetAddressOf());
        context->PSSetSamplers(0, 1, psSampler.GetAddressOf());
        context->PSSetConstantBuffers(0, 1, psConstantBuffer.GetAddressOf());

        // Output Merger
        context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
        context->OMSetBlendState(blendState.Get(), blendFactor, sampleMask);
        context->OMSetDepthStencilState(depthStencilState.Get(), stencilRef);

        // Rasterizer
        context->RSSetState(rasterizerState.Get());
        context->RSSetViewports(numViewports, &viewport);
    }

    // =============================================
    // IBlurBackend Implementation
    // =============================================

    bool BlurBackendDX11::Initialize(void* device, uint32_t width, uint32_t height)
    {
        if (!device)
            return false;

        if (m_initialized)
            return true;

        m_device = static_cast<ID3D11Device*>(device);
        m_device->GetImmediateContext(m_context.GetAddressOf());

        // Compile shaders
        if (!CompileShaders())
            return false;

        // Create render targets
        if (!CreateRenderTargets(width, height))
            return false;

        // Create quad buffer
        if (!CreateQuadBuffer())
            return false;

        // Create sampler state
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        HRESULT hr = m_device->CreateSamplerState(&samplerDesc, m_linearSampler.GetAddressOf());
        if (FAILED(hr))
            return false;

        // Create constant buffer
        D3D11_BUFFER_DESC cbDesc = {};
        cbDesc.ByteWidth = sizeof(BlurParams);
        cbDesc.Usage = D3D11_USAGE_DYNAMIC;
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        hr = m_device->CreateBuffer(&cbDesc, nullptr, m_constantBuffer.GetAddressOf());
        if (FAILED(hr))
            return false;

        m_initialized = true;
        return true;
    }

    void BlurBackendDX11::Shutdown()
    {
        m_vertexShader.Reset();
        m_blurPixelShader.Reset();
        m_copyPixelShader.Reset();
        m_inputLayout.Reset();
        m_constantBuffer.Reset();
        m_quadVertexBuffer.Reset();
        m_linearSampler.Reset();

        for (int i = 0; i < 2; i++)
        {
            m_blurTexture[i].Reset();
            m_blurRTV[i].Reset();
            m_blurSRV[i].Reset();
        }

        m_context.Reset();
        m_device.Reset();

        m_initialized = false;
        m_textureWidth = 0;
        m_textureHeight = 0;
    }

    bool BlurBackendDX11::Resize(uint32_t width, uint32_t height)
    {
        if (!m_initialized)
            return false;

        if (width == m_textureWidth && height == m_textureHeight)
            return true;

        // Release old render targets
        for (int i = 0; i < 2; i++)
        {
            m_blurTexture[i].Reset();
            m_blurRTV[i].Reset();
            m_blurSRV[i].Reset();
        }

        // Create new ones
        return CreateRenderTargets(width, height);
    }

    void* BlurBackendDX11::ApplyBlur(void* sourceTexture, float blurRadius, int passes)
    {
        if (!m_initialized || !m_context || !sourceTexture)
            return nullptr;

        ID3D11ShaderResourceView* sourceSRV = static_cast<ID3D11ShaderResourceView*>(sourceTexture);

        // Save complete D3D11 pipeline state
        D3D11StateBackup stateBackup;
        stateBackup.Save(m_context.Get());

        // Set viewport for blur textures
        D3D11_VIEWPORT blurViewport = {};
        blurViewport.Width = static_cast<float>(m_textureWidth);
        blurViewport.Height = static_cast<float>(m_textureHeight);
        blurViewport.MinDepth = 0.0f;
        blurViewport.MaxDepth = 1.0f;
        m_context->RSSetViewports(1, &blurViewport);

        // Set common state
        m_context->IASetInputLayout(m_inputLayout.Get());
        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        UINT stride = sizeof(float) * 4;
        UINT offset = 0;
        m_context->IASetVertexBuffers(0, 1, m_quadVertexBuffer.GetAddressOf(), &stride, &offset);

        m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
        m_context->PSSetShader(m_blurPixelShader.Get(), nullptr, 0);
        m_context->PSSetSamplers(0, 1, m_linearSampler.GetAddressOf());

        // First pass: copy source to blur texture 0
        m_context->OMSetRenderTargets(1, m_blurRTV[0].GetAddressOf(), nullptr);
        m_context->PSSetShader(m_copyPixelShader.Get(), nullptr, 0);
        m_context->PSSetShaderResources(0, 1, &sourceSRV);
        m_context->Draw(6, 0);

        // Blur passes (ping-pong between textures)
        m_context->PSSetShader(m_blurPixelShader.Get(), nullptr, 0);
        int srcIdx = 0;
        int dstIdx = 1;

        for (int p = 0; p < passes; p++)
        {
            // Horizontal blur
            RenderBlurPass(true, blurRadius);
            ID3D11ShaderResourceView* nullSRV = nullptr;
            m_context->PSSetShaderResources(0, 1, &nullSRV);
            m_context->OMSetRenderTargets(1, m_blurRTV[dstIdx].GetAddressOf(), nullptr);
            m_context->PSSetShaderResources(0, 1, m_blurSRV[srcIdx].GetAddressOf());
            m_context->Draw(6, 0);

            std::swap(srcIdx, dstIdx);

            // Vertical blur
            RenderBlurPass(false, blurRadius);
            m_context->PSSetShaderResources(0, 1, &nullSRV);
            m_context->OMSetRenderTargets(1, m_blurRTV[dstIdx].GetAddressOf(), nullptr);
            m_context->PSSetShaderResources(0, 1, m_blurSRV[srcIdx].GetAddressOf());
            m_context->Draw(6, 0);

            std::swap(srcIdx, dstIdx);
        }

        // Unbind shader resources to avoid hazards
        ID3D11ShaderResourceView* nullSRV = nullptr;
        m_context->PSSetShaderResources(0, 1, &nullSRV);

        // Restore complete D3D11 pipeline state
        stateBackup.Restore(m_context.Get());

        // Store result index for GetBlurResult
        m_lastResultIndex = srcIdx;

        // Return the final blurred result
        return m_blurSRV[srcIdx].Get();
    }

    void* BlurBackendDX11::GetBlurResult()
    {
        if (!m_initialized)
            return nullptr;
        return m_blurSRV[m_lastResultIndex].Get();
    }

    // =============================================
    // Internal Methods
    // =============================================

    bool BlurBackendDX11::CompileShaders()
    {
        HRESULT hr;
        ComPtr<ID3DBlob> vsBlob;
        ComPtr<ID3DBlob> psBlurBlob;
        ComPtr<ID3DBlob> psCopyBlob;
        ComPtr<ID3DBlob> errorBlob;

        UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
        compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

        // Compile vertex shader
        hr = D3DCompile(
            g_shaderCode, strlen(g_shaderCode),
            "BlurShader", nullptr, nullptr,
            "VS_Main", "vs_4_0",
            compileFlags, 0,
            vsBlob.GetAddressOf(), errorBlob.GetAddressOf());

        if (FAILED(hr))
            return false;

        // Compile blur pixel shader
        hr = D3DCompile(
            g_shaderCode, strlen(g_shaderCode),
            "BlurShader", nullptr, nullptr,
            "PS_Blur", "ps_4_0",
            compileFlags, 0,
            psBlurBlob.GetAddressOf(), errorBlob.GetAddressOf());

        if (FAILED(hr))
            return false;

        // Compile copy pixel shader
        hr = D3DCompile(
            g_shaderCode, strlen(g_shaderCode),
            "BlurShader", nullptr, nullptr,
            "PS_Copy", "ps_4_0",
            compileFlags, 0,
            psCopyBlob.GetAddressOf(), errorBlob.GetAddressOf());

        if (FAILED(hr))
            return false;

        // Create shaders
        hr = m_device->CreateVertexShader(
            vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
            nullptr, m_vertexShader.GetAddressOf());
        if (FAILED(hr)) return false;

        hr = m_device->CreatePixelShader(
            psBlurBlob->GetBufferPointer(), psBlurBlob->GetBufferSize(),
            nullptr, m_blurPixelShader.GetAddressOf());
        if (FAILED(hr)) return false;

        hr = m_device->CreatePixelShader(
            psCopyBlob->GetBufferPointer(), psCopyBlob->GetBufferSize(),
            nullptr, m_copyPixelShader.GetAddressOf());
        if (FAILED(hr)) return false;

        // Create input layout
        D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        hr = m_device->CreateInputLayout(
            layoutDesc, 2,
            vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
            m_inputLayout.GetAddressOf());
        if (FAILED(hr)) return false;

        return true;
    }

    bool BlurBackendDX11::CreateRenderTargets(uint32_t width, uint32_t height)
    {
        // Use quarter resolution for performance
        uint32_t blurWidth = width / 2;
        uint32_t blurHeight = height / 2;

        for (int i = 0; i < 2; i++)
        {
            D3D11_TEXTURE2D_DESC texDesc = {};
            texDesc.Width = blurWidth;
            texDesc.Height = blurHeight;
            texDesc.MipLevels = 1;
            texDesc.ArraySize = 1;
            texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            texDesc.SampleDesc.Count = 1;
            texDesc.Usage = D3D11_USAGE_DEFAULT;
            texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

            HRESULT hr = m_device->CreateTexture2D(&texDesc, nullptr, m_blurTexture[i].GetAddressOf());
            if (FAILED(hr))
                return false;

            hr = m_device->CreateRenderTargetView(m_blurTexture[i].Get(), nullptr, m_blurRTV[i].GetAddressOf());
            if (FAILED(hr))
                return false;

            hr = m_device->CreateShaderResourceView(m_blurTexture[i].Get(), nullptr, m_blurSRV[i].GetAddressOf());
            if (FAILED(hr))
                return false;
        }

        m_textureWidth = blurWidth;
        m_textureHeight = blurHeight;

        return true;
    }

    bool BlurBackendDX11::CreateQuadBuffer()
    {
        // Fullscreen quad (2 triangles)
        float vertices[] = {
            // pos.x, pos.y, uv.x, uv.y
            0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f
        };

        D3D11_BUFFER_DESC bufDesc = {};
        bufDesc.ByteWidth = sizeof(vertices);
        bufDesc.Usage = D3D11_USAGE_IMMUTABLE;
        bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = vertices;

        HRESULT hr = m_device->CreateBuffer(&bufDesc, &initData, m_quadVertexBuffer.GetAddressOf());
        if (FAILED(hr))
            return false;

        return true;
    }

    void BlurBackendDX11::RenderBlurPass(bool horizontal, float radius)
    {
        // Update constant buffer
        D3D11_MAPPED_SUBRESOURCE mapped;
        HRESULT hr = m_context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        if (SUCCEEDED(hr))
        {
            BlurParams* params = static_cast<BlurParams*>(mapped.pData);
            params->texelSizeX = 1.0f / static_cast<float>(m_textureWidth);
            params->texelSizeY = 1.0f / static_cast<float>(m_textureHeight);
            params->blurDirX = horizontal ? 1.0f : 0.0f;
            params->blurDirY = horizontal ? 0.0f : 1.0f;
            params->blurRadius = radius;
            m_context->Unmap(m_constantBuffer.Get(), 0);
        }

        m_context->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
    }
}

#endif // _WIN32
