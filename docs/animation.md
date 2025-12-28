# Animation System

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
