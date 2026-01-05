#pragma once
#include "imgui.h"
#include <vector>
#include <functional>
#include <optional>
#include <cstdint>

namespace EFIGUI
{
    // Z-Order from back to front
    enum class Layer : uint8_t
    {
        Background = 0,     // Bottommost (background decorations)
        Content,            // General window content
        Widget,             // Widget surfaces (button backgrounds, input boxes)
        WidgetGlow,         // Widget glow effects
        Popup,              // Popup/Dropdown surfaces
        PopupGlow,          // Popup glow effects
        Overlay,            // Tooltips, notifications
        Debug,              // Debug info (topmost)

        COUNT
    };

    // Convert Layer enum to string for debugging
    const char* LayerToString(Layer layer);

    // Global layer configuration
    struct LayerConfig
    {
        Layer defaultWidgetGlow      = Layer::WidgetGlow;
        Layer defaultMarqueeBorder   = Layer::WidgetGlow;
        Layer defaultButtonBackground = Layer::Widget;
        Layer defaultPopupBackground = Layer::Popup;
        Layer defaultPopupGlow       = Layer::PopupGlow;
        Layer defaultTooltip         = Layer::Overlay;

        bool autoElevateInPopup = true;  // Auto-elevate glow layers inside popups
    };

    // Deferred draw command
    struct DeferredDrawCommand
    {
        std::function<void(ImDrawList*)> command;
        Layer layer;
        int priority;
    };

    // Layer Manager (Singleton)
    class LayerManager
    {
    public:
        static LayerManager& Get();

        void SetConfig(const LayerConfig& config);
        const LayerConfig& GetConfig() const;

        // Deferred drawing API
        void AddDrawCommand(Layer layer, std::function<void(ImDrawList*)> command, int priority = 0);
        void AddRectFilled(Layer layer, ImVec2 min, ImVec2 max, ImU32 color, float rounding = 0.0f, ImDrawFlags flags = 0);
        void AddRect(Layer layer, ImVec2 min, ImVec2 max, ImU32 color, float rounding = 0.0f, ImDrawFlags flags = 0, float thickness = 1.0f);
        void AddCircleFilled(Layer layer, ImVec2 center, float radius, ImU32 color, int num_segments = 0);
        void AddLine(Layer layer, ImVec2 p1, ImVec2 p2, ImU32 color, float thickness = 1.0f);
        void AddText(Layer layer, ImVec2 pos, ImU32 color, const char* text);
        void AddText(Layer layer, ImFont* font, float font_size, ImVec2 pos, ImU32 color, const char* text);

        // Frame lifecycle
        void BeginFrame();
        void Flush(ImDrawList* targetDrawList = nullptr);

        // Popup context tracking
        void PushPopupContext();
        void PopPopupContext();
        bool IsInPopupContext() const;
        Layer GetEffectiveLayer(Layer requestedLayer) const;

    private:
        LayerManager() = default;
        LayerConfig m_config;
        std::vector<DeferredDrawCommand> m_commands[static_cast<size_t>(Layer::COUNT)];
        int m_popupContextDepth = 0;
    };

    // Convenience function
    inline LayerManager& Layers() { return LayerManager::Get(); }
}
