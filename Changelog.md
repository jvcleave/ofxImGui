
# Changelog

## ofxImGui 1.82

This is a major update for ofxImGui. Prior to this version, DearImGui was bound to openFrameworks using a "custom backend code".
DearImGui has grown considerably over time, but some of the new (optional but nice) features require an advanced backend code, which is now provided by ImGui. For these features, DearImGui now needs to be bound to the OS' windowing framework (GLFW). Luckily this is the same as most OpenFrameworks projects, and other popular windowing frameworks are also covered in case OpenFrameworks moves away from GLFW.

### Breaking changes
- **GLFW** : If your project uses a windowing system other then GLFW, this newer ofxImGui will not work correctly. This is mainly the case for Emscriptem projects, iOS and some Rpi environments. Consider using an [older ofxImGui release](), or try [this untested commit](./commit/05ab131) together with `OFXIMGUI_ENABLE_OF_BINDINGS` if you need ImGui 1.79 features.
- **Autodraw** : This setting was already around but worked differently, which lead to confusions. It used to call `gui.draw()` when calling `gui.end()`. Now, when autodraw is enabled, ofxImGui uses a callback on `ofEvents::afterDraw()` to render the gui, drawing the gui always on top of your ofApp. This might affect the render order rendered in your pipeline.

As an upgrade assistant, you might want to enable `OFXIMGUI_DEBUG` while transitioning, it will provide some general warnings on mistakes.
For more exhaustive information, refer to [Daandelange/ofxImGui#1](https://github.com/Daandelange/ofxImGui/issues/1) and [jvcleave/ofxImGui#107](https://github.com/jvcleave/ofxImGui/issues/107).

### New Features
- Load custom fonts
- Set `ImGuiConfigFlags` when setting up ofxImGui. _There were some workarounds for this, but not it's possible natively._
- Share ofxImGui instances within the same ofApp. _By enabling a shared mode, any ofxImGui instance will act as a singleton so multiple instances can seamlessly let eachother know when to render.
- Multi-window support **beta**. _Any feedback is appreciated_.
- Helpers :
  - ...
- Main features introduced by the DearImGui update:
  - Navigation support : Control the GUI with a gamepad or a keyboard !
  - Viewports : The Gui can seamlessly move out of your ofApp !
  - Docking : Awesome layout engine to make your own workspaces !
  - Tables : Display tables !
