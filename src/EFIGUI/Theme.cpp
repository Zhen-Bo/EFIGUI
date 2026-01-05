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

        // =============================================
        // ShowThemeStyleEditor Implementation
        // =============================================

        // Helper to edit ImU32 color as float[4]
        static bool ColorEdit4U32(const char* label, ImU32* color)
        {
            ImVec4 col = ImGui::ColorConvertU32ToFloat4(*color);
            if (ImGui::ColorEdit4(label, &col.x, ImGuiColorEditFlags_AlphaBar))
            {
                *color = ImGui::ColorConvertFloat4ToU32(col);
                return true;
            }
            return false;
        }

        void ShowThemeStyleEditor(bool* p_open)
        {
            if (!ImGui::Begin("EFIGUI Theme Style Editor", p_open, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::End();
                return;
            }

            auto& config = GetConfig();

            if (ImGui::Button("Reset to Default"))
            {
                ResetToDefault();
            }

            ImGui::Separator();

            // Colors section
            if (ImGui::CollapsingHeader("Colors", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto& c = config.colors;
                ColorEdit4U32("Background Dark", &c.backgroundDark);
                ColorEdit4U32("Background Panel", &c.backgroundPanel);
                ColorEdit4U32("Background Navbar", &c.backgroundNavbar);
                ColorEdit4U32("Background Content", &c.backgroundContent);
                ImGui::Separator();
                ColorEdit4U32("Accent Primary", &c.accentPrimary);
                ColorEdit4U32("Accent Secondary", &c.accentSecondary);
                ColorEdit4U32("Accent Tertiary", &c.accentTertiary);
                ColorEdit4U32("Accent Success", &c.accentSuccess);
                ImGui::Separator();
                ColorEdit4U32("Text Primary", &c.textPrimary);
                ColorEdit4U32("Text Secondary", &c.textSecondary);
                ColorEdit4U32("Text Muted", &c.textMuted);
                ColorEdit4U32("Text Accent", &c.textAccent);
                ImGui::Separator();
                ColorEdit4U32("Border Default", &c.borderDefault);
                ColorEdit4U32("Border Hover", &c.borderHover);
                ColorEdit4U32("Border Active", &c.borderActive);
                ImGui::Separator();
                ColorEdit4U32("Title Bar Left", &c.titleBarLeft);
                ColorEdit4U32("Title Bar Right", &c.titleBarRight);
            }

            // Dimensions section
            if (ImGui::CollapsingHeader("Dimensions"))
            {
                auto& d = config.dimensions;
                ImGui::DragFloat("Window Rounding", &d.windowRounding, 0.5f, 0.0f, 24.0f);
                ImGui::DragFloat("Frame Rounding", &d.frameRounding, 0.5f, 0.0f, 16.0f);
                ImGui::DragFloat("Button Rounding", &d.buttonRounding, 0.5f, 0.0f, 16.0f);
                ImGui::DragFloat("Nav Item Rounding", &d.navItemRounding, 0.5f, 0.0f, 16.0f);
                ImGui::Separator();
                ImGui::DragFloat("Title Bar Height", &d.titleBarHeight, 0.5f, 20.0f, 60.0f);
                ImGui::DragFloat("Navbar Width", &d.navbarWidth, 1.0f, 100.0f, 400.0f);
                ImGui::DragFloat("Navbar Width Collapsed", &d.navbarWidthCollapsed, 1.0f, 40.0f, 120.0f);
                ImGui::DragFloat("Nav Item Height", &d.navItemHeight, 0.5f, 24.0f, 80.0f);
                ImGui::DragFloat("Nav Item Padding", &d.navItemPadding, 0.5f, 0.0f, 24.0f);
                ImGui::Separator();
                ImGui::DragFloat("Content Padding", &d.contentPadding, 0.5f, 0.0f, 48.0f);
                ImGui::DragFloat("Item Spacing", &d.itemSpacing, 0.5f, 0.0f, 24.0f);
                ImGui::Separator();
                ImGui::DragFloat("Toggle Width", &d.toggleWidth, 0.5f, 24.0f, 96.0f);
                ImGui::DragFloat("Toggle Height", &d.toggleHeight, 0.5f, 12.0f, 48.0f);
                ImGui::DragFloat("Toggle Knob Size", &d.toggleKnobSize, 0.5f, 8.0f, 32.0f);
                ImGui::DragFloat("Toggle Label Gap", &d.toggleLabelGap, 0.5f, 0.0f, 32.0f);
            }

            // Effects section
            if (ImGui::CollapsingHeader("Effects"))
            {
                auto& e = config.effects;
                ImGui::DragFloat("Glow Intensity", &e.glowIntensity, 0.05f, 0.0f, 2.0f);
                ImGui::DragFloat("Glow Radius", &e.glowRadius, 0.5f, 0.0f, 24.0f);
                ImGui::Separator();
                ImGui::DragInt("Button Glow Layers", &e.buttonGlowLayers, 0.2f, 1, 10);
                ImGui::DragFloat("Button Glow Expand", &e.buttonGlowExpand, 0.1f, 0.0f, 8.0f);
                ImGui::Separator();
                ImGui::DragInt("Slider Glow Layers", &e.sliderGlowLayers, 0.2f, 1, 10);
                ImGui::DragFloat("Slider Knob Glow Expand", &e.sliderKnobGlowExpand, 0.1f, 0.0f, 8.0f);
                ImGui::DragFloat("Slider Input Glow Expand", &e.sliderInputGlowExpand, 0.1f, 0.0f, 8.0f);
            }

            // Animation section
            if (ImGui::CollapsingHeader("Animation"))
            {
                auto& a = config.animation;
                ImGui::DragFloat("Default Speed", &a.defaultSpeed, 0.5f, 1.0f, 30.0f);
                ImGui::DragFloat("Pulse Frequency", &a.pulseFrequency, 0.1f, 0.1f, 10.0f);
                ImGui::DragFloat("Breathe Frequency", &a.breatheFrequency, 0.1f, 0.1f, 5.0f);
                ImGui::DragFloat("Sweep Frequency", &a.sweepFrequency, 0.1f, 0.1f, 3.0f);
                ImGui::Separator();
                ImGui::DragFloat("Marquee Speed", &a.marqueeSpeed, 0.01f, 0.01f, 0.5f);
                ImGui::DragFloat("Marquee Sweep Length", &a.marqueeSweepLength, 0.01f, 0.05f, 0.5f);
            }

            // Slider Theme section
            if (ImGui::CollapsingHeader("Slider"))
            {
                auto& s = config.slider;
                ImGui::DragFloat("Height", &s.height, 0.5f, 12.0f, 48.0f);
                ImGui::DragFloat("Track Height", &s.trackHeight, 0.5f, 2.0f, 16.0f);
                ImGui::DragFloat("Knob Radius", &s.knobRadius, 0.5f, 4.0f, 20.0f);
                ImGui::Separator();
                ImGui::DragFloat("Input Width", &s.inputWidth, 1.0f, 30.0f, 100.0f);
                ImGui::DragFloat("Input Height", &s.inputHeight, 0.5f, 16.0f, 40.0f);
                ImGui::DragFloat("Input Gap", &s.inputGap, 0.5f, 0.0f, 24.0f);
                ImGui::DragFloat("Input Rounding", &s.inputRounding, 0.5f, 0.0f, 12.0f);
                ImGui::Separator();
                ImGui::DragInt("Glow Layers", &s.glowLayers, 0.2f, 1, 10);
                ImGui::DragFloat("Knob Glow Expand", &s.knobGlowExpand, 0.1f, 0.0f, 8.0f);
                ImGui::DragFloat("Input Glow Expand", &s.inputGlowExpand, 0.1f, 0.0f, 8.0f);
            }

            // Toggle Theme section
            if (ImGui::CollapsingHeader("Toggle"))
            {
                auto& t = config.toggle;
                ImGui::DragFloat("Width", &t.width, 0.5f, 24.0f, 96.0f);
                ImGui::DragFloat("Height", &t.height, 0.5f, 12.0f, 48.0f);
                ImGui::DragFloat("Knob Size", &t.knobSize, 0.5f, 8.0f, 32.0f);
                ImGui::DragFloat("Label Gap", &t.labelGap, 0.5f, 0.0f, 32.0f);
                ImGui::Separator();
                ImGui::DragFloat("Anim Speed", &t.animSpeed, 0.5f, 1.0f, 30.0f);
                ImGui::DragFloat("Glow Threshold", &t.glowThreshold, 0.05f, 0.0f, 1.0f);
                ImGui::DragFloat("Glow Radius", &t.glowRadius, 0.5f, 0.0f, 16.0f);
                ImGui::DragFloat("Disabled Alpha", &t.disabledAlpha, 0.05f, 0.0f, 1.0f);
                ImGui::Separator();
                ImGui::DragFloat("Desc Offset X", &t.descOffsetX, 1.0f, 0.0f, 120.0f);
                ImGui::DragFloat("Desc Spacing Y", &t.descSpacingY, 0.5f, 0.0f, 16.0f);
            }

            // Card Theme section
            if (ImGui::CollapsingHeader("Card"))
            {
                auto& c = config.card;
                ImGui::DragFloat("Icon Size", &c.iconSize, 0.5f, 16.0f, 64.0f);
                ImGui::DragFloat("Icon Padding", &c.iconPadding, 0.5f, 0.0f, 32.0f);
                ImGui::DragFloat("Icon Text Offset", &c.iconTextOffset, 0.5f, 0.0f, 16.0f);
                ImGui::DragFloat("Base Height", &c.baseHeight, 1.0f, 32.0f, 120.0f);
                ImGui::DragFloat("Min Text Width", &c.minTextWidth, 1.0f, 20.0f, 100.0f);
                ImGui::DragFloat("Name Offset Y", &c.nameOffsetY, 0.5f, 0.0f, 32.0f);
                ImGui::DragFloat("Desc Offset Y", &c.descOffsetY, 0.5f, 0.0f, 48.0f);
                ImGui::Separator();
                ImGui::DragFloat("Toggle Width", &c.toggleWidth, 0.5f, 20.0f, 80.0f);
                ImGui::DragFloat("Toggle Height", &c.toggleHeight, 0.5f, 12.0f, 48.0f);
                ImGui::DragFloat("Toggle Rounding", &c.toggleRounding, 0.5f, 0.0f, 24.0f);
                ImGui::DragFloat("Toggle Right Margin", &c.toggleRightMargin, 1.0f, 0.0f, 100.0f);
                ImGui::DragFloat("Knob Radius", &c.knobRadius, 0.5f, 4.0f, 20.0f);
                ImGui::DragFloat("Knob Padding", &c.knobPadding, 0.5f, 0.0f, 12.0f);
                ImGui::DragFloat("Knob Travel", &c.knobTravel, 0.5f, 4.0f, 32.0f);
                ImGui::DragFloat("Toggle Anim Speed", &c.toggleAnimSpeed, 0.5f, 1.0f, 30.0f);
                ImGui::Separator();
                ImGui::DragFloat("Section Line Offset", &c.sectionLineOffset, 0.5f, 0.0f, 16.0f);
                ImGui::DragFloat("Section Spacing", &c.sectionSpacing, 0.5f, 0.0f, 32.0f);
            }

            // Nav Theme section
            if (ImGui::CollapsingHeader("Navigation"))
            {
                auto& n = config.nav;
                ImGui::DragFloat("Icon Padding", &n.iconPadding, 0.5f, 0.0f, 32.0f);
                ImGui::DragFloat("Label Spacing", &n.labelSpacing, 0.5f, 0.0f, 32.0f);
                ImGui::DragFloat("Collapsed Icon Scale", &n.collapsedIconScale, 0.05f, 0.5f, 2.0f);
                ImGui::Separator();
                ImGui::DragFloat("Accent Bar Width", &n.accentBarWidth, 0.5f, 1.0f, 12.0f);
                ImGui::DragFloat("Accent Bar Height Ratio", &n.accentBarHeightRatio, 0.05f, 0.2f, 1.0f);
                ImGui::DragFloat("Accent Bar Glow Radius", &n.accentBarGlowRadius, 0.5f, 0.0f, 16.0f);
                ImGui::Separator();
                ImGui::DragFloat("Section Padding X", &n.sectionPaddingX, 0.5f, 0.0f, 32.0f);
                ImGui::DragFloat("Section Padding Y", &n.sectionPaddingY, 0.5f, 0.0f, 16.0f);
                ImGui::DragFloat("Section Height", &n.sectionHeight, 0.5f, 12.0f, 48.0f);
                ImGui::Separator();
                ImGui::DragInt("Accent Glow Alpha", &n.accentGlowAlpha, 1.0f, 0, 255);
                ImGui::DragInt("Bg Alpha Multiplier", &n.bgAlphaMultiplier, 1.0f, 0, 255);
                ImGui::DragInt("Hover Bg Alpha", &n.hoverBgAlpha, 1.0f, 0, 255);
            }

            // Button Theme section
            if (ImGui::CollapsingHeader("Button"))
            {
                auto& b = config.button;
                ImGui::DragFloat("Padding X", &b.paddingX, 1.0f, 8.0f, 64.0f);
                ImGui::DragFloat("Height", &b.height, 0.5f, 20.0f, 60.0f);
                ImGui::Separator();
                ImGui::DragFloat("Glow Intensity Base", &b.glowIntensityBase, 0.05f, 0.0f, 2.0f);
                ImGui::DragFloat("Glow Intensity Active", &b.glowIntensityActive, 0.05f, 0.0f, 2.0f);
                ImGui::Separator();
                ImGui::Text("Cooldown Button:");
                ImGui::DragFloat("Cooldown Threshold", &b.cooldownThreshold, 0.01f, 0.0f, 0.5f);
                ImGui::DragInt("Cooldown Overlay Alpha", &b.cooldownOverlayAlpha, 1.0f, 0, 255);
                ImGui::DragInt("Edge Glow Layers", &b.edgeGlowLayers, 0.2f, 1, 10);
                ImGui::DragFloat("Edge Glow Base Width", &b.edgeGlowBaseWidth, 0.5f, 0.0f, 10.0f);
                ImGui::DragFloat("Edge Glow Expand Step", &b.edgeGlowExpandStep, 0.5f, 0.0f, 10.0f);
                ImGui::DragFloat("Edge Glow Base Alpha", &b.edgeGlowBaseAlpha, 0.05f, 0.0f, 1.0f);
                ImGui::DragFloat("Particle Radius Small", &b.particleRadiusSmall, 0.5f, 1.0f, 10.0f);
                ImGui::DragFloat("Particle Radius Large", &b.particleRadiusLarge, 0.5f, 2.0f, 20.0f);
                ImGui::DragInt("Particle Alpha Bright", &b.particleAlphaBright, 1.0f, 0, 255);
                ImGui::DragInt("Particle Alpha Dim", &b.particleAlphaDim, 1.0f, 0, 255);
            }

            // Window Theme section
            if (ImGui::CollapsingHeader("Window"))
            {
                auto& w = config.window;
                ImGui::DragFloat("Title Bar Height", &w.titleBarHeight, 0.5f, 20.0f, 60.0f);
                ImGui::DragFloat("Title Text Padding", &w.titleTextPadding, 0.5f, 0.0f, 32.0f);
                ImGui::DragFloat("Title Button Size", &w.titleButtonSize, 0.5f, 12.0f, 40.0f);
                ImGui::DragFloat("Title Button Margin", &w.titleButtonMargin, 0.5f, 0.0f, 32.0f);
                ImGui::DragFloat("Title Button Spacing", &w.titleButtonSpacing, 0.5f, 0.0f, 24.0f);
                ImGui::DragFloat("Content Spacing Y", &w.contentSpacingY, 0.5f, 0.0f, 24.0f);
                ImGui::Separator();
                ImGui::DragFloat("Navbar Button Size", &w.navbarButtonSize, 0.5f, 12.0f, 40.0f);
                ImGui::DragFloat("Navbar Icon Padding", &w.navbarIconPadding, 0.5f, 0.0f, 24.0f);
                ImGui::DragFloat("Navbar Button Padding", &w.navbarButtonPadding, 0.5f, 0.0f, 16.0f);
                ImGui::Separator();
                ImGui::DragFloat("Glow Radius Base", &w.glowRadiusBase, 0.5f, 0.0f, 16.0f);
                ImGui::DragFloat("Glow Radius Expand", &w.glowRadiusExpand, 0.5f, 0.0f, 8.0f);
                ImGui::DragInt("Glow Anim Alpha", &w.glowAnimAlpha, 1.0f, 0, 255);
            }

            // Draw Theme section
            if (ImGui::CollapsingHeader("Draw"))
            {
                auto& dr = config.draw;
                ImGui::Text("RectGlow:");
                ImGui::DragFloat("Glow Min Intensity", &dr.glowMinIntensity, 0.01f, 0.0f, 0.5f);
                ImGui::DragInt("Glow Layer Count", &dr.glowLayerCount, 0.2f, 1, 10);
                ImGui::DragFloat("Glow Alpha Multiplier", &dr.glowAlphaMultiplier, 0.05f, 0.0f, 1.0f);
                ImGui::DragFloat("Glow Line Thickness", &dr.glowLineThickness, 0.5f, 0.5f, 10.0f);
                ImGui::Separator();
                ImGui::Text("NeonBorder:");
                ImGui::DragFloat("Neon Glow Intensity", &dr.neonGlowIntensity, 0.05f, 0.0f, 2.0f);
                ImGui::DragFloat("Neon Glow Radius", &dr.neonGlowRadius, 0.5f, 0.0f, 16.0f);
                ImGui::Separator();
                ImGui::Text("GlowLayers:");
                ImGui::DragFloat("Rect Alpha", &dr.glowLayersRectAlpha, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Circle Alpha", &dr.glowLayersCircleAlpha, 0.01f, 0.0f, 1.0f);
                ImGui::Separator();
                ImGui::Text("MarqueeBorder:");
                ImGui::DragInt("Num Segments", &dr.marqueeNumSegments, 1.0f, 20, 200);
                ImGui::DragInt("Corner Segments", &dr.marqueeCornerSegments, 0.2f, 2, 20);
                ImGui::DragInt("Min Edge Segments", &dr.marqueeMinEdgeSegments, 0.2f, 1, 10);
                ImGui::DragInt("Min Alpha", &dr.marqueeMinAlpha, 1.0f, 0, 100);
                ImGui::DragFloat("Hover Threshold", &dr.marqueeHoverThreshold, 0.05f, 0.0f, 1.0f);
                ImGui::Separator();
                ImGui::Text("Glassmorphism:");
                ImGui::DragInt("Glass Blur Alpha", &dr.glassBlurAlpha, 1.0f, 0, 255);
            }

            // Layout Theme section
            if (ImGui::CollapsingHeader("Layout"))
            {
                auto& l = config.layout;
                ImGui::Text("Separator:");
                ImGui::DragFloat("Offset Y", &l.separatorOffsetY, 0.5f, 0.0f, 16.0f);
                ImGui::DragFloat("Spacing", &l.separatorSpacing, 0.5f, 0.0f, 24.0f);
                ImGui::Separator();
                ImGui::Text("Tooltip:");
                ImGui::DragFloat("Padding X", &l.tooltipPaddingX, 0.5f, 0.0f, 24.0f);
                ImGui::DragFloat("Padding Y", &l.tooltipPaddingY, 0.5f, 0.0f, 24.0f);
            }

            // NumericInput Theme section
            if (ImGui::CollapsingHeader("NumericInput"))
            {
                auto& ni = config.numericInput;
                ImGui::Text("Dimensions:");
                ImGui::DragFloat("Default Width", &ni.defaultWidth, 1.0f, 30.0f, 100.0f);
                ImGui::DragFloat("Default Height", &ni.defaultHeight, 0.5f, 16.0f, 40.0f);
                ImGui::DragFloat("Rounding", &ni.rounding, 0.5f, 0.0f, 12.0f);
                ImGui::DragFloat("Inset", &ni.inset, 0.25f, 0.0f, 8.0f);
                ImGui::DragFloat("Padding X", &ni.paddingX, 0.5f, 0.0f, 16.0f);
                ImGui::Separator();
                ImGui::Text("Effects:");
                ImGui::DragFloat("Glow Expand Size", &ni.glowExpandSize, 0.5f, 0.0f, 16.0f);
                ImGui::DragFloat("Hover Threshold", &ni.hoverThreshold, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Glow Threshold", &ni.glowThreshold, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Editing Glow Intensity", &ni.editingGlowIntensity, 0.05f, 0.0f, 1.0f);
                ImGui::Separator();
                ImGui::Text("Bezel/Frame:");
                ColorEdit4U32("Bezel Color", &ni.bezelColor);
                ImGui::Separator();
                ImGui::Text("Screen Background:");
                ColorEdit4U32("Screen BG Default", &ni.screenBgDefault);
                ColorEdit4U32("Screen BG Hover", &ni.screenBgHover);
                ColorEdit4U32("Screen BG Editing", &ni.screenBgEditing);
                ImGui::Separator();
                ImGui::Text("Inner Border:");
                ColorEdit4U32("Inner Border Top", &ni.innerBorderTop);
                ColorEdit4U32("Inner Border Bottom", &ni.innerBorderBottom);
                ImGui::Separator();
                ImGui::Text("Outer Border:");
                ColorEdit4U32("Outer Border Default", &ni.outerBorderDefault);
                ColorEdit4U32("Outer Border Hover", &ni.outerBorderHover);
                ImGui::Separator();
                ImGui::Text("Text:");
                ColorEdit4U32("Text Default", &ni.textDefault);
            }

            ImGui::End();
        }
    }
}
