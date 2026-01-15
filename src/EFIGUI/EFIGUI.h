// src/EFIGUI/EFIGUI.h
// Unified entry point - users only need to include this file
#pragma once

// Core systems
#include "Core/EFIGUI.h"
#include "Core/Style.h"
#include "Core/Animation.h"
#include "Core/Layer.h"
#include "Core/Draw.h"

// Styles
#include "Styles/StyleTypes.h"
#include "Styles/CheckboxStyle.h"
#include "Styles/RadioStyle.h"
#include "Styles/SelectableStyle.h"
#include "Styles/PopupStyle.h"

// Themes
#include "Themes/CyberpunkTheme.h"

// New v0.5.0 Components
#include "Components/Checkbox.h"
#include "Components/Radio.h"
#include "Components/Selectable.h"
#include "Components/Popup.h"

// Legacy support (deprecated in v0.5.0)
#include "Theme.h"
#include "Components.h"
