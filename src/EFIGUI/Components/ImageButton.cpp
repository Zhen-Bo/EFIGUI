// src/EFIGUI/Components/ImageButton.cpp
#include "ImageButton.h"
#include "../Core/Animation.h"
#include "../Core/Draw.h"
#include "../Core/Style.h"
#include "imgui_internal.h"
#include <algorithm>

namespace EFIGUI {

bool ImageButtonEx(const char* str_id, ImTextureID user_texture_id,
                   const ImVec2& image_size,
                   const ImVec2& uv0, const ImVec2& uv1,
                   const ImageButtonStyle& style) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(str_id);

    // Calculate button size (image + padding)
    ImVec2 buttonSize(
        image_size.x + style.padding * 2,
        image_size.y + style.padding * 2
    );

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect bb(pos, ImVec2(pos.x + buttonSize.x, pos.y + buttonSize.y));

    ImGui::ItemSize(bb, 0.0f);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    // Get animation state
    Animation::WidgetState& animState = Animation::GetState(id);
    Animation::UpdateWidgetState(animState, hovered, held, false);

    ImDrawList* draw = window->DrawList;

    // Draw background
    ImU32 bgColor = held ? style.bgActiveColor :
                    (hovered ? style.bgHoverColor : style.bgColor);
    draw->AddRectFilled(bb.Min, bb.Max, bgColor, style.rounding);

    // Draw border
    ImU32 borderColor = (hovered || animState.hoverAnim > 0.1f)
                            ? style.borderHoverColor
                            : style.borderColor;
    draw->AddRect(bb.Min, bb.Max, borderColor, style.rounding, 0, style.borderWidth);

    // Draw glow on hover
    if (style.showGlow && animState.hoverAnim > 0.01f) {
        Draw::RectGlow(bb.Min, bb.Max, style.glowColor,
                       style.glowIntensity * animState.hoverAnim, 4.0f);
    }

    // Calculate image position (centered with padding)
    ImVec2 imageMin(bb.Min.x + style.padding, bb.Min.y + style.padding);
    ImVec2 imageMax(imageMin.x + image_size.x, imageMin.y + image_size.y);

    // Determine tint color
    ImU32 tintColor = held ? style.tintActiveColor :
                      (hovered ? style.tintHoverColor : style.tintColor);

    // Draw image
    draw->AddImage(user_texture_id, imageMin, imageMax, uv0, uv1, tintColor);

    return pressed;
}

bool ImageButton(const char* str_id, ImTextureID user_texture_id,
                 const ImVec2& image_size,
                 const ImVec2& uv0, const ImVec2& uv1) {
    return ImageButtonEx(str_id, user_texture_id, image_size, uv0, uv1,
                         StyleSystem::GetCurrentStyle<ImageButtonStyle>());
}

bool ImageButtonWithBg(const char* str_id, ImTextureID user_texture_id,
                       const ImVec2& image_size,
                       const ImVec2& uv0, const ImVec2& uv1,
                       const ImVec4& bg_col, const ImVec4& tint_col) {
    // Create a temporary style with custom colors
    ImageButtonStyle style = StyleSystem::GetCurrentStyle<ImageButtonStyle>();
    style.bgColor = ImGui::ColorConvertFloat4ToU32(bg_col);
    style.tintColor = ImGui::ColorConvertFloat4ToU32(tint_col);
    style.tintHoverColor = style.tintColor;
    style.tintActiveColor = style.tintColor;

    return ImageButtonEx(str_id, user_texture_id, image_size, uv0, uv1, style);
}

} // namespace EFIGUI
