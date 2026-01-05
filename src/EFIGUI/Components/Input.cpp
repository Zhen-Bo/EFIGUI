// =============================================
// EFIGUI Components - Input
// Modern text input fields with glow effects
// =============================================

#include "Internal.h"

namespace EFIGUI
{
    // =============================================
    // Input
    // =============================================

    bool ModernInputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::ToVec4(Theme::ButtonDefault()));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Theme::ToVec4(Theme::ButtonHover()));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Theme::ToVec4(Theme::ButtonHover()));
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::ToVec4(Theme::BorderDefault()));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Theme::FrameRounding());

        bool result = ImGui::InputText(label, buf, buf_size, flags);

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);

        return result;
    }

    bool ModernInputTextMultiline(const char* label, char* buf, size_t buf_size, ImVec2 size, ImGuiInputTextFlags flags)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::ToVec4(Theme::ButtonDefault()));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Theme::ToVec4(Theme::ButtonHover()));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Theme::ToVec4(Theme::ButtonHover()));

        bool result = ImGui::InputTextMultiline(label, buf, buf_size, size, flags);

        ImGui::PopStyleColor(3);

        return result;
    }

    // Simple callback for std::string auto-resize only
    static int StringInputTextCallback(ImGuiInputTextCallbackData* data)
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            std::string* str = (std::string*)data->UserData;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);
            data->Buf = (char*)str->c_str();
        }
        return 0;
    }

    bool ModernInputText(const char* label, std::string* str, ImGuiInputTextFlags flags)
    {
        if (str->capacity() < 256)
            str->reserve(256);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::ToVec4(Theme::ButtonDefault()));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Theme::ToVec4(Theme::ButtonHover()));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Theme::ToVec4(Theme::ButtonHover()));
        ImGui::PushStyleColor(ImGuiCol_Border, Theme::ToVec4(Theme::BorderDefault()));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Theme::FrameRounding());

        flags |= ImGuiInputTextFlags_CallbackResize;
        bool result = ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, StringInputTextCallback, (void*)str);

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(4);

        return result;
    }

    bool ModernInputTextMultiline(const char* label, std::string* str, ImVec2 size, ImGuiInputTextFlags flags)
    {
        if (str->capacity() < 256)
            str->reserve(256);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, Theme::ToVec4(Theme::ButtonDefault()));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, Theme::ToVec4(Theme::ButtonHover()));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, Theme::ToVec4(Theme::ButtonHover()));

        flags |= ImGuiInputTextFlags_CallbackResize;
        bool result = ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, StringInputTextCallback, (void*)str);

        ImGui::PopStyleColor(3);

        return result;
    }
}
