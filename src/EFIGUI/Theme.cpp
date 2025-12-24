#include "Theme.h"

namespace EFIGUI
{
    namespace Theme
    {
        // Custom accent colors (can be overridden by user)
        static ImU32 s_customAccentPrimary = 0;
        static ImU32 s_customAccentSecondary = 0;

        void Apply()
        {
            ImGuiStyle& style = ImGui::GetStyle();

            // Rounding
            style.WindowRounding = WindowRounding;
            style.FrameRounding = FrameRounding;
            style.GrabRounding = FrameRounding;
            style.TabRounding = FrameRounding;
            style.ScrollbarRounding = FrameRounding;
            style.ChildRounding = FrameRounding;
            style.PopupRounding = FrameRounding;

            // Spacing and padding
            style.WindowPadding = ImVec2(0.0f, 0.0f);  // We handle padding manually
            style.FramePadding = ImVec2(12.0f, 8.0f);
            style.ItemSpacing = ImVec2(ItemSpacing, ItemSpacing);
            style.ItemInnerSpacing = ImVec2(8.0f, 4.0f);
            style.ScrollbarSize = 12.0f;
            style.GrabMinSize = 10.0f;

            // Borders
            style.WindowBorderSize = 0.0f;  // Custom border handling
            style.FrameBorderSize = 0.0f;
            style.PopupBorderSize = 1.0f;

            // Colors
            ImVec4* colors = style.Colors;

            // Use custom accent if set
            ImU32 accentPrimary = s_customAccentPrimary ? s_customAccentPrimary : AccentCyan;
            ImU32 accentSecondary = s_customAccentSecondary ? s_customAccentSecondary : AccentPurple;

            // Window
            colors[ImGuiCol_WindowBg] = ToVec4(BackgroundDark);
            colors[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);  // Transparent by default
            colors[ImGuiCol_PopupBg] = ToVec4(BackgroundPanel);

            // Title (we use custom title bar, but set these just in case)
            colors[ImGuiCol_TitleBg] = ToVec4(TitleBarLeft);
            colors[ImGuiCol_TitleBgActive] = ToVec4(TitleBarRight);
            colors[ImGuiCol_TitleBgCollapsed] = ToVec4(TitleBarLeft);

            // Text
            colors[ImGuiCol_Text] = ToVec4(TextPrimary);
            colors[ImGuiCol_TextDisabled] = ToVec4(TextMuted);

            // Border
            colors[ImGuiCol_Border] = ToVec4(BorderDefault);
            colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

            // Frame (input boxes, etc)
            colors[ImGuiCol_FrameBg] = ToVec4(ButtonDefault);
            colors[ImGuiCol_FrameBgHovered] = ToVec4(ButtonHover);
            colors[ImGuiCol_FrameBgActive] = ToVec4(ButtonActive);

            // Button
            colors[ImGuiCol_Button] = ToVec4(ButtonDefault);
            colors[ImGuiCol_ButtonHovered] = ToVec4(ButtonHover);
            colors[ImGuiCol_ButtonActive] = ToVec4(ButtonActive);

            // Header (collapsing headers, selectables)
            colors[ImGuiCol_Header] = ToVec4(ButtonDefault);
            colors[ImGuiCol_HeaderHovered] = ToVec4(ButtonHover);
            colors[ImGuiCol_HeaderActive] = ToVec4(ButtonActive);

            // Tab
            colors[ImGuiCol_Tab] = ToVec4(ButtonDefault);
            colors[ImGuiCol_TabHovered] = ToVec4(accentPrimary);
            colors[ImGuiCol_TabActive] = ToVec4(ButtonActive);
            colors[ImGuiCol_TabUnfocused] = ToVec4(ButtonDefault);
            colors[ImGuiCol_TabUnfocusedActive] = ToVec4(ButtonHover);

            // Scrollbar
            colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.04f, 0.5f);
            colors[ImGuiCol_ScrollbarGrab] = ToVec4(BorderDefault);
            colors[ImGuiCol_ScrollbarGrabHovered] = ToVec4(BorderHover);
            colors[ImGuiCol_ScrollbarGrabActive] = ToVec4(accentPrimary);

            // Slider
            colors[ImGuiCol_SliderGrab] = ToVec4(accentPrimary);
            colors[ImGuiCol_SliderGrabActive] = ToVec4(accentSecondary);

            // Check mark
            colors[ImGuiCol_CheckMark] = ToVec4(accentPrimary);

            // Separator
            colors[ImGuiCol_Separator] = ToVec4(BorderDefault);
            colors[ImGuiCol_SeparatorHovered] = ToVec4(BorderHover);
            colors[ImGuiCol_SeparatorActive] = ToVec4(accentPrimary);

            // Resize grip
            colors[ImGuiCol_ResizeGrip] = ToVec4(BorderDefault);
            colors[ImGuiCol_ResizeGripHovered] = ToVec4(BorderHover);
            colors[ImGuiCol_ResizeGripActive] = ToVec4(accentPrimary);

            // Plot
            colors[ImGuiCol_PlotLines] = ToVec4(accentPrimary);
            colors[ImGuiCol_PlotLinesHovered] = ToVec4(accentSecondary);
            colors[ImGuiCol_PlotHistogram] = ToVec4(accentPrimary);
            colors[ImGuiCol_PlotHistogramHovered] = ToVec4(accentSecondary);

            // Table
            colors[ImGuiCol_TableHeaderBg] = ToVec4(BackgroundPanel);
            colors[ImGuiCol_TableBorderStrong] = ToVec4(BorderDefault);
            colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
            colors[ImGuiCol_TableRowBg] = ImVec4(0, 0, 0, 0);
            colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.03f);

            // Drag/Drop
            colors[ImGuiCol_DragDropTarget] = ToVec4(accentPrimary);

            // Nav
            colors[ImGuiCol_NavWindowingHighlight] = ToVec4(accentPrimary);
            colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.2f);
            colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.6f);
        }

        void SetAccentColor(ImU32 primary, ImU32 secondary)
        {
            s_customAccentPrimary = primary;
            s_customAccentSecondary = secondary;
        }
    }
}
