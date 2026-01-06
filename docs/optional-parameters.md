# Understanding Optional Parameters

## Design Principle

EFIGUI uses `std::optional<T>` to distinguish between "use default" and "use specific value":

- **Omit parameter** → Uses Theme default value
- **Provide any value (including 0)** → Uses that exact value

```cpp
// Omit parameter = use Theme default cyan glow
GlowButton("OK");

// Provide color = use red glow
GlowButton("OK", ImVec2(0,0), Theme::StatusError());

// Provide 0 = fully transparent (alpha = 0)
BeginGlassPanel("panel", size, true, 0);
```

---

## How `std::optional` Works

`std::optional<T>` is a C++17 feature that represents "maybe a value, maybe nothing".

```cpp
#include <optional>

std::optional<int> maybeNumber;           // Empty (no value)
std::optional<int> definitelyNumber = 42; // Has value 42

if (maybeNumber.has_value()) { /* won't execute */ }
if (definitelyNumber.has_value()) { /* will execute */ }

int x = definitelyNumber.value();     // x = 42
int y = maybeNumber.value_or(100);    // y = 100 (fallback)
```

---

## Using Optional Parameters in EFIGUI

### Pattern 1: Don't provide the parameter (use default)

```cpp
// All these use theme defaults:
EFIGUI::GlowButton("Click");                    // Default cyan glow
EFIGUI::BeginGlassPanel("panel");               // Default background alpha
EFIGUI::StatusIndicator("Online", greenColor);  // Default dot size (8.0f)
```

### Pattern 2: Provide `std::nullopt` explicitly (same as not providing)

```cpp
// These are equivalent to Pattern 1:
EFIGUI::GlowButton("Click", ImVec2(0,0), std::nullopt);
EFIGUI::BeginGlassPanel("panel", ImVec2(0,0), true, std::nullopt);
```

### Pattern 3: Provide a specific value

```cpp
// Custom red glow
EFIGUI::GlowButton("Delete", ImVec2(0,0), IM_COL32(255, 60, 60, 255));

// 50% transparent background
EFIGUI::BeginGlassPanel("panel", ImVec2(0,0), true, 128);

// Larger status dot (12 pixels)
EFIGUI::StatusIndicator("Online", greenColor, false, 12.0f);

// Fully transparent background (alpha = 0)
EFIGUI::BeginGlassPanel("invisible", ImVec2(0,0), true, 0);
```

---

## Quick Reference: Optional Parameter Types

| Type | What it controls | Example values |
|------|-----------------|----------------|
| `std::optional<uint8_t>` | Alpha/transparency | `0` (transparent) to `255` (opaque) |
| `std::optional<ImU32>` | Colors | `IM_COL32(r, g, b, a)` or `Theme::AccentCyan()` |
| `std::optional<float>` | Sizes/dimensions | `8.0f`, `12.0f`, etc. |

---

## Real-World Examples

```cpp
// Example 1: A panel with custom transparency
EFIGUI::BeginGlassPanel("myPanel",
    ImVec2(300, 200),  // size
    true,              // show border
    180                // 70% opaque (180/255)
);
// ... content ...
EFIGUI::EndGlassPanel();

// Example 2: Navigation with custom accent color
ImU32 myAccent = IM_COL32(255, 100, 50, 255);  // Orange
if (EFIGUI::NavItem(Icons::Home, "Dashboard", isSelected,
                     Theme::NavbarWidth(), false, myAccent))
{
    // clicked
}

// Example 3: Status indicator with larger dot
EFIGUI::StatusIndicator("Server Online",
    Theme::StatusSuccess(),  // green
    true,                    // pulse animation
    12.0f                    // 12px dot (default is 8px)
);

// Example 4: Feature card with semi-transparent background
EFIGUI::FeatureCard(
    Icons::Shield,
    "God Mode",
    "Makes you invincible",
    &godModeEnabled,
    150  // Semi-transparent background
);
```
