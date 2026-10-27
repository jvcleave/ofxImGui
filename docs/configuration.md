# Configuration reference

All options can be set in an optional **`ofximguiconfig.h`** placed anywhere on your project's include path (C++17 `__has_include` picks it up automatically), or via project compiler flags / `config.make`.

---

## Backend & renderer selection

These are usually set automatically from OF's own platform defines. Override only if you have a non-standard setup.

| Define | Effect |
|--------|--------|
| `OFXIMGUI_FORCE_OF_BACKEND` | Use the OpenFrameworks event backend instead of GLFW even on desktop |
| `OFXIMGUI_BACKEND_GLFW` | GLFW backend selected (auto on desktop / Emscripten) |
| `OFXIMGUI_BACKEND_OPENFRAMEWORKS` | OF event backend selected (auto on mobile / fallback) |
| `OFXIMGUI_RENDERER_GLSL` | OpenGL 2/3 renderer (auto on desktop) |
| `OFXIMGUI_RENDERER_GLES` | GLES renderer (auto on RPi / iOS / Android / Emscripten) |

---

## GLFW multi-context / multi-window

Relevant when using multiple `ofAppBaseWindow` instances or ImGui viewports (pop-out windows).

| Define | Default | Effect |
|--------|---------|--------|
| `OFXIMGUI_GLFW_EVENTS_REPLACE_OF_CALLBACKS` | `1` | Replace OF's raw GLFW callbacks with ofxImGui's own (enables multi-context chaining). Set to `0` to use OF event listeners instead (simpler, no multi-context). |
| `OFXIMGUI_GLFW_FIX_MULTICONTEXT_PRIMARY_VP` | `0` | Full multi-context support for the primary (main) viewport. Needed for independent contexts per window. |
| `OFXIMGUI_GLFW_FIX_MULTICONTEXT_SECONDARY_VP` | `1` | Multi-context support for secondary (popped-out) viewports. |

---

## Debug

| Define | Effect |
|--------|--------|
| `OFXIMGUI_DEBUG` | Verbose `ofLog` output, `drawOfxImGuiDebugWindow()`, keypress trace. |

Call `gui.drawOfxImGuiDebugWindow()` inside a frame to open the debug panel (only compiled when `OFXIMGUI_DEBUG` is defined).

---

## Test engine integration

| Define | Where to set | Effect |
|--------|-------------|--------|
| `OFX_IMGUI_ENABLE_TEST_ENGINE` | `config.make` / project preprocessor | Activates the Dear ImGui Test Engine hooks in **every** TU that compiles ImGui. Requires the `ofxImGuiTestSuite` addon and its `imgui_test_engine` submodule. |

The guarded block in `imconfig.h` also exposes two sub-options (set before that block is reached):

| Define | Default | Effect |
|--------|---------|--------|
| `IMGUI_TEST_ENGINE_ENABLE_COROUTINE_STDTHREAD_IMPL` | `1` | Use `std::thread`-based coroutines (no platform coroutine API needed). |
| `IMGUI_TEST_ENGINE_ENABLE_CAPTURE` | `0` | Enable built-in screenshot capture (requires ffmpeg; off by default). |

---

## `imconfig.h` — OF type interop

These are compiled in unconditionally and are not user-overridable without replacing `imconfig.h`:

- **`IM_VEC2_CLASS_EXTRA` / `IM_VEC4_CLASS_EXTRA`** — implicit conversions between `ImVec2`/`ImVec4` and `ofVec2f`/`ofVec3f`/`ofVec4f`, `ofColor`/`ofFloatColor`, and `glm::vec*` (OF ≥ 0.10).
- **`#define ImDrawIdx ofIndexType`** — matches OF's index buffer type.
- **`IMGUI_DISABLE_OBSOLETE_KEYIO`** — enforces the modern `io.AddKeyEvent()` API.
- **`IMGUI_DEFINE_MATH_OPERATORS`** — enables `ImVec2` arithmetic operators.
- **`IMGUI_IMPL_OPENGL_LOADER_CUSTOM`** — suppresses ImGui's built-in loader; OF handles OpenGL loading.

---

## `addon_config.mk` flags

| Flag | Platform | Effect |
|------|----------|--------|
| `-DofxAddons_ENABLE_IMGUI` | all | Signals to other addons (e.g. ofxKit) that ofxImGui is present. |
| `-DIMGUI_IMPL_OPENGL_ES2` | armv6l, armv7l, iOS | Selects the GLES 2 renderer backend. |
