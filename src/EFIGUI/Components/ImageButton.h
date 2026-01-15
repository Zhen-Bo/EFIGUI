// src/EFIGUI/Components/ImageButton.h
#pragma once
#include "imgui.h"
#include "../Styles/ImageButtonStyle.h"

namespace EFIGUI {

/// ImageButton - Clickable image with glow effects
/// @param str_id Unique string ID for the button
/// @param user_texture_id Texture ID (ImTextureID)
/// @param image_size Size of the image
/// @param uv0 UV coordinates of top-left corner (default: 0,0)
/// @param uv1 UV coordinates of bottom-right corner (default: 1,1)
/// @return true if clicked
bool ImageButton(const char* str_id, ImTextureID user_texture_id,
                 const ImVec2& image_size,
                 const ImVec2& uv0 = ImVec2(0, 0),
                 const ImVec2& uv1 = ImVec2(1, 1));

/// ImageButton with custom style
bool ImageButtonEx(const char* str_id, ImTextureID user_texture_id,
                   const ImVec2& image_size,
                   const ImVec2& uv0, const ImVec2& uv1,
                   const ImageButtonStyle& style);

/// ImageButton with frame background (matches ImGui::ImageButton signature more closely)
/// @param str_id Unique string ID for the button
/// @param user_texture_id Texture ID (ImTextureID)
/// @param image_size Size of the image
/// @param uv0 UV coordinates of top-left corner
/// @param uv1 UV coordinates of bottom-right corner
/// @param bg_col Background color (default: transparent)
/// @param tint_col Tint color (default: white)
/// @return true if clicked
bool ImageButtonWithBg(const char* str_id, ImTextureID user_texture_id,
                       const ImVec2& image_size,
                       const ImVec2& uv0 = ImVec2(0, 0),
                       const ImVec2& uv1 = ImVec2(1, 1),
                       const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
                       const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

} // namespace EFIGUI
