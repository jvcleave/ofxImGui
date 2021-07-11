# Developpers

This readme contains some useful information for ofxImGui development.

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
- Go to the [official ImGui](https://github.com/ocornut/imgui/tree/docking/) repo and get the `glfw` and `opengl 2+3` header and source files from and into the `backends` folders. Beware that we're using the `docking` branch of imgui, until it gets merges in the master.
- Similarly, put the `*.h` and `*.cpp` files from the `imgui root` go into `ofxImGui/libs/imgui/src`.
- Manually compare `ofxImGui/src/imconfig.h` with the new `ofxImGui/libs/imgui/src/imconfig.h`, merge new changes if needed, then delete `ofxImGui/libs/imgui/src/imconfig.h`.
- Apply hacks listed below.
- After updating: Check ofxImGui's source code for detecting obsolete API functions usage.

### Applying platform specific hacks
After updating imgui, it's required to do some changes.
- *For multiwindow support* (until the backend evolves towards multi context support or other).  
In `imgui_impl_gflw`, add this right below the first `IM_ASSERT`s in `ImGui_ImplGlfw_NewFrame(){}` :  
````cpp
	// Custom hack : switch g_Window too !
	#ifdef IMGUI_BACKEND_GLFW_CUSTOM_NEWFRAME
        IMGUI_BACKEND_GLFW_CUSTOM_NEWFRAME();
    #endif
````  
This issue is that some globals are hardcoded. Switching context is possible, but the `g_Window` global variable isn't switched.  
Related issues:
     - [Does GLFW implementation only handle one GLFWindow?](https://discourse.dearimgui.org/t/does-glfw-implementation-only-handle-one-glfwindow/305)
     - [Add support for multiple GLFW contexts](https://github.com/ocornut/imgui/pull/3934)
     - [Multiple host viewports](https://github.com/ocornut/imgui/issues/3012)
     - [Correct use of ImGui_ImplGlfw_NewFrame with multiple ImGui contexts, and g_Time](https://github.com/ocornut/imgui/issues/2526)
     - [Nesting multiple imgui contexts (glfw+opengl3)](https://github.com/ocornut/imgui/issues/2004)
- *Add GL ES 1 support so that it compiles on Rpis :*  
````cpp
// --- CUSTOM MODIFICATION
// Rpi dirty fix : Add support for GLES 1.1, used by the imgui fixed pipeline.
#elif defined(TARGET_RASPBERRY_PI) && defined(TARGET_OPENGLES) // && defined(IMGUI_IMPL_OPENGL_ES1)
#include "gles1CompatibilityHacks.h"
// --- END CUSTOM MODIFICATION
````
- *Cleaner backend code* : `GLFW_GAMEPAD_BUTTON_A` ... `GLFW_GAMEPAD_BUTTON_LAST` are hardcoded in the imgui backend while GLFW provides macros so they are easier to read. You can replace them with the following lines of code.  
````cpp
    MAP_BUTTON(ImGuiNavInput_Activate,   GLFW_GAMEPAD_BUTTON_A);     // Cross / A
    MAP_BUTTON(ImGuiNavInput_Cancel,     GLFW_GAMEPAD_BUTTON_B);     // Circle / B
    MAP_BUTTON(ImGuiNavInput_Menu,       GLFW_GAMEPAD_BUTTON_X);     // Square / X
    MAP_BUTTON(ImGuiNavInput_Input,      GLFW_GAMEPAD_BUTTON_Y);     // Triangle / Y
    MAP_BUTTON(ImGuiNavInput_DpadLeft,   13);//GLFW_GAMEPAD_BUTTON_DPAD_LEFT);    // D-Pad Left
    MAP_BUTTON(ImGuiNavInput_DpadRight,  14);//GLFW_GAMEPAD_BUTTON_DPAD_RIGHT);    // D-Pad Right
    MAP_BUTTON(ImGuiNavInput_DpadUp,     GLFW_GAMEPAD_BUTTON_DPAD_UP);    // D-Pad Up
    MAP_BUTTON(ImGuiNavInput_DpadDown,   12);//GLFW_GAMEPAD_BUTTON_DPAD_DOWN);    // D-Pad Down
    MAP_BUTTON(ImGuiNavInput_FocusPrev,  GLFW_GAMEPAD_BUTTON_LEFT_BUMPER);     // L1 / LB
    MAP_BUTTON(ImGuiNavInput_FocusNext,  GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER);    // R1 / RB
    MAP_BUTTON(ImGuiNavInput_TweakSlow,  GLFW_GAMEPAD_BUTTON_LEFT_BUMPER);      // L2 / LT
    MAP_BUTTON(ImGuiNavInput_TweakFast,  GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER);     // R2 / RT
    MAP_ANALOG(ImGuiNavInput_LStickLeft, GLFW_GAMEPAD_AXIS_LEFT_X,  -0.3f,  -0.9f);
    MAP_ANALOG(ImGuiNavInput_LStickRight,GLFW_GAMEPAD_AXIS_LEFT_X,  +0.3f,  +0.9f);
    MAP_ANALOG(ImGuiNavInput_LStickUp,   GLFW_GAMEPAD_AXIS_LEFT_Y,  -0.3f,  -0.9f);
    MAP_ANALOG(ImGuiNavInput_LStickDown, GLFW_GAMEPAD_AXIS_LEFT_Y,  +0.3f,  +0.9f);
````  
But then the raspberry pi might have an old GLFW implementation, please add these lines to ensure cross platform compatibility.  
````cpp
// Custom modification : Add support for older GLFW versions (<3.2) (on Rpi Stretch for example)
#include "glfwCompatibilityHacks.h"
````
 *Note:* Currently, **oF 0.11.0 uses GLFW pre-3.3.0**; this causes the imgui glfw backend to use an unavailable function. Until oF's GLFW library gets updated, `imgui_impl_glfw.cpp` will need to be modified in order to work with ofxImGui. (_this has been applied in the master branch already using option 1, only when updating DearImGui. You can manually revert the change and update your GLFW if you wish._)  
Update: oF 0.11.1 [uses GLFW 3.3-stable](https://github.com/openframeworks/apothecary/commit/68a0ec866341a8487d5c555311f3d5975bd62436) and doesn't need this hack.  
Update: [oF 0.11.2 uses glfw pre-3.3.0 again](https://github.com/openframeworks/apothecary/pull/197).  
  - [Change `3300` to `3310`](https://github.com/ocornut/imgui/blob/dd4ca70b0d612038edadcf37bf601c0f21206d28/backends/imgui_impl_glfw.cpp#L62). This change disables some optional imgui features, related to viewport behaviour, and available mouse cursors.  

# Improve ofxImGui's backend bindings
In order to enable more native mouse cursors, and possibly other interface polishes, you can update GLFW within your oF installation. (**don't proceed on BigSur**)  
_This should also enable gamepad support on RPI with Raspbian <= Buster which ships with GLFW <= 3.2.1._  
````bash
# Instructions for Mac/Win/Linux
cd OF/scripts
# Only if you don't have apothecary :
git clone https://github.com/openframeworks/apothecary.git
# Manually edit `apothecary/apothecary/apothecary/formulas/glfw.sh`, change to :
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