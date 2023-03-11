# ofxImGui

ofxAddon that allows you to use [ImGui](https://github.com/ocornut/imgui) in [openFrameworks](https://github.com/openframeworks/openFrameworks).

### Immediate Mode Gui
Unlike most C++ gui libraries, ImGui uses the [immediate mode](https://en.wikipedia.org/wiki/Immediate_mode_(computer_graphics)) paradigm rather then being OOP. In other words, every frame you rebuild the whole GUI, executing only the necessary parts, and ImGui is surprisingly fast at that ! This makes the Gui closer to the OpenGL api as to the inner oF logic (`update()`, `draw()`, `begin()`, `end()`, `push()`, `pop()`, etc.).  
_(Note: Internally, ImGui is not "ramming" on the CPU as most immediate-mode GUIs do)._

![Screenshot](images/ofxImGui.gif)

- - - -

### Supported Platforms
ofxImGui should run on the [latest openFrameworks release and it's OS/IDE requirements](https://openframeworks.cc/download/). These are typically:

 - Mac OSX, Xcode
 - Windows 10, Visual Studio
 - Raspberry Pi
 - Linux Desktop

 Currently, **ofxImGui has been refactored**, both implementing a custom oF backend (similar to Jvcleave's original implementation) and a "native imgui backend" (similar to one of the many imgui forks). OsX, Windows and Linux have been tested, but some more specific platforms remain to be tested. They should be compatible but the GL pipeline setup and cpp includes remain to be tested; if you do so, please report back your findings.

*Notes on Rpi support: Some combinations of Rpi and oF versions won't provide all GLSL versions. It's recommended to use the full KMS driver rather then the Legacy Broadcom one (very low FPS), but they both work. Tested with Raspbian Stretch. Also, if you start your application with a minimal desktop environment (using `startx ./bin/ofApp`), the imgui viewport features do not work correctly.* 

#### GLSL Support Table

| OS | OpenGL 2.x | OpenGL 3.x  | OpenGL 4.x | GL ES 1.0 | GL ES 2 | GL ES 3 |
|---|---|---|---|---|---|---|
| Windows | Yes | Yes | Yes | Unknown | Unknown | Unknown |
| Mac OsX | Yes | Yes | Yes |  *Unavailable* | *Unavailable* | *Unavailable* |
| Linux | Yes | Yes | Yes | Yes | Yes |
| Rpi3 | Should | Unknown | Unknown | Yes | Yes | Yes |
| Rpi4 | Unknown | Should | Should | Should | Should | Should |
| iOS | *Unavailable* | *Unavailable* | *Unavailable* | Should | Should | Should |

*Note: This support table does not take into account software emulated support for graphics APIs.*  
*Note: GL ES 1 (the fixed pipeline ES shading language) is not natively supported by the native DearImGui backend, but it works with [some dirty hacks](src/gles1CompatibilityHacks.h).*

#### oF & ImGui Support Table
New ImGui versions bring changes and new API features, sometimes depreciations.  
Versions are tagged in the git repo.

| ofxImGui version  | ImGui version | Supported oF version |
|------------------:|---------------|----------------------|
| ofxImGui 1.89.2   | 1.89.2*       | 0.11 -> 0.11.2 |
| ofxImGui 1.82     | 1.82*         | 0.11 -> 0.11.2 |
| ofxImGui 1.79     | 1.79*         | 0.11.1 |
| ofxImGui 1.75     | 1.75          | 0.11.x |
| ofxImGui 1.62     | 1.62          | 0.10.x | 
| ofxImGui 1.50     | 1.50 WIP      | 0.10.x |
| ofxImGui 1.49     | 1.49          | 0.10.x |
| ofxImGui 1.47     | 1.47          | 0.10.x |
| ofxImGui 0.90     | 1.45 WIP      | 0.9.x  |

__*__ Uses the native ImGui backend, offering pop-out-windows (viewports), docking, gamepad control, and more.


#### ImGui Feature Support Table Per Backend

By default, ofxImGui uses the GLFW engine when possible, which gives the best user experience, falling back to the OpenFrameworks backend which is supported on most ofApp setups but provides slightly less ImGui functionality. You can force ofxImGui to use the OpenFrameworks one by defining `OFXIMGUI_FORCE_OF_BACKEND`.

One tiny disadvantage of the Glfw backend is that multiwindow-together-with-viewports support might break on ImGui updates. There are automatic update scripts, but they might not work in the future. Without the custom modifications, it will work fine but you'll have to choose between multiwindow or viewports, knowing that you can configure ImGui to never merge viewport windows with the host window.

| ofxImGui backend      | Viewports | Custom mouse cursors | Docking | Gamepads| GL SL | GL ES | Vulkan | Multiple ofxAppWindows | Automatic Contexts |
|-----------------------|-----------|----------------------|---------|---------|-------|-------|--------|------------------------|--------------------|
|  EngineGLFW           | [x]       | [x]*                 | [x]     | [x]     | [x]   | [x]   | Maybe  | [x]^                   | [x]!               |
|  EngineOpenFrameworks | [ ]       | [ ]                  | [x]     | [ ]     | [x]   | [x]   | Maybe  | [x]^                   | [x]!               |

- __*__ Partial support, by default not all cursors are enabled, see [Updating GLFW](./Developpers.md#Improve-ofxImGui-s-backend-bindings).
- __^__ One Context per ofAppWindow (isolated mode): No inter-communication between the GUIs (cross-docking won't work).  
  (_EngineGLFW backend is slightly modified for supporting multiple glfw contexts_)  
  Hopefully DearImGui will introduce something to handle «[multiple host viewports](https://github.com/ocornut/imgui/issues/3012)».  
  Please note that using **ofxImGui in multiwindow OpenFrameworks applications** works, but keep in mind that this might break with future ImGui updates.
- __!__ A singleton class ensures ensures the creation of the ImGui Context within openFrameworks. If multiple source files setup ofxImGui, the first sets up normally (as a master), the following ones as slaves, both still being able to draw to the same gui context. This can be useful when using ofxImGui from multiple ofxAddons.

- - - -

## Usage

### Install
````bash
cd /path/to/of/addons && git clone https://github.com/Daandelange/ofxImGui.git
````

### Optional
Configure oF (tested with 0.11.0 and 0.11.2) to use GLFW 3.4 and ImGui will have an even more polished interface. See [Developpers.md](./Developpers.md#Improve-ofxImGui-s-backend-bindings).  
This step is also recommended for RPIs where GLFW is v3.2, which doesn't provide gamepad support.  

### Compilation flags
DearImGui needs to know your GL Context. ofxImGui tries to match your project's settings.  
If your projects needs to force a specific GL configuration, you can set some native imgui compilation flags to match your project settings :
 - `IMGUI_IMPL_OPENGL_ES2` --> Use GLES2 (or GL ES 1.1 with some hacks).
 - `IMGUI_IMPL_OPENGL_ES3` --> Use GLES3.
 - `[none of the previous ones]` --> Use OpenGL.

 By default, ofxImGui uses the improved GLFW backend when using `ofAppGLFWWindow`s. You may force to use the openframeworks backend by defining :
 - `OFXIMGUI_FORCE_OF_BACKEND`

You may also over-ride some automatic macro defines (not recommended, there are drawbacks, but it might solve some very specific use cases):
 - `IMGUI_GLFW_INJECT_MULTICONTEXT_SUPPORT=0` to disable imgui_impl_glfw changes to support multiple context. Disables using ofxImGui within multiple `ofAppBaseWindow`s.
 - `INTERCEPT_GLFW_CALLBACKS=0` to use an alternative method to bind imgui to glfw events. If `0`, ofxImGui doesn't add multi-context event routing, disabling multi-window-ofApp support; event propagation: `GLFW -> ImGui -> OpenFrameworks`. By default (`=1`), ofxImGui binds to GLFW, allowing to route events to the correct context instances; event propagation: `GLFW -> ofxImGui -> (ImGui + OpenFrameworks)`.

To have an insight on how your ofxImGui interfaces ImGui, you can call `gui.drawOfxImGuiDebugWindow();` together with `OFXIMGUI_DEBUG`. It will provide you an explanation of your configuration and it also provides some suggestions for gradually improving your configuration (to get the most out of OF+ImGui).

### Setup
Calling `mygui.setup()`, you can pass a few arguments to setup ofxImGui to your needs.  
ofxImGui implements DearImGui in such a way that each oF window gets its own imgui context, seamlessly shared between any oF window context. You can also load multiple instances of it in the same ofApp (to use the addon within multiple addons). This feature (shared mode) is automatically enabled when a second `ofxImGui::Gui` instance is created within the same application's platform window context. See example-sharedContext for more info.  
_Note: Only the fist call to `gui.setup()` has full control over the settings (master); the next ones setup as slaves._  
_Note: Any call to `setup()` has to be done from a valid ofWindow, `ofGetWindowPtr()` is used internally to associate the gui context to that exact ofWindow._  

#### Return value
The setup function returns an `ofxImGui::SetupState` indicating if the setup failed succeeded as a slave or master. 
`if( gui->setup(...) & ofxImGui::SetupState::Success ) // I'm either a slave or a master`   

#### Arguments
- **theme** : `nullptr` (use default theme, default), `BaseTheme*` (pointer to you theme instance))
- **autoDraw** : `true` (automatic, sets up a listener on `ofApp::afterDraw()`, default) / `false` (manual, allows more precise control over the oF rendering pipeline, you have to call `myGui.draw()` )
- **customFlags** : `ImGuiConfigFlags` ( set custom ImGui config flags, default: `ImGuiConfigFlags_None`)
- **restoreGuiState** : `true` (enabled) or `false` (disabled, default).  
Helper for enabling ImGui's layout saving/restoring feature. Creates `imgui.ini` next to your app binary to save some GUI parameters.
- **showImGuiMouseCursor** : `true` (use the imgui mouse cursor) or `false` (use default system mouse cursor, default).

Tip: While interfacing your ofApp with ofxImGui, a good practise is to enable `OFXIMGUI_DEBUG` together with `ofSetLogLevel(OF_LOG_VERBOSE)`, it provides some general warnings on mistakes and logs some important setup steps.

#### Advanced setup : ImGui config flags & ImGui::GetIO()

ofxImGui provides a simple way to interface imgui, but it's a huge library providing lots of different options to suit your precise needs.  
Most of these advanced options are explained in the `imgui_demo.cpp` source code. Also checkout `example-dockingandviewports` and `example-advanced`.  
Some options are: Docking, viewports, custom fonts, and other configuration flags.

- - - -

## Examples
There are several example projects, covering from the simplest use case to more advanced ones : [Examples.md](./Examples.md).

- - - -

## Developper info
Useful dev info and how to get familiar with DearImGui : [Developper.md](./Developpers.md).

- - - -

## Updating ofxImGui
- `cd /path/to/ofxImGui && git pull && git submodule update`
- (*optional but recommended*) After updating: Add `#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS` in your `imconfig.h` file to make sure you are not using to-be-obsoleted symbols. Update any if needed.

## Alternatives and similar ofxAddons
- [moebiussurfing/ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui/): An extension of this addon which facilitates advanced integration with ofApps while also making the UI more user-friendly. It embeds a presets system, and embeds many 3rd party ui widgets.
