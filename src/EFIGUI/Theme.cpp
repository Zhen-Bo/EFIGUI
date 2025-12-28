#include "Theme.h"

namespace EFIGUI
{
    // =============================================
    // ThemeManager Singleton Implementation
    // =============================================

    ThemeManager& ThemeManager::Instance()
    {
        static ThemeManager instance;
        return instance;
    }

    void ThemeManager::LoadPreset(ThemePreset preset)
    {
        switch (preset)
        {
        case ThemePreset::Cyberpunk:
        default:
            // Reset to default Cyberpunk theme (struct default values)
            m_config = ThemeConfig{};
            break;
        }
    }

    namespace Theme
    {
        void Apply()
        {
            ImGuiStyle& style = ImGui::GetStyle();
            const auto& config = GetConfig();
            const auto& dim = config.dimensions;
            const auto& colors = config.colors;

            // Rounding
            style.WindowRounding = dim.windowRounding;
            style.FrameRounding = dim.frameRounding;
            style.GrabRounding = dim.frameRounding;
            style.TabRounding = dim.frameRounding;
            style.ScrollbarRounding = dim.frameRounding;
            style.ChildRounding = dim.frameRounding;
            style.PopupRounding = dim.frameRounding;

            // Spacing and padding
            style.WindowPadding = ImVec2(0.0f, 0.0f);  // We handle padding manually
            style.FramePadding = ImVec2(12.0f, 8.0f);
            style.ItemSpacing = ImVec2(dim.itemSpacing, dim.itemSpacing);
            style.ItemInnerSpacing = ImVec2(8.0f, 4.0f);
            style.ScrollbarSize = 12.0f;
            style.GrabMinSize = 10.0f;

            // Borders
            style.WindowBorderSize = 0.0f;  // Custom border handling
            style.FrameBorderSize = 0.0f;
            style.PopupBorderSize = 1.0f;

            // Colors
            ImVec4* imColors = style.Colors;

            // Window
            imColors[ImGuiCol_WindowBg] = ToVec4(colors.backgroundDark);
            imColors[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);  // Transparent by default
            imColors[ImGuiCol_PopupBg] = ToVec4(colors.backgroundPanel);

            // Title (we use custom title bar, but set these just in case)
            imColors[ImGuiCol_TitleBg] = ToVec4(colors.titleBarLeft);
            imColors[ImGuiCol_TitleBgActive] = ToVec4(colors.titleBarRight);
            imColors[ImGuiCol_TitleBgCollapsed] = ToVec4(colors.titleBarLeft);

            // Text
            imColors[ImGuiCol_Text] = ToVec4(colors.textPrimary);
            imColors[ImGuiCol_TextDisabled] = ToVec4(colors.textMuted);

            // Border
            imColors[ImGuiCol_Border] = ToVec4(colors.borderDefault);
            imColors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

            // Frame (input boxes, etc)
            imColors[ImGuiCol_FrameBg] = ToVec4(colors.buttonDefault);
            imColors[ImGuiCol_FrameBgHovered] = ToVec4(colors.buttonHover);
            imColors[ImGuiCol_FrameBgActive] = ToVec4(colors.buttonActive);

            // Button
            imColors[ImGuiCol_Button] = ToVec4(colors.buttonDefault);
            imColors[ImGuiCol_ButtonHovered] = ToVec4(colors.buttonHover);
            imColors[ImGuiCol_ButtonActive] = ToVec4(colors.buttonActive);

            // Header (collapsing headers, selectables)
            imColors[ImGuiCol_Header] = ToVec4(colors.buttonDefault);
            imColors[ImGuiCol_HeaderHovered] = ToVec4(colors.buttonHover);
            imColors[ImGuiCol_HeaderActive] = ToVec4(colors.buttonActive);

            // Tab
            imColors[ImGuiCol_Tab] = ToVec4(colors.buttonDefault);
            imColors[ImGuiCol_TabHovered] = ToVec4(colors.accentPrimary);
            imColors[ImGuiCol_TabActive] = ToVec4(colors.buttonActive);
            imColors[ImGuiCol_TabUnfocused] = ToVec4(colors.buttonDefault);
            imColors[ImGuiCol_TabUnfocusedActive] = ToVec4(colors.buttonHover);

            // Scrollbar
            imColors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.04f, 0.5f);
            imColors[ImGuiCol_ScrollbarGrab] = ToVec4(colors.borderDefault);
            imColors[ImGuiCol_ScrollbarGrabHovered] = ToVec4(colors.borderHover);
            imColors[ImGuiCol_ScrollbarGrabActive] = ToVec4(colors.accentPrimary);

            // Slider
            imColors[ImGuiCol_SliderGrab] = ToVec4(colors.accentPrimary);
            imColors[ImGuiCol_SliderGrabActive] = ToVec4(colors.accentSecondary);

            // Check mark
            imColors[ImGuiCol_CheckMark] = ToVec4(colors.accentPrimary);

            // Separator
            imColors[ImGuiCol_Separator] = ToVec4(colors.borderDefault);
            imColors[ImGuiCol_SeparatorHovered] = ToVec4(colors.borderHover);
            imColors[ImGuiCol_SeparatorActive] = ToVec4(colors.accentPrimary);

            // Resize grip
            imColors[ImGuiCol_ResizeGrip] = ToVec4(colors.borderDefault);
            imColors[ImGuiCol_ResizeGripHovered] = ToVec4(colors.borderHover);
            imColors[ImGuiCol_ResizeGripActive] = ToVec4(colors.accentPrimary);

            // Plot
            imColors[ImGuiCol_PlotLines] = ToVec4(colors.accentPrimary);
            imColors[ImGuiCol_PlotLinesHovered] = ToVec4(colors.accentSecondary);
            imColors[ImGuiCol_PlotHistogram] = ToVec4(colors.accentPrimary);
            imColors[ImGuiCol_PlotHistogramHovered] = ToVec4(colors.accentSecondary);

            // Table
            imColors[ImGuiCol_TableHeaderBg] = ToVec4(colors.backgroundPanel);
            imColors[ImGuiCol_TableBorderStrong] = ToVec4(colors.borderDefault);
            imColors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
            imColors[ImGuiCol_TableRowBg] = ImVec4(0, 0, 0, 0);
            imColors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.03f);

            // Drag/Drop
            imColors[ImGuiCol_DragDropTarget] = ToVec4(colors.accentPrimary);

            // Nav
            imColors[ImGuiCol_NavWindowingHighlight] = ToVec4(colors.accentPrimary);
            imColors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.2f);
            imColors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.6f);
        }

        void SetAccentColor(ImU32 primary, ImU32 secondary)
        {
            auto& colors = GetConfig().colors;
            if (primary != 0)
            {
                colors.accentPrimary = primary;
                // Update derived glow color
                auto rgb = ExtractRGB(primary);
                colors.accentPrimaryGlow = IM_COL32(rgb.r, rgb.g, rgb.b, 80);
                colors.textAccent = primary;
            }
            if (secondary != 0)
            {
                colors.accentSecondary = secondary;
                // Update derived glow color
                auto rgb = ExtractRGB(secondary);
                colors.accentSecondaryGlow = IM_COL32(rgb.r, rgb.g, rgb.b, 80);
            }
        }
    }
}
