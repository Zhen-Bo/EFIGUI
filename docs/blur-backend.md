# Blur Backend

## Initialization

```cpp
// Without blur (works everywhere)
EFIGUI::Initialize();

// With DX11 blur (Windows + DirectX 11)
EFIGUI::Initialize(EFIGUI::BackendType::DX11, pDevice, width, height);
```

---

## Backend Types

| Type | Status | Description |
|------|--------|-------------|
| `None` | Available | No blur (fallback to solid colors) |
| `DX11` | Available | DirectX 11 (Windows only) |
| `DX12` | Planned | DirectX 12 |
| `Vulkan` | Planned | Vulkan |
| `OpenGL` | Planned | OpenGL |

---

## Using Blur

```cpp
// Check if blur is available
if (EFIGUI::HasBlurBackend())
{
    // Apply blur to a texture
    void* blurredTexture = EFIGUI::ApplyBlur(sourceSRV, 2.0f, 2);
}

// Handle window resize
EFIGUI::ResizeBlurBackend(newWidth, newHeight);
```

---

## API Reference

| Function | Description |
|----------|-------------|
| `HasBlurBackend()` | Check if blur backend is available and initialized |
| `ApplyBlur(sourceSRV, radius, passes)` | Apply blur effect to source texture, returns blurred SRV |
| `GetBlurResult()` | Get the last blur result texture |
| `ResizeBlurBackend(width, height)` | Resize blur textures (call when window resizes) |

---

## DX11 Requirements

- Windows platform
- DirectX 11 SDK
- `d3d11.lib` and `d3dcompiler.lib` for linking
- `d3dcompiler_47.dll` for runtime shader compilation
