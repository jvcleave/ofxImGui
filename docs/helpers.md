# OF-specific helpers

## ImHelpers — `ofParameter` integration

Include `ImHelpers.h` to bridge `ofParameter` and `ofParameterGroup` with ImGui widgets.

### Parameters

```cpp
#include "ImHelpers.h"

ofParameter<float>  speed { "Speed", 1.f, 0.f, 10.f };
ofParameter<int>    count { "Count", 5, 0, 20 };
ofParameter<bool>   active{ "Active", false };
ofParameter<ofColor> col  { "Color", ofColor::white };
ofParameterGroup    group { "Settings", speed, count, active, col };

// In draw():
gui.begin();
ofxImGui::AddParameter(speed);   // SliderFloat
ofxImGui::AddParameter(count);   // SliderInt
ofxImGui::AddParameter(active);  // Checkbox
ofxImGui::AddParameter(col);     // ColorEdit4
ofxImGui::AddGroup(group);       // All parameters as a collapsible tree
gui.end();
```

Supported types: `float`, `int`, `bool`, `ofColor`, `ofFloatColor`, `ofVec2f`–`ofVec4f`, `glm::vec2`–`vec4`, `glm::ivec2`–`ivec4`, `std::string`.

### Window helpers

```cpp
// Window bound to a bool parameter (tracks open state):
ofParameter<bool> showPanel{ "Panel", true };

if (ofxImGui::BeginWindow(showPanel)) {
    // ... widgets ...
    ofxImGui::EndWindow();
}
```

### Other controls

```cpp
ofxImGui::AddCombo(myParam, {"Option A", "Option B"});
ofxImGui::AddRadio(myParam, {"Low", "Med", "High"}, /*columns=*/3);
ofxImGui::AddStepper(myIntParam);           // +/- buttons
ofxImGui::AddSlider(myParam, "%.2f", false); // optional log scale
ofxImGui::AddRange(minParam, maxParam);      // dual-handle range
```

### Textures

```cpp
// Display an ofTexture or any ofBaseHasTexture inside ImGui:
ofxImGui::AddImage(myFbo, {320, 240});

// Get a raw ImTextureID for use with ImGui::Image() directly:
ImTextureID tid = ofxImGui::GetImTextureID(myTexture);
ImGui::Image(tid, {320, 240});
```

---

## ImCanvas — drawing inside an ImGui child

`ofxImGui::Canvas` opens a clipped ImGui child region; `ofxImGui::DrawList` wraps `ImDrawList` with OF-style color/fill/line-width state.

```cpp
#include "ImCanvas.h"

ofxImGui::Canvas canvas;

// In draw(), inside a gui.begin()/end() block:
if (canvas.begin("MyCanvas", {400, 300})) {
    auto dl = canvas.getDrawList();
    dl.setColor(ofColor::red);
    dl.drawCircle({200, 150}, 50);
    dl.setColor(ofColor(0, 128, 255));
    dl.drawLine({0, 0}, {400, 300});
    canvas.end();
}

// Mouse in canvas-local coordinates:
if (canvas.isHovered()) {
    glm::vec2 pos = canvas.getMousePos();
}
```

`DrawList` also accepts `ofPath` directly:

```cpp
dl.drawPath(myPath);
dl.drawPath(myPath, translate, scale);  // editor pan/zoom
```

---

## ImViewport — world-space picking & dragging

`ofxImGui::Viewport` maps between screen and world space so you can build editor-style handles that maintain a constant screen-pixel hit radius regardless of camera zoom.

```cpp
#include "ImViewport.h"

ofxImGui::Viewport vp;
ofCamera cam;

// In draw(), after gui.begin():
vp.update(cam);   // or vp.update() for screen-space only

// Hit-test a world point with a 8px screen radius:
if (vp.isNear(myPoint, 8.f)) {
    if (vp.isClicked()) vp.beginDrag("point0");
}
if (vp.isDragging("point0")) {
    myPoint += vp.getWorldMouseDelta();
    if (vp.isReleased()) vp.endDrag();
}

// Draw handles after gui.end() so they appear on top:
gui.end();
// ... draw handle circles at worldToScreen(myPoint) ...
```

---

## GuiEventHelper — block OF input when ImGui is active

Registers listeners **before** your app that return `true` to consume mouse/keyboard events when ImGui's `WantCaptureMouse` / `WantCaptureKeyboard` flags are set.

```cpp
#include "GuiEventHelper.h"

ofxImGui::GuiEventHelper eventHelper;

void ofApp::setup() {
    gui.setup();
    eventHelper.setup();  // start filtering
}
// No teardown needed — destructor unregisters automatically.
```

Without this, clicks on ImGui windows also trigger your OF mouse handlers.

---

## LoggerChannel — route `ofLog` into ImGui

```cpp
#include "ofxImGuiLoggerChannel.h"

// Redirect all OF logging to ImGui's text buffer:
ofSetLoggerChannel(std::make_shared<ofxImGui::LoggerChannel>());

// In draw(), inside a frame:
ImGui::InputTextMultiline("##log",
    ofxImGui::LoggerChannel::getBuffer().Buf.Data,
    ofxImGui::LoggerChannel::getBuffer().Buf.Size,
    {-1, 200},
    ImGuiInputTextFlags_ReadOnly);
```
