# Icons

## Default Icons

EFIGUI provides default text icons that work without any font setup:

```cpp
EFIGUI::Icons::Close       // "X"
EFIGUI::Icons::Minimize    // "-"
EFIGUI::Icons::Home        // "H"
EFIGUI::Icons::Cog         // "*"
// ... etc
```

---

## Customizing Icons

```cpp
// Use any text
EFIGUI::Icons::Close = "[X]";
EFIGUI::Icons::Check = "OK";

// Use Font Awesome (requires loading the font)
EFIGUI::Icons::Close = "\xef\x80\x8d";  // FA xmark
EFIGUI::Icons::Home  = "\xef\x80\x95";  // FA home
```

---

## Font Awesome Setup

```cpp
// After adding your main font
ImGuiIO& io = ImGui::GetIO();
ImFontConfig config;
config.MergeMode = true;
static const ImWchar iconRanges[] = { 0xf000, 0xf8ff, 0 };
io.Fonts->AddFontFromFileTTF("fa-solid-900.ttf", 16.0f, &config, iconRanges);

// Then set EFIGUI icons
EFIGUI::Icons::Close = "\xef\x80\x8d";  // xmark
EFIGUI::Icons::Home  = "\xef\x80\x95";  // home
EFIGUI::Icons::Cog   = "\xef\x80\x93";  // cog
```

---

## Available Icon Slots

- **Window controls:** `Close`, `Minimize`, `Collapse`, `Expand`
- **Navigation:** `ChevronLeft`, `ChevronRight`, `Bars`, `Home`, `Cog`, `User`
- **Common UI:** `Check`, `Warning`, `Info`, `Error`, `Eye`, `Shield`, `Bolt`

---

## Icon Helper Function

Convert a Unicode codepoint to UTF-8 string at runtime:

```cpp
// Usage: ImGui::Text("%s Home", EFIGUI::Icon(0xf015));
const char* EFIGUI::Icon(uint32_t codepoint);
```

---

## Using Embedded Font Data

If you have Font Awesome data embedded in your binary:

```cpp
// Load embedded Font Awesome icons
const void* fontData = GetMyFontAwesomeData();
size_t fontDataSize = GetMyFontAwesomeDataSize();

EFIGUI::LoadFontAwesomeIcons(fontData, fontDataSize, 16.0f);
```
