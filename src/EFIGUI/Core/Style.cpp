// src/EFIGUI/Core/Style.cpp
#include "Style.h"

namespace EFIGUI {

bool StyleSystem::s_initialized = false;

void StyleSystem::Init() {
    Init<CyberpunkTheme>();
}

} // namespace EFIGUI
