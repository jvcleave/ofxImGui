# ofxImGui

ofxAddon that allows you to use [ImGui](https://github.com/ocornut/imgui) in [openFrameworks](https://github.com/openframeworks/openFrameworks).

Unlike most C++ gui libraries, ImGui uses the [immediate mode](https://en.wikipedia.org/wiki/Immediate_mode_(computer_graphics)) paradigm rather then being OOP. In other words, every frame you rebuild the whole GUI, executing only the necessary parts, and ImGui is surprisingly fast at that ! This makes the Gui closer to the OpenGL api as to the inner oF logic (`update()`, `draw()`, `begin()`, `end()`, `push()`, `pop()`, etc.).

![Screenshot](images/Screenshot.png)

### Fork info
ofxImGui is a fork of the original [jvcleave/ofxImGui](https://github.com/jvcleave/ofxImGui), which cannot be updated anymore to newer versions of ImGui [since january 2020](https://github.com/ocornut/imgui/releases/tag/v1.80), and has been relying on depreciated functions for a few years.   

### Branches
| Branch   | Description |
| ---------|-------------|
| master   | Up-to-date with the latest oF, quite stable. |
| develop  | Active development branch before releasing to the master |
| jvcleave | Depreciated. Used for merging into jvcleave/ofxImGui. |
| legacy   | Depreciated. Used for compatibility with the original oF bindings. Can be used to have an updated (but limited) DearImGui in legacy ofxImGui projects. Can be used in non-GLFW rpi setups. |
| OF**     | Master equivalent, blocked at the most update compatible with oF 0.**. |

The master branch may not be tested on all platforms. See [Releases](https://github.com/daandelange/ofxImGui/releases/) for more extensively tested versions.


### Supported Platforms
ofxImGui should run on the [latest openFrameworks release and it's OS/IDE requirements](https://openframeworks.cc/download/). These are typically:

 - Mac OSX, Xcode
 - Windows 10, Visual Studio
 - Raspberry Pi
 - Linux Desktop

 Please note that using **ofxImGui in multiwindow OpenFrameworks applications is in testing phase**. For now, one ImGui instance is created per oF window, without any communication in between them (cross-docking won't work). Hopefully DearImGui will introduce something to handle "multiple host viewports".

 Currently, **ofxImGui only works on GLFW-based openFrameworks applications**. Thus, the RPI_LEGACY, iOS and Vulkan implementations might be broken; as they've not yet been ported to the new ImGui backends.

#### GLSL Support Table

| OS | OpenGL 2.x (fixed pipeline) | OpenGL 3.x  | OpenGL 4.x | GL ES 1.0 (fixed pipeline) | GL ES 2.0 |
|---|---|---|---|---|---|
| Windows | Yes | Yes | Yes | Unknown | Unknown |
| Mac OsX | Yes | Yes | Yes |  *Unavailable* | *Unavailable* |
| Linux | Yes | Yes | Yes | Yes | Yes |
| iOS | *Unavailable* | *Unavailable* | *Unavailable* | Unknown | Unknown |
| Rpi3 | Unknown | Unknown | Unknown | Yes (with Legacy driver and `OFXIMGUI_ENABLE_OF_BINDINGS`) | Yes (KMS and Legacy driver) |
| Rpi4 | Unknown | Should (with KMS driver) | Should (with KMS driver) | Should | Should |
| iOS | *Unavailable* | *Unavailable* | *Unavailable* | Should | Should |

#### oF & ImGui Support Table
| ofxImGui version       | ImGui version | Supported oF version |
|-----------------------:|---------------|----------------------|
| ofxImGui 1.82      | 1.82*         | 0.11.2 |
| ofxImGui 1.79      | 1.79*         | 0.11.1 |
| d3cod3/ofxImGui 1.??   | 1.??*         | 0.11.x |
| jvcleave/ofxImGui 1.75 | 1.75          | 0.11.x |
| jvcleave/ofxImGui 1.62 | 1.62          | 0.10.x | 
| jvcleave/ofxImGui 1.50 | 1.50 WIP      | 0.10.x |
| jvcleave/ofxImGui 1.49 | 1.49          | 0.10.x |
| jvcleave/ofxImGui 1.47 | 1.47          | 0.10.x |
| jvcleave/ofxImGui 0.90 | 1.45 WIP      | 0.9.x  |

New ImGui versions bring changes and new API features, sometimes depreciations.  
__*__ Uses the native ImGui backend, offering pop-out-windows (viewports), docking, gamepad control, and more.

## Usage

### Install
````bash
cd /path/to/of/addons && git clone https://github.com/Daandelange/ofxImGui.git
````

#### Optional
Configure oF (tested with 0.11.0) to use GLFW 3.4 and imgui will have an even more polished interface. See [Developpers.md](./Developpers.md).

### Setup
In the setup, you can choose to automatically render, or choose to manually render the gui if you need more precise control of when it's drawn to the screen.  
ofxImGui implements DearImGui in such a way that each oF window gets its own imgui context, seamlessly shared between any oF window context. You can also load multiple instances of it in the same ofApp. Note: Only the fist call to `gui.setup()` has full control over the settings; the next ones setup as slaves.

#### Auto
Automatically renders the gui then you call `gui.end()` (non-shared mode), or right after your `ofApp::draw()` using a callback.

#### Manual
Allows to precisely control when your gui is rendered, within your oF rendering pipeline.

#### Shared Context
Automatically enabled when 2 `ofxImGui::Gui` instances share the same platform window context. Ensures that the gui renders **after** al instances have sent their gui elements to imgui.

#### Advanced setup : ImGui config flags.
ofxImGui provides a simple way to interface imgui, but it's a big library providing lots of different options to suit your precise needs.
Most of these advanced options are explained in the `imgui_demo.cpp` source code. Checkout `example-dockingandviewports` and `example-advanced`.

### Examples
There are several example projects, covering from the simplest use case to more advanced ones : [Examples.md](./Examples.md).

### Developper info
Useful dev info and how to get familiar with DearImGui : [Developper.md](./Developpers.md).

### Updating ofxImGui
- `cd /path/to/ofxImGui && git pull && git submodule update`
- After updating: Add `#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS` in your `imconfig.h` file to make sure you are not using to-be-obsoleted symbols. Update any if needed.


