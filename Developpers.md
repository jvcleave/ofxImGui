# Developpers

This readme contains some useful information for ofxImGui development.

## ofxImGui branches

| Branch   | Description |
| ---------|-------------|
| master   | Up-to-date with the latest oF, quite stable. |
| develop  | Active development branch before releasing to the master |
| OF**     | Master equivalent, blocked at the most recent update compatible with oF 0.**. |

The master branch may not be tested on all platforms. See [Releases](https://github.com/daandelange/ofxImGui/releases/) for more extensively tested versions.

## Coding with ImGui

### ImGui coding style
ImGui uses a lot of assertions to warn you about API call mistakes. They will make your app crash when something is bad, providing a textual indication about what could be wrong. For more insight, attach your favourite debugger and check the callstack when an assert is triggered.

### ImGui Documentation
In ImGui, everything is documented in the source code. Resolve a symbol in your IDE and read the comments around the definition. [A documentation is also on its way, covering some big topics](https://github.com/ocornut/imgui/tree/master/docs).

### Native UI Elements
There are plenty of ImGui widgets. Use `example-demo` to discover them, together with the demo source code to check how to use them in your project. There's also a useful [interactive demo](https://pthom.github.io/imgui_manual_online/) that runs in your browser.

### ImGui Plugins
ImGui has a huge community and is growing fast. There are a lot of plugins available (gui widgets). You can easily add some to your project, they should normally load into ofxImGui. See `example-advanced` and `example-imguizmo`.

### ofxImGui compilation flags
- `#define OFXIMGUI_DEBUG` : Set to print some information on how ofxImGui runs. Also gives some hints if you're implementing it wrong.

## Updating ImGui
DearImGui has a fast update scheme and changes frequently. ofxImGui rather tries to follow the slower openFrameworks update scheme.  
Here are some instructions for updating DearImGui within ofxImGui:
- Download the updated files and put them in `ofxImGui/libs`: (choose one)
    1. Manually:
      - Go to the [official ImGui](https://github.com/ocornut/imgui/tree/docking/) repo and get the `glfw` and `opengl 2+3` header and source files from and into the `backends` folders. Beware that we're using the `docking` branch of imgui, until it gets merged in the master.
      - Similarly, put the `*.h` and `*.cpp` files from the `imgui root` go into `ofxImGui/libs/imgui/src`.
    2. Automatically:
      - Edit the commit you want in `libs/UpdateImGui.sh`.
      - Run: `cd ofxImGui/libs && ./UpdateImGui.sh`.
- Manually compare `ofxImGui/src/imconfig.h` with the new `ofxImGui/libs/imgui/src/imconfig.h`, merge new changes if needed, then delete `ofxImGui/libs/imgui/src/imconfig.h`.
- Apply hacks listed below.
- After updating: Check ofxImGui's source code for detecting obsolete API functions usage using `IMGUI_DISABLE_OBSOLETE_FUNCTIONS`.

### Applying platform specific hacks
After updating imgui, it's required to do some changes.
- *For multiwindow support with viewports enabled* : Enable support for multiple contexts in the glfw backend.  
In `imgui_impl_gflw.cpp` : Add context registering, switching and restoring.
````cpp
// Below #include "imgui_impl_glfw.h"
#include "backends/imgui_impl_glfw_context_support.h" // CUSTOM ADDED LINE FOR OFXIMGUI

// Within all GLFW callback functions taking `window` as 1st arg, add this to the begin :
ImGui_ImplGlfw_ScopedContext sc(window); // CUSTOM ADDED LINE FOR OFXIMGUI

// At the end of ImGui_ImplGlfw_CreateWindow :
ImGui_ImplGlfw_RegisterWindowContext(vd->Window, ImGui::GetCurrentContext()); // CUSTOM ADDED LINE FOR OFXIMGUI

// In ImGui_ImplGlfw_DestroyWindow, right before `if (vd->WindowOwned)` :
ImGui_ImplGlfw_RemoveWindowContext(vd->Window); // CUSTOM ADDED LINE FOR OFXIMGUI
````  
Switching contexts is possible since these issues, but still we are in "Isolation mode". Right now, ImGui doesn't offer a way to register additional host viewports from a native system window, but they plan to support it in the future. So now we create an ImGui context per ofAppWindow.
Related issues:
     - [Multiple GLFW contexts : glfw event binding](https://github.com/ocornut/imgui/issues/5439)
     - [Does GLFW implementation only handle one GLFWindow?](https://discourse.dearimgui.org/t/does-glfw-implementation-only-handle-one-glfwindow/305)
     - [Add support for multiple GLFW contexts](https://github.com/ocornut/imgui/pull/3934)
     - [Multiple host viewports](https://github.com/ocornut/imgui/issues/3012)
     - [Correct use of ImGui_ImplGlfw_NewFrame with multiple ImGui contexts, and g_Time](https://github.com/ocornut/imgui/issues/2526)
     - [Nesting multiple imgui contexts (glfw+opengl3)](https://github.com/ocornut/imgui/issues/2004)
- *Add GL ES 1 support so that it compiles on Rpis :*  in `imgui_impl_opengl2.cpp`
````cpp
// --- CUSTOM MODIFICATION
// Rpi dirty fix : Add support for GLES 1.1, used by the imgui fixed pipeline.
#elif defined(TARGET_RASPBERRY_PI) && defined(TARGET_OPENGLES) // && defined(IMGUI_IMPL_OPENGL_ES1)
#include "gles1CompatibilityHacks.h"
// --- END CUSTOM MODIFICATION
````
- *GLFW compatibility* : The raspberry pi might have an old GLFW implementation, please add these lines in `imgui_impl_glfw.cpp` to ensure cross platform compatibility.  
````cpp
// Custom modification : Add support for older GLFW versions (<3.2) (on Rpi Stretch for example)
#include "glfwCompatibilityHacks.h"
````
 *Note:* Currently, **oF 0.11.0 uses GLFW pre-3.3.0**; this causes the imgui glfw backend to use an unavailable function. Until oF's GLFW library gets updated, `imgui_impl_glfw.cpp` will need to be modified in order to work with ofxImGui. (_this has been applied in the master branch already using option 1, only when updating DearImGui. You can manually revert the change and update your GLFW if you wish._)  
Update: oF 0.11.1 [uses GLFW 3.3-stable](https://github.com/openframeworks/apothecary/commit/68a0ec866341a8487d5c555311f3d5975bd62436) and doesn't need this hack.  
Update: [oF 0.11.2 uses glfw pre-3.3.0 again](https://github.com/openframeworks/apothecary/pull/197).  
Update: [oF 0.11.3 will use glfw 3.3.7](https://github.com/openframeworks/apothecary/pull/225).  
  - [Change `3300` to `3310`](https://github.com/ocornut/imgui/blob/dd4ca70b0d612038edadcf37bf601c0f21206d28/backends/imgui_impl_glfw.cpp#L62). This change disables some optional imgui features, related to viewport behaviour, and available mouse cursors.  

# Improve ofxImGui's backend bindings
In order to enable more native mouse cursors, and possibly other interface polishes, you can update GLFW within your oF installation. (**don't proceed on BigSur**)  
_This should also enable gamepad support on RPI with Raspbian <= Buster which ships with GLFW <= 3.2.1._  
````bash
# Instructions for Mac/Win/Linux
cd OF/scripts
# Only if you don't have apothecary (OF release zip):
git clone https://github.com/openframeworks/apothecary.git
# Manually edit the GLFW formula `apothecary/apothecary/apothecary/formulas/glfw.sh`, change to :
# - `VER=3.3-stable` (for gfwf 3.3.x)
# - `VER=master` (for gfwf 3.4.x, recommended)
# - `GIT_URL=https://github.com/glfw/glfw.git`
# Manually delete `apothecary/apothecary/build/glfw if it exists
# Update (change osx by vs/msys2/linux/linux64/linuxarmv6l/linuxarmv7l or remove `-t osx` for autoselect)
./apothecary/apothecary/apothecary -t osx -j 4 update glfw
# Copy ./apothecary/glfw to OF/libs/
./apothecary/apothecary/apothecary -t osx -j 4 copy glfw
# Recompile oF (github installs only, not releases)
# cd ./osx && ./compileOF.sh -j3
````  
````bash
# Instructions for rpi distros
# Raspbian <= Buster don't have GLFW 3.3 in their repos, but you can pray.
apt update && apt upgrade libglfw3 libglfw3-dev
````  
Then you can [revert `3310` to `3300`](https://github.com/ocornut/imgui/blob/dd4ca70b0d612038edadcf37bf601c0f21206d28/backends/imgui_impl_glfw.cpp#L62) to tell imgui to use more precise cursors.

# Future directions and contribution suggestions
If you have some time to improve ofxImGui, please do and submit a pull request. There are things to be improved for any programer skill level. This list is non-exhaustive.  
Please keep in mind that our philosophy is to stick with the slower oF release cycle rather then the fast Dear Imgui one.

 - There's some work needed on to make the Helpers more useful and to document their features. Improving the Helpers example could help. Also, there's been discussions about a layout engine to position windows next to eachother. Other ideas include saving more states, such as collapsed sections.
 - There is also some work to be done about event handling/bubbling. Currently, mouse and keyboard interactions are both send to oF and ofxImGui. In oF-event-space, how can one know if ImGui is using the mouse/keyboard data or not ? (`example-advanced` could be extended with some demo code on this subject, or even some helper functions to facilitate this).  
- RaspberryPi support will probably [evolve soon](https://github.com/openframeworks/openFrameworks/issues/6713). If using GLFW, imgui support will be good. But it'be great to be able ofxImGui from the console (without X11 desktop environment = more CPU & RAM available) will be a great addition. If oF 0.12 gets, a custom backend will need to be written, maybe letting the user choose between `1` a simpler console-compatible backend or `2` a GLFW-bound fully-featured backend.