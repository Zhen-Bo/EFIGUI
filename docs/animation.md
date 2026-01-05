# Animation System

## What's New in v0.3.1

- **SmoothFloat / SmoothVec2**: New helpers for smooth value transitions with automatic lerping
- **Per-Instance Config integration**: Animation helpers work seamlessly with the new Config structs

---

## Smooth Value Transitions (v0.3.1+)

For dynamic UI scaling and smooth property changes, use `SmoothFloat` and `SmoothVec2`:

```cpp
// Declare persistent smooth value (static or member variable)
static EFIGUI::Animation::SmoothFloat buttonScale;

// Set target when conditions change
if (isImportant) {
    buttonScale.SetTarget(1.5f);  // Scale up to 150%
} else {
    buttonScale.SetTarget(1.0f);  // Normal size
}

// Use in your render loop - automatically lerps toward target
EFIGUI::GlowButtonConfig config;
config.size = ImVec2(100 * buttonScale.Update(), 36);
EFIGUI::GlowButton("Dynamic Button", config);
```

### SmoothFloat API

| Method | Description |
|--------|-------------|
| `SetTarget(float)` | Set the target value to lerp toward |
| `Update(float speed = 8.0f)` | Lerp toward target, returns current value |
| `SetImmediate(float)` | Set both current and target instantly (no animation) |
| `IsComplete(float epsilon = 0.001f)` | Returns true when current ≈ target |
| `current` | Current interpolated value |
| `target` | Target value |

### SmoothVec2 API

Same API but for `ImVec2` values:

```cpp
static EFIGUI::Animation::SmoothVec2 panelSize;

panelSize.SetTarget(ImVec2(400, 300));
ImVec2 currentSize = panelSize.Update(12.0f);  // Faster lerp speed

EFIGUI::BeginGlassPanel("panel", currentSize);
// ...
EFIGUI::EndGlassPanel();
```

### Dynamic Slider-Linked Buttons Example

```cpp
// Buttons scale based on slider position
static EFIGUI::Animation::SmoothFloat leftScale, rightScale;
static float balance = 0.5f;

// Calculate target scales (slider at 0 = left big, at 1 = right big)
leftScale.SetTarget(1.0f - balance * 0.4f);   // 0.6 ~ 1.0
rightScale.SetTarget(0.6f + balance * 0.4f);  // 0.6 ~ 1.0

// Render with smooth scaling
EFIGUI::GlowButtonConfig leftCfg, rightCfg;
leftCfg.size = ImVec2(80 * leftScale.Update(), 32);
rightCfg.size = ImVec2(80 * rightScale.Update(), 32);

EFIGUI::GlowButton("Left", leftCfg);
ImGui::SameLine();
EFIGUI::ModernSliderFloat("Balance", &balance, 0.0f, 1.0f);
ImGui::SameLine();
EFIGUI::GlowButton("Right", rightCfg);
```

---

## Widget Animation State

```cpp
// Get animation state for any widget
ImGuiID id = ImGui::GetID("myWidget");
EFIGUI::Animation::WidgetState& state = EFIGUI::Animation::GetState(id);

// Update based on widget interaction
EFIGUI::Animation::UpdateWidgetState(state, isHovered, isActive, isSelected);

// Use the animated values (0.0 to 1.0)
float hoverProgress = state.hoverAnim;
float activeProgress = state.activeAnim;
float selectedProgress = state.selectedAnim;
```

---

## WidgetState Properties

| Property | Description |
|----------|-------------|
| `hoverAnim` | 0 = not hovered, 1 = fully hovered |
| `activeAnim` | 0 = not active, 1 = fully active |
| `selectedAnim` | 0 = not selected, 1 = fully selected |
| `glowPhase` | For pulsing glow effect |
| `clickAnim` | For click ripple effect |
| `slideAnim` | For toggle slide animation |
| `lastUpdateFrame` | Frame number when last updated (for pruning) |

---

## Periodic Effects

```cpp
// Pulsing effect (0 to 1 to 0...)
float pulse = EFIGUI::Animation::Pulse(2.0f);  // 2 Hz frequency

// Sweeping effect (0 to 1, then resets)
float sweep = EFIGUI::Animation::Sweep(0.8f);  // 0.8 seconds per sweep

// Smooth breathing (sine wave)
float breathe = EFIGUI::Animation::Breathe(1.0f);  // 1 Hz frequency
```

---

## Color Interpolation

```cpp
// Lerp between two colors based on animation
ImU32 color = EFIGUI::Animation::LerpColorU32(
    Theme::TextSecondary(),  // Start color
    Theme::AccentCyan(),     // End color
    state.hoverAnim          // 0.0 to 1.0
);
```

---

## State Management

EFIGUI automatically manages animation state cleanup:

- Unused widget states are pruned every ~1 second (60 frames at 60fps)
- States that haven't been updated for 60+ frames are automatically removed
- This prevents memory growth from widgets that are no longer rendered

```cpp
// Manual cleanup (usually not needed)
EFIGUI::Animation::ClearStates();  // Clear all states

// Prune stale states (called automatically in BeginFrame)
EFIGUI::Animation::PruneStaleStates(60);  // Remove states idle for 60+ frames
```
