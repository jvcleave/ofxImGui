
# Changelog

_Note: This document is focused on ofxImGui. For detailed ImGui API changes and new features, please refer to the [ImGui changelog](https://github.com/ocornut/imgui/blob/v1.91.0-docking/docs/CHANGELOG.txt)._  
_Tip: For compile-time depreciation hinting on your code, uncomment `IMGUI_DISABLE_OBSOLETE_FUNCTIONS` in `imconfig.h`. If you do this regurarly, it's quite easy to keep up with the occasional ImGui breaking API changes._ 

This document is a summary. You can find more information by looking at the respective commits. The commit messages are usually well detailed and otherwise you can always refer to the corresponding code changes. 

Git [commit tags](https://github.com/jvcleave/ofxImGui/tags) :
 - The latest commit with a specific ImGui version is tagged `imgui_X.XX.X`.
 - ofxImGui versions are tagged `X.XX.X`.

- - - -

# ofxImGui 1.93.x (develop)

- Feature: Allow non-data paths for font loading (fixes rpi paths).
- Feature: FreeType font loader for more precise control on text rendering (opt-in).
- Feature: Autoscaling GUI on HiDPI displays (GLFW only).
- Feature: New AI generated docs.
- Feature: Better game pad support in debug window and explain mappings.
- Feature: Canvas and Viewport widgets.
- Feature: ImGuiTestSuite support for automated GUI tests (undocumented yet).
- Feature: CI builds for continuous testing.
- Examples: Emscripten examples added.
- Fix: Rpi & GLES compilation.
- **Newly introduced ImGui Features** : ImDraw API improved (anti-aliasing, stroke caps, etc.)

- - - -

# ofxImGui 1.92.x (develop)

- Feature: New compiler flag to disable glfw version hacks `OFXIMGUI_GLFW_NO_VERSION_HACKS`.
- Feature: New compiler flag for ofxAddons implementing custom ofxImGui widgets: `ofxAddons_ENABLE_IMGUI`.
- Feature: Possibility to encode font-files into the application binary.
- Feature: AfterDraw and BeforeDraw event listeners, facilitating render order.
- Feature: isAutoDrawEnabled getter.
- Feature: New event helper class `GuiEventHelper` (to bind non-gui IO events).
- Feature: New ofxImGuiDebugWindow input test section.
- **Newly introduced ImGui Features** : font-atlas refactor (breaks advanced font usage, glyphs load on the fly, improves DPI scaling), GLFW multi-context support and many more.
- Improve Docs and examples.
- Improve GLFW context helper, prevent useless push/pop states.
- Fix some examples by @Moebiussurfing.
- Fix iOS platform support by [@Roymacdonald].
- Fix non-clang compilers by [@GitBruno].
- Fix some imgui_impl_glfw features not being detected on some OF versions.
- Fix engine OpenFrameworks rendering the GUI twice.
- Fix mouse-hover helpers for daandelange#17 [@moebiussurfing].
- Fix: Prepare for future upstream ImGui changes.
- Fix: Add License.md

- - - -

# ofxImGui 1.91.0 (develop)

- Feature: `isMaster()` by @Moebiussurfing.
- **Newly introduced ImGui Features** : New widgets (multi-select, box-select, item flags, links) and more.
- Improved: Warnings when using `GL_TEXTURE_RECTANGLE` instead of ` GL_TEXTURE_2D`.
- Improved: Lots of new documentation.
- Fix issue where default font was overwritten loading a first custom font.
- Fix SWIG binding compatibility by @Jonathhhan.
- Fix examples by @GitBruno.

- - - -

# ofxImGui 1.82 (develop)

This is a major update for ofxImGui. Prior to this version, DearImGui was bound to openFrameworks using a "custom backend code".
DearImGui has grown considerably over time, but some of the new (optional but nice) features require an advanced backend code, which is now provided by ImGui. For these features to be enabled, DearImGui now needs to be bound to the OS' windowing framework (GLFW). Luckily this is currently the same as most OpenFrameworks projects, and other popular windowing frameworks are also covered in case OpenFrameworks moves away from GLFW.

### Breaking changes
- **GLFW** : If your project uses a windowing system other then GLFW, this newer ofxImGui will not work correctly. This is mainly the case for Emscriptem projects, iOS and some Rpi environments. Consider using an [older ofxImGui release](https://github.com/jvcleave/ofxImGui/tree/330e1425a88de7babd53ceb2fb93f8109b61724c), or try [this untested commit](https://github.com/jvcleave/ofxImGui/tree/05ab1311511523c63c6f22d38ee015942b9ea557) together with `OFXIMGUI_ENABLE_OF_BINDINGS` if you need ImGui 1.79 features.
- **Autodraw** : This setting was already around but worked differently, which lead to confusions. It used to call `gui.draw()` when calling `gui.end()`. Now, when autodraw is enabled, ofxImGui uses a callback on `ofEvents::afterDraw()` to render the gui, drawing the gui always on top of your ofApp. This might affect the render order rendered in your pipeline.
- **imgui_stdlib.h/cpp** : If you were using them in your project, please remove the files, they are now included with ofxImGui.
- **ImHelpers.cpp** : If you are using them, they are not included by default via `#import "ofxImGui.h"`, you now need to include it manually (`#import "ImHelpers.h"`).
- **ImGui API changes** : Refer to the ImGui changelog.

As an upgrade assistant, you might want to enable `OFXIMGUI_DEBUG` while transitioning, it will provide some general warnings on mistakes.

For more exhaustive information on changes, refer to [Daandelange/ofxImGui#1](https://github.com/Daandelange/ofxImGui/issues/1) and [jvcleave/ofxImGui#107](https://github.com/jvcleave/ofxImGui/issues/107).

### New Features
- Load custom fonts
- Set `ImGuiConfigFlags` when setting up ofxImGui. _There were some workarounds for this, but not it's possible natively._
- Share ofxImGui instances within the same ofApp. _By enabling a shared mode, any ofxImGui instance will act as a singleton so multiple instances can seamlessly let eachother know when to render._
- Multi-window support **beta**. _Any feedback is appreciated_.
- Improved example projects.
- Helpers :
  - AddGroup() can now receive treenode flags.
  - New: ofColor and std::string parameters
- Main features introduced by the DearImGui update:
  - Navigation support : Control the GUI with a gamepad or a keyboard !
  - Viewports : The Gui can seamlessly move out of your ofApp !
  - Docking : Awesome layout engine to make your own workspaces !
  - Tables : Display tables !

For more details on how to use them, please refer to the repo's readme and the examples' code.

- - - -