// src/EFIGUI/Components/VectorInput.h
#pragma once
#include "imgui.h"
#include "../Styles/VectorInputStyle.h"

namespace EFIGUI {

// InputFloat variants
bool InputFloat2(const char* label, float v[2], const char* format = "%.3f",
                 ImGuiInputTextFlags flags = 0);
bool InputFloat2Ex(const char* label, float v[2], const char* format,
                   ImGuiInputTextFlags flags, const VectorInputStyle& style);

bool InputFloat3(const char* label, float v[3], const char* format = "%.3f",
                 ImGuiInputTextFlags flags = 0);
bool InputFloat3Ex(const char* label, float v[3], const char* format,
                   ImGuiInputTextFlags flags, const VectorInputStyle& style);

bool InputFloat4(const char* label, float v[4], const char* format = "%.3f",
                 ImGuiInputTextFlags flags = 0);
bool InputFloat4Ex(const char* label, float v[4], const char* format,
                   ImGuiInputTextFlags flags, const VectorInputStyle& style);

// InputInt variants
bool InputInt2(const char* label, int v[2], ImGuiInputTextFlags flags = 0);
bool InputInt2Ex(const char* label, int v[2], ImGuiInputTextFlags flags,
                 const VectorInputStyle& style);

bool InputInt3(const char* label, int v[3], ImGuiInputTextFlags flags = 0);
bool InputInt3Ex(const char* label, int v[3], ImGuiInputTextFlags flags,
                 const VectorInputStyle& style);

bool InputInt4(const char* label, int v[4], ImGuiInputTextFlags flags = 0);
bool InputInt4Ex(const char* label, int v[4], ImGuiInputTextFlags flags,
                 const VectorInputStyle& style);

} // namespace EFIGUI
