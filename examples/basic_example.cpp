// EFIGUI Basic Example
// This example demonstrates the basic usage of EFIGUI components

#include <EFIGUI/EFIGUI.h>

// Your ImGui and graphics API initialization code here...
// This is just a demonstration of EFIGUI API usage

void RenderEFIGUIDemo()
{
    // =============================================
    // Example: Complete Window with Navigation
    // =============================================

    static int selectedNav = 0;
    static bool windowOpen = true;

    if (EFIGUI::BeginCustomWindow("EFIGUI Demo", &windowOpen))
    {
        // Left navbar
        ImGui::BeginChild("Navbar", ImVec2(EFIGUI::Theme::NavbarWidth, 0), false);
        {
            static bool navCollapsed = false;
            if (EFIGUI::NavbarHeader("Navigation", EFIGUI::Icons::Expand, EFIGUI::Icons::Collapse,
                                      navCollapsed, EFIGUI::Theme::NavbarWidth))
                navCollapsed = !navCollapsed;

            if (EFIGUI::NavItem(EFIGUI::Icons::Home, "Dashboard", selectedNav == 0))
                selectedNav = 0;
            if (EFIGUI::NavItem(EFIGUI::Icons::Cog, "Settings", selectedNav == 1))
                selectedNav = 1;
            if (EFIGUI::NavItem(EFIGUI::Icons::User, "Profile", selectedNav == 2))
                selectedNav = 2;

            EFIGUI::Separator();

            if (EFIGUI::NavItem(EFIGUI::Icons::Info, "About", selectedNav == 3))
                selectedNav = 3;
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Content area
        ImGui::BeginChild("Content", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(EFIGUI::Theme::ContentPadding, EFIGUI::Theme::ContentPadding));

            switch (selectedNav)
            {
            case 0:
                RenderDashboard();
                break;
            case 1:
                RenderSettings();
                break;
            case 2:
                RenderProfile();
                break;
            case 3:
                RenderAbout();
                break;
            }

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
    }
    EFIGUI::EndCustomWindow();
}

void RenderDashboard()
{
    EFIGUI::SectionHeader("Dashboard");

    // Feature cards
    ImGui::Columns(3, nullptr, false);

    static bool featureVisual = true;
    static bool featureProtection = false;
    static bool featurePerformance = true;

    EFIGUI::FeatureCard(EFIGUI::Icons::Eye, "Visual", "Enhanced visuals", &featureVisual);
    ImGui::NextColumn();

    EFIGUI::FeatureCard(EFIGUI::Icons::Shield, "Protection", "Anti-detection", &featureProtection);
    ImGui::NextColumn();

    EFIGUI::FeatureCard(EFIGUI::Icons::Bolt, "Performance", "Optimized", &featurePerformance);

    ImGui::Columns(1);

    EFIGUI::Spacing();

    // Status indicators
    EFIGUI::SectionHeader("Status");

    EFIGUI::StatusIndicator("System", EFIGUI::Theme::StatusSuccess, true);
    EFIGUI::StatusIndicator("Network", EFIGUI::Theme::StatusWarning, false);
    EFIGUI::StatusIndicator("Updates", EFIGUI::Theme::StatusInfo, true);

    EFIGUI::Spacing();

    // Progress bar
    static float progress = 0.65f;
    EFIGUI::ModernProgressBar(progress, ImVec2(-1, 0), "Loading Progress");
}

void RenderSettings()
{
    EFIGUI::SectionHeader("General Settings");

    static bool enabled1 = true;
    static bool enabled2 = false;
    static bool enabled3 = true;

    EFIGUI::ModernToggle("Enable Feature A", &enabled1);
    EFIGUI::ModernToggleWithDesc("Advanced Mode", "Enables experimental features", &enabled2);
    EFIGUI::ModernToggle("Auto-Update", &enabled3);

    EFIGUI::Spacing();
    EFIGUI::SectionHeader("Values");

    static float sliderVal = 0.5f;
    static int intVal = 50;

    EFIGUI::ModernSliderFloat("Intensity", &sliderVal, 0.0f, 1.0f, "%.2f");
    EFIGUI::ModernSliderInt("Count", &intVal, 0, 100, "%d items");

    EFIGUI::Spacing();
    EFIGUI::SectionHeader("Input");

    static char textBuffer[256] = "Hello EFIGUI!";
    EFIGUI::ModernInputText("Text Input", textBuffer, sizeof(textBuffer));

    static std::string stdString = "Modern C++ string";
    EFIGUI::ModernInputText("String Input", stdString);
}

void RenderProfile()
{
    EFIGUI::SectionHeader("User Profile");

    ImGui::Text("%s Username: Demo User", EFIGUI::Icons::User);
    ImGui::Text("%s Status: Active", EFIGUI::Icons::Check);

    EFIGUI::Spacing();
    EFIGUI::SectionHeader("Actions");

    if (EFIGUI::GlowButton("Save Changes", ImVec2(200, 40)))
    {
        // Save action
    }

    EFIGUI::SameLine();

    if (EFIGUI::DangerButton("Delete Account", ImVec2(200, 40)))
    {
        // Delete action
    }
}

void RenderAbout()
{
    EFIGUI::SectionHeader("About EFIGUI");

    ImGui::TextWrapped(
        "EFIGUI (Escape From ImGui) is a modern UI framework built on Dear ImGui. "
        "It features a cyberpunk-inspired theme with glassmorphism effects, "
        "smooth animations, and a complete set of styled components."
    );

    EFIGUI::Spacing();

    ImGui::Text("Version: %s", EFIGUI_VERSION_STRING);
    ImGui::Text("ImGui Version: %s", IMGUI_VERSION);

    EFIGUI::Spacing();

    if (EFIGUI::GlowButton("Visit GitHub", ImVec2(200, 40)))
    {
        // Open GitHub
    }
}

// =============================================
// Initialization Example
// =============================================

/*
int main()
{
    // Initialize your graphics API (DX11, etc.)
    // Initialize Dear ImGui

    // Initialize EFIGUI with DX11 blur backend
    EFIGUI::Initialize(EFIGUI::BackendType::DX11, pDevice, windowWidth, windowHeight);

    // Main loop
    while (running)
    {
        // Begin ImGui frame
        ImGui_ImplXXX_NewFrame();
        ImGui::NewFrame();

        // Render EFIGUI demo
        RenderEFIGUIDemo();

        // End ImGui frame
        ImGui::Render();
        // Present

        // Handle resize
        if (resized)
        {
            EFIGUI::ResizeBlurBackend(newWidth, newHeight);
        }
    }

    // Cleanup
    EFIGUI::Shutdown();
    // Shutdown ImGui
    // Shutdown graphics API

    return 0;
}
*/
