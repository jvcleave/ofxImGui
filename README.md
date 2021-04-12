# ofxImGui

ofxAddon that allows you to use [ImGui](https://github.com/ocornut/imgui) in [openFrameworks](https://github.com/openframeworks/openFrameworks).

Unlike most C++ gui libraries, ImGui uses the [immediate mode](https://en.wikipedia.org/wiki/Immediate_mode_(computer_graphics)) API pattern, rather then being OOP. In other words, every frame you rebuild the whole GUI, executing only the necessary parts, and ImGui is surprisingly fast. This makes the Gui closer to the OpenGL api as to the inner oF logic (`update()`, `draw()`, `begin()`, `end()`, `push()`, `pop()`, etc.).

![Screenshot](images/Screenshot.png)

### Compatibility
The master branch may not be tested on all platforms. See [Releases](https://github.com/daandelange/ofxImGui/releases/) for more extensively tested versions.

#### Test Platforms
ofxImGui should run on the [latest openFrameworks release and it's OS/IDE requirements](https://openframeworks.cc/download/). These are typically:

 - Mac OSX, Xcode
 - Windows 10, Visual Studio
 - Raspberry Pi
 - Linux Desktop

 Please note that using ofxImGui in multiwindow openframworks applications is not yet supported, future versions of ImGui might allow this.
 Also, the iOS and Vulkan implementations might be broken; as they've not yet been ported to the new ImGui backends.

#### GLSL Support Table

| OS | OpenGL 2.x (fixed pipeline) | OpenGL 3.x  | OpenGL 4.x | GL ES 1.0 (fixed pipeline) | GL ES 2.0 |
|---|---|---|---|---|---|
| Windows | [x] | [x] | [x] | Unknown | Unknown |
| Mac OsX | [x] | [x] | [x] |  *Unavailable* | *Unavailable* |
| Linux | [x] | [x] | [x] | [x] | [x] |
| iOS | *Unavailable* | *Unavailable* | *Unavailable* | Unknown | Unknown |
| Rpi3 | Unknown | Unknown | Unknown | [x]  (with Legacy driver and `OFXIMGUI_ENABLE_OF_BINDINGS`) | [x] KMS and Legacy driver |
| Rpi4 | Unknown | Should (with KMS driver) | Should (with KMS driver) | Should | Should |
| iOS | *Unavailable* | *Unavailable* | *Unavailable* | Should | Should |

#### oF & ImGui Support Table

| ofxImGui version | Embedded ImGui version | Supported oF version |
|----- |-------| ------ |
| 1.?? | 1.??* | 0.11.x |
| 1.75 | 1.75  | 0.11.x |
| 1.62 | 1.62  | 0.10.x | 
| 1.50 | 1.50 WIP | 0.10.x |
| 1.49 | 1.49  | 0.10.x |
| 1.47 | 1.47  | 0.10.x |
| 0.90 | 1.45 WIP | 0.9.x |

New ImGui versions bring changes and new API features, sometimes depreciations.  
__*__ Uses the native ImGui backend, offering pop-out-windows (viewports), docking, gamepad control, and more.

### Usage
In the setup, you can choose to automatically render, or choose to manually render the gui, if you need more precise control of when it's drawn to the screen.
ofxImGui implements ImGui in such a way that each oF window gets its own imgui context, seamlessly shared between any oF window context. Note: Only the fist call to `gui.setup()` has full control over the settings; the next ones setup as slaves.

#### Auto
Automatically renders the gui then you call `gui.end()` (non-shared mode), or right after your `ofOff::draw()` using a callback.

#### Manual
Allows to precisely control when your gui is rendered, within your oF rendering pipeline.

#### Shared Context
Automatically enabled when 2 `ofxImGui::Gui` instances share the same platform window context. Ensures that the gui renders **after** al instances have sent their gui elements to imgui.

#### Widgets
There are plenty of ImGui widgets. Use example-demo to discover them, together with the demo source code to check how to use them in your project. There's also a useful [interactive demo](https://pthom.github.io/imgui_manual_online/) that runs in your browser.

#### Advanced setup : ImGui config flags.
ofxImGui provides a simple way to interface imgui, but it's a big library providing lots of different options to suit your precise needs.
Most of these advanced options are explained in the `imgui_demo.cpp` source code. Checkout `example-dockingandviewports` and `example-advanced`.

### Examples

#### example-demo    
Shows the imgui demo windows, very useful for discovering all the available widgets and ways of using the ImGui API.

#### example-simple
A very minimal example of how to use imgui within OpenFrameworks.

#### example-advanced
Shows advanced use of imgui within oF, below are some covered topics :
 - OF sepcific image loading.
 - How to create helper functions (simple addons).
 - How to use an addon.
 - How to use the ImGui api to get screen coordinates, and draw that from within oF.
 - It also lets you view the ImGui metrics window, useful for seeing how ImGui is interfaced with oF, it's current inner state and global inspector for debugging purposes.

#### example-fonts
Load custom fonts and fontawesome into ofxImGui.

#### example-helpers
ofxImGui helper functions for interfacing with `ofParameter`.

#### example-ios  
iOS specific with keyboard input helper.

#### example-sharedcontext
When several projects use ofxImGui (*an addon and your ofApp, for example*), you need to share the ofxImGui context.

#### example-multiwindow
How to use one ImGui instance in a multiwindow ofApp. Alpha only, implementation to be finished.

#### example-imguizmo
To be used together with [ofxImGuizmo](https://github.com/nariakiiwatani/ofxImGuizmo.git), an ofxImGui addon.


### Developper info
#### ImGui coding style
ImGui uses a lot of assertions to warn you about API call mistakes. They will make your app crash when something is bad, providing a textual indication about what could be wrong. For more insight, attach your favourite debugger and check the callstack when an assert is triggered.

#### ImGui Documentation
In ImGui, everything is documented in the source code. Resolve a symbol in your IDE and read the comments around the definition. [A documentation is also on its way, covering some big topics](https://github.com/ocornut/imgui/tree/master/docs).

#### Plugins
ImGui has a huge community and is growing fast. There are a lot of plugins available. You can easily add some to your project, they should normally load into ofxImGui. See `example-advanced` and `example-imguizmo`.

#### ofxImGui compilation flags
- `#define OFXIMGUI_DEBUG` : Set to print some information on how ofxImGui runs. Also gives some hints if you're implementing it wrong.
- `#define OFXIMGUI_ENABLE_OF_BINDINGS` to use ofxImGui's legacy way of binding platform windows. Needed for any oF project that doesn't use GLFW windows (the most widely used oF windowing system) or any oF version prior to 0.11. Disables some optional ImGui features and binds to ofEvents. (might be required for iOS / Vulkan usage)

#### Updating ImGui
- Go to the [official ImGui](https://github.com/ocornut/imgui/tree/docking/) repo and get the `glfw` and `opengl 2+3` header and source files from and into the `backends` folders. Beware that we're using the `docking` branch of imgui, until it gets merges in the master.
- Similarly, header and source file from the `imgui root` go into `ofxImGui/libs/imgui/src`.
- After updating: Check ofxImGui's source code for obsolete function usage. See [`IMGUI_DISABLE_OBSOLETE_FUNCTIONS`](#updating-ofximgui) below.

- Make some changes
	- To support GL ES 1 on Rpi, you need to add these lines below `#include <OpenGL/gl.h>` in `imgui_impl_opengl2.cpp`.  
	````cpp
// Rpi GLES 1.1 fixes
#elif defined(TARGET_RASPBERRY_PI) && defined(TARGET_OPENGLES) //&& defined(IMGUI_IMPL_OPENGL_ES1)
#include <GLES/gl.h>
#define glOrtho glOrthof // Fixes rendering
#define glPolygonMode( X, Y )  // no alternative !
#define glPushAttrib( X ) // no alternative !
#define glPopAttrib( X ) // no alternative !
#define GL_POLYGON_MODE GL_LINES
	````
- *Note:* Currently, **oF0.11.0 uses GLFW pre-3.3.0**; this causes the imgui glfw backend to use an unavailable function. Until oF's GLFW library gets updated, `imgui_impl_glfw.cpp` will need to be modified in order to work with ofxImGui. [Change `3300` to `3310`](https://github.com/ocornut/imgui/blob/dd4ca70b0d612038edadcf37bf601c0f21206d28/backends/imgui_impl_glfw.cpp#L62). This change disables some optional imgui features, related to viewport behaviour, and available mouse cursors. Alternatively, you can update GLFW within oF.
````bash
cd OF/scripts
# Only if you don't have apothecary :
git clone https://github.com/openframeworks/apothecary.git
# Manually edit `apothecary/apothecary/apothecary/formulas/glfw.sh`, change to :
# - `VER=3.3-stable` (for gfwf 3.3.x)
# - `VER=master` (for gfwf 3.4.x, recommended)
# - `GIT_URL=https://github.com/glfw/glfw.git`
# Manually delete `apothecary/apothecary/build/glfw if it exists
# Update
./apothecary/apothecary/apothecary -t osx -j 4 update glfw
# Manually copy ./apothecary/glfw to OF/libs/ & recompile oF
````

#### Updating ofxImGui
- `git pull && git submodule update`
- After updating: Add `#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS` in your `imconfig.h` file to make sure you are not using to-be-obsoleted symbols. Update any if needed.


