# ofxImGui

ofxAddon that allows you to use [ImGui](https://github.com/ocornut/imgui) in [openFrameworks](https://github.com/openframeworks/openFrameworks).
Unlike most C++ gui libraries, ImGui uses the [immediate mode](https://en.wikipedia.org/wiki/Immediate_mode_(computer_graphics)) API pattern, rather then OOP. In other words, every frame you rebuild the whole GUI, executing only the necessary parts, and ImGui is extremely fast. This makes the Gui closer to the OpenGL api and also to the inner oF logic (`update()`, `draw()`, `begin()`, `end()`, `push()`, `pop()`, etc.).

![Screenshot](images/Screenshot.png)

### Compatibility
Master branch may not be tested on all platforms. See [Releases](https://github.com/jvcleave/ofxImGui/releases/) for more extensively tested versions.

#### Test Platforms
ofxImGui should run on the [latest openFrameworks release and it's OS/IDE requirements](https://openframeworks.cc/download/). These are typically:

 - Mac OSX, Xcode
 - Windows 10, Visual Studio
 - Raspberry Pi
 - Linux Desktop (Ubuntu)

 Please note that using ofxImGui in multiwindow openframworks applications is not yet supported, future versions of ImGui might allow this.

### Usage
#### Auto

#### Manual

#### Chaining

#### Widgets
There are plenty of ImGui widgets. Use example-demo to see them all, and the demo source code for how to use them. There's also an [interactive demo](https://pthom.github.io/imgui_manual_online/) around that runs in your browser.

#### Advanced : ImGui config flags.

### Examples

#### example-demo    
Shows the imgui demo windows, very useful for discovering all the available widgets and ways of using the ImGui API. It also lets you view the ImGui metrics window, useful for seeing how ImGui is interfaced with oF, it's current inner state and global inspector for debugging purposes.

#### example-advanced
Shows advances use of imgui within oF. There's some OF sepcific image loading, and how to include an addon.
Also, it shows how to use the ImGui api to get gui coordinates, and draw that from within oF.

#### example-helpers
ofxImGui helper functions for interfacing with `ofParameter`.

#### example-ios  
iOS specific with keyboard input helper.

#### example-multiwindow
How to use one ImGui instance in a multiwindow ofApp. Alpha only.



### Developper info
#### ImGui coding style
ImGui assertions.

#### Tools
In ImGui, everything is documented in the source code. Resolve a symbol in your IDE then read the comments around.

#### Plugins
ImGui has a huge community and is growing fast. There's a lot of plugins available. You can easily add some to your project, they should normally load into ofxImGui.

#### Compilation flags
- `#define OFXIMGUI_DEBUG` : Set to `cout` some information of how ofxImGui runs.
- `#define OFXIMGUI_MULTIWINDOW_IMPL` to enable alpha support for multiwindow ofApps. Note: this is not about the imgui viewport.
- `#define OFXIMGUI_ENABLE_OF_BINDINGS` to use ofxImGui's legacy way of binding platform windows. (not recommended, some features might be missing)

#### Updating ImGui
- Go to the [official ImGui](https://github.com/ocornut/imgui/tree/docking/) repo and get the glwf and opengl 2+3 header and source files from and into the `backends` folders. Beware that we're using the `docking` branch of imgui.
- Similarly, header and source file from the imgui root go into `ofxImGui/libs/imgui/src`.
- Currently, oF0.11.0 uses GLFW pre-3.3.0; this causes the imgui glfw backend to use an unavailable function. Until oF's GLWF library gets updated, `imgui_impl_glfw.cpp` will need to be modified in order to work with ofxImGui. [Change 3300 to 3310](https://github.com/ocornut/imgui/blob/dd4ca70b0d612038edadcf37bf601c0f21206d28/backends/imgui_impl_glfw.cpp#L62). This change disables some optional imgui features. Alternatively, you can update GLFW within oF.
- After updating: See `IMGUI_DISABLE_OBSOLETE_FUNCTIONS` below.

#### Updating ofxImGui
- After updating: Add `#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS` in your `imconfig.h` file to make sure you are not using to-be-obsoleted symbols. Update any if needed.

[![Build status](https://travis-ci.org/jvcleave/ofxImGui.svg?branch=master)](https://travis-ci.org/jvcleave/ofxImGui)
