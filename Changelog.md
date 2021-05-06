
# Changelog

## ofxImGui 1.82

This is a major ofxImGui update. Prior to this version, ofxImGui was bound to openFrameworks using a "custom backend code".
DearImGui has grown considerably over time, but some of the new (optional) features require an advanced backend code, which is now provided by ImGui. For this reason, ImGui now needs to be bound to the OS windowing framework (GLFW). Luckily this is the same as most OpenFrameworks projects, and other popular windowing frameworks are also covered in case OpenFrameworks moves away from GLFW.

### Breaking changes
- **GLFW** : If your project uses a windowing system other then GLFW, ofxImGui will not work correctly. This is mainly the case for Emscriptem projects, iOS and some Rpi environments. Consider using an [older ofxImGui release](), or try [this untested commit](#05ab131) together with `OFXIMGUI_ENABLE_OF_BINDINGS` if you need ImGui 1.79 features.
- **Autodraw** : This setting was used to call `gui.draw()` when calling `gui.end()`. Now, when autodraw is enabled, ofxImGui uses a callback on `ofEvents::afterDraw()` to render the gui, drawing the gui always on top of your ofApp. This might affect the render order rendered in your pipeline.

For more exhaustive information, refer to [Daandelange/ofxImGui#1](https://github.com/Daandelange/ofxImGui/issues/1) and [jvcleave/ofxImGui#107](https://github.com/jvcleave/ofxImGui/issues/107).

### New Features
- Load custom fonts
- Set `ImGuiConfigFlags` when setting up ofxImGui
- Share ofxImGui instances withuin the same ofApp
- Multi-window support
- Helpers :
  - ...
- Main features introduced by the DearImGui update:
  - Gamepad support : Control the GUI with a gamepad !
  - Viewports : The Gui can move out of your 
  - Docking : Awesome layout engine to make your own workspaces
  - Tables : Display tables
