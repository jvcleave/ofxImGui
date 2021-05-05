# Developpers

This readme contains some useful information for coding with imgui.

## Coding with ImGui

### ImGui coding style
ImGui uses a lot of assertions to warn you about API call mistakes. They will make your app crash when something is bad, providing a textual indication about what could be wrong. For more insight, attach your favourite debugger and check the callstack when an assert is triggered.

### ImGui Documentation
In ImGui, everything is documented in the source code. Resolve a symbol in your IDE and read the comments around the definition. [A documentation is also on its way, covering some big topics](https://github.com/ocornut/imgui/tree/master/docs).

### Native UI Elements
There are plenty of ImGui widgets. Use `example-demo` to discover them, together with the demo source code to check how to use them in your project. There's also a useful [interactive demo](https://pthom.github.io/imgui_manual_online/) that runs in your browser.

### ImGui Plugins
ImGui has a huge community and is growing fast. There are a lot of plugins available (gui widgets). You can easily add some to your project, they should normally load into ofxDearImGui. See `example-advanced` and `example-imguizmo`.

### ofxDearImGui compilation flags
- `#define OFXIMGUI_DEBUG` : Set to print some information on how ofxDearImGui runs. Also gives some hints if you're implementing it wrong.
- `#define OFXIMGUI_ENABLE_OF_BINDINGS` to use ofxImGui's legacy way of binding platform windows. Needed for any oF project that doesn't use GLFW windows (the most widely used oF windowing system) or any oF version prior to 0.11. Disables some optional ImGui features and binds to ofEvents. (might be required for iOS / Vulkan usage)

## Updating ImGui
DearImGui has a fast update scheme and changes frequently. ofxImGui rather tries to follow the slower openFrameworks update scheme.  
Here are some instructions for updating DearImGui within ofxImGui:
- Go to the [official ImGui](https://github.com/ocornut/imgui/tree/docking/) repo and get the `glfw` and `opengl 2+3` header and source files from and into the `backends` folders. Beware that we're using the `docking` branch of imgui, until it gets merges in the master.
- Similarly, put the `*.h` and `*.cpp` files from the `imgui root` go into `ofxImGui/libs/imgui/src`.
- Manually compare `ofxImGui/src/imconfig.h` with the new `ofxImGui/libs/imgui/src/imconfig.h`, merge new changes if needed, then delete `ofxImGui/libs/imgui/src/imconfig.h`.
- Apply hacks listed below.
- After updating: Check ofxImGui's source code for detecting obsolete API functions usage.

## Platform specific hacks

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
- *Note:* Currently, **oF 0.11.0 uses GLFW pre-3.3.0**; this causes the imgui glfw backend to use an unavailable function. Until oF's GLFW library gets updated, `imgui_impl_glfw.cpp` will need to be modified in order to work with ofxImGui. (_this has been applied in the master branch already, only when updating DearImGui_)  
Update: oF 0.11.1 [uses GLFW 3.3-stable](https://github.com/openframeworks/apothecary/commit/68a0ec866341a8487d5c555311f3d5975bd62436) and doesn't need this hack.
Update: [oF 0.11.3 will use glfw pre-3.3.0 again](https://github.com/openframeworks/apothecary/pull/197).
  - Option 1: [Change `3300` to `3310`](https://github.com/ocornut/imgui/blob/dd4ca70b0d612038edadcf37bf601c0f21206d28/backends/imgui_impl_glfw.cpp#L62). This change disables some optional imgui features, related to viewport behaviour, and available mouse cursors.
  - Option 2: update GLFW within oF : (**don't proceed on BigSur**)  
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
# Copy ./apothecary/glfw to OF/libs/
./apothecary/apothecary/apothecary -t osx -j 4 copy glfw
# Recompile oF (github installs, not releases)
# cd ./osx && ./compileOF.sh -j3
````
