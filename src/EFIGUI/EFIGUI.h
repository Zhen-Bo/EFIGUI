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
#include "Styles/TabBarStyle.h"
#include "Styles/TreeNodeStyle.h"
#include "Styles/TableStyle.h"
#include "Styles/ListBoxStyle.h"
#include "Styles/MenuStyle.h"
#include "Styles/ColorEditStyle.h"
#include "Styles/DragStyle.h"
#include "Styles/VSliderStyle.h"
#include "Styles/PlotStyle.h"
#include "Styles/VectorInputStyle.h"

// Themes
#include "Themes/CyberpunkTheme.h"

// New v0.5.0 Components
#include "Components/Checkbox.h"
#include "Components/Radio.h"
#include "Components/Selectable.h"
#include "Components/Popup.h"
// Phase 2 Advanced Components
#include "Components/TabBar.h"
#include "Components/TreeNode.h"
#include "Components/Table.h"
#include "Components/ListBox.h"
#include "Components/Menu.h"
// Phase 3 Specialized Widgets
#include "Components/ColorEdit.h"
#include "Components/Drag.h"
#include "Components/VSlider.h"
#include "Components/Plot.h"
#include "Components/VectorInput.h"

// Legacy support (deprecated in v0.5.0)
#include "Theme.h"
#include "Components.h"
