# Getting started

## 1. Add to your project

In your app's `addons.make`:
```
ofxImGui
```

## 2. Minimal setup

```cpp
// ofApp.h
#include "ofxImGui.h"
ofxImGui::Gui gui;

// ofApp.cpp
void ofApp::setup() {
    gui.setup();  // autoDraw on by default
}

void ofApp::draw() {
    gui.begin();
    ImGui::Begin("My window");
    ImGui::Text("Hello, OF!");
    ImGui::End();
    gui.end();
    // GUI renders automatically after ofApp::draw() returns
}
```

## 3. Manual draw

Pass `autoDraw = false` when you need to control exactly when ImGui renders relative to your own draw calls — for example to draw editing handles on top of ImGui windows:

```cpp
gui.setup(nullptr, false);

void ofApp::draw() {
    drawScene();             // rendered below ImGui
    gui.begin();
    // ... ImGui calls ...
    gui.end();
    gui.draw();              // render ImGui here
    drawHandles();           // rendered above ImGui
}
```

> **Note:** In autoDraw mode ImGui renders at `OF_EVENT_ORDER_AFTER_APP`, i.e. *after* `ofApp::draw()` returns. Anything drawn in `draw()` ends up below ImGui. Use manual draw whenever your OF scene content needs to appear on top of ImGui windows.

## 4. Frame lifecycle

| Call | What it does |
|------|-------------|
| `gui.begin()` | Sets context, starts a new ImGui frame (`NewFrame`) |
| `gui.end()` | Ends the frame (`EndFrame`) |
| `gui.draw()` | Renders — only needed when `autoDraw = false` |
| `gui.autoDraw(args)` | Internal: called by the window draw event when autoDraw is on |

`beforeDraw` and `afterDraw` events fire around the actual render step.

## 5. Shared / multi-window contexts

Create a second `Gui` on the same window to join the existing context as a **slave** (shares fonts, state). Create it on a different `ofAppBaseWindow` to get an independent context. Check `isMaster()` / `isInSharedMode()` / `SetupState` return value if you need to branch.

```cpp
// Multi-window: each window gets its own context
gui1.setup(window1);
gui2.setup(window2);

// Shared on same window: gui2 is a slave
gui1.setup(window1);  // master
gui2.setup(window1);  // slave — joins gui1's context
```

## 6. Loading fonts

```cpp
// Before or after setup(), before the first frame:
gui.addFont("fonts/Roboto-Regular.ttf", 16.f);

// Set as default:
gui.addFont("fonts/Roboto-Regular.ttf", 16.f, nullptr, nullptr, /*setDefault=*/true);

// Rebuild after adding fonts if needed (no-op on ImGui >= 1.92):
gui.rebuildFontsTexture();
```

## 7. Saving / restoring layout

```cpp
// Pass restoreGuiState = true to persist window positions:
gui.setup(nullptr, true, ImGuiConfigFlags_None, /*restoreGuiState=*/true);
// Saves to ofToDataPath("imgui.ini") by default.
// Override before calling setup():
gui.iniFileName = ofToDataPath("my_layout.ini");
```

## 8. HiDPI / high-resolution displays

Content scale is detected and applied automatically via `glfwGetWindowContentScale` on the GLFW backend. `io.FontGlobalScale` and `ImGui::GetStyle()` are scaled on setup. For sharper text, load fonts at `fontSize * scale` using `addFont()` and set them as default.
