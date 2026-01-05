#include "Layer.h"
#include <algorithm>
#include <string>

namespace EFIGUI
{
    const char* LayerToString(Layer layer)
    {
        switch (layer)
        {
            case Layer::Background:  return "Background";
            case Layer::Content:     return "Content";
            case Layer::Widget:      return "Widget";
            case Layer::WidgetGlow:  return "WidgetGlow";
            case Layer::Popup:       return "Popup";
            case Layer::PopupGlow:   return "PopupGlow";
            case Layer::Overlay:     return "Overlay";
            case Layer::Debug:       return "Debug";
            default:                 return "Unknown";
        }
    }

    LayerManager& LayerManager::Get()
    {
        static LayerManager instance;
        return instance;
    }

    void LayerManager::SetConfig(const LayerConfig& config)
    {
        m_config = config;
    }

    const LayerConfig& LayerManager::GetConfig() const
    {
        return m_config;
    }

    void LayerManager::AddDrawCommand(Layer layer, std::function<void(ImDrawList*)> command, int priority)
    {
        Layer effectiveLayer = GetEffectiveLayer(layer);
        size_t idx = static_cast<size_t>(effectiveLayer);
        if (idx < static_cast<size_t>(Layer::COUNT))
        {
            m_commands[idx].push_back({ std::move(command), effectiveLayer, priority });
        }
    }

    void LayerManager::AddRectFilled(Layer layer, ImVec2 min, ImVec2 max, ImU32 color, float rounding, ImDrawFlags flags)
    {
        AddDrawCommand(layer, [=](ImDrawList* draw) {
            draw->AddRectFilled(min, max, color, rounding, flags);
        });
    }

    void LayerManager::AddRect(Layer layer, ImVec2 min, ImVec2 max, ImU32 color, float rounding, ImDrawFlags flags, float thickness)
    {
        AddDrawCommand(layer, [=](ImDrawList* draw) {
            draw->AddRect(min, max, color, rounding, flags, thickness);
        });
    }

    void LayerManager::AddCircleFilled(Layer layer, ImVec2 center, float radius, ImU32 color, int num_segments)
    {
        AddDrawCommand(layer, [=](ImDrawList* draw) {
            draw->AddCircleFilled(center, radius, color, num_segments);
        });
    }

    void LayerManager::AddLine(Layer layer, ImVec2 p1, ImVec2 p2, ImU32 color, float thickness)
    {
        AddDrawCommand(layer, [=](ImDrawList* draw) {
            draw->AddLine(p1, p2, color, thickness);
        });
    }

    void LayerManager::AddText(Layer layer, ImVec2 pos, ImU32 color, const char* text)
    {
        // Copy the string to avoid dangling pointer
        std::string textCopy = text;
        AddDrawCommand(layer, [=](ImDrawList* draw) {
            draw->AddText(pos, color, textCopy.c_str());
        });
    }

    void LayerManager::AddText(Layer layer, ImFont* font, float font_size, ImVec2 pos, ImU32 color, const char* text)
    {
        // Copy the string to avoid dangling pointer
        std::string textCopy = text;
        AddDrawCommand(layer, [=](ImDrawList* draw) {
            draw->AddText(font, font_size, pos, color, textCopy.c_str());
        });
    }

    void LayerManager::BeginFrame()
    {
        // Clear all command buffers from previous frame
        for (size_t i = 0; i < static_cast<size_t>(Layer::COUNT); i++)
        {
            m_commands[i].clear();
        }
        m_popupContextDepth = 0;
    }

    void LayerManager::Flush(ImDrawList* targetDrawList)
    {
        // Default to ForegroundDrawList if null, maintaining current behavior
        ImDrawList* draw = targetDrawList ? targetDrawList : ImGui::GetForegroundDrawList();

        // Execute commands layer by layer (Background first, Debug last)
        for (size_t layerIdx = 0; layerIdx < static_cast<size_t>(Layer::COUNT); layerIdx++)
        {
            auto& commands = m_commands[layerIdx];

            // Sort by priority within each layer (lower priority first)
            std::stable_sort(commands.begin(), commands.end(),
                [](const DeferredDrawCommand& a, const DeferredDrawCommand& b) {
                    return a.priority < b.priority;
                });

            // Execute all commands for this layer
            for (const auto& cmd : commands)
            {
                cmd.command(draw);
            }
        }
    }

    void LayerManager::PushPopupContext()
    {
        m_popupContextDepth++;
    }

    void LayerManager::PopPopupContext()
    {
        if (m_popupContextDepth > 0)
        {
            m_popupContextDepth--;
        }
    }

    bool LayerManager::IsInPopupContext() const
    {
        return m_popupContextDepth > 0;
    }

    Layer LayerManager::GetEffectiveLayer(Layer requestedLayer) const
    {
        if (!m_config.autoElevateInPopup || !IsInPopupContext())
        {
            return requestedLayer;
        }

        // Auto-elevate glow layers when inside popup context
        switch (requestedLayer)
        {
            case Layer::WidgetGlow:
                return Layer::PopupGlow;
            case Layer::Widget:
                return Layer::Popup;
            default:
                return requestedLayer;
        }
    }
}
