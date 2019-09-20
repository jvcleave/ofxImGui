# I have added
## Docking (in docking branch)

Allow to use docking with ofxImGui.

To activate docking in ofApp::Setup() : 

```C++
  // Init ImGui with docking
  gui.enableDocking();
  gui.setup();
```

![Docking](images/docking.png)

## A Knob Widget 
Also in docking branch I think, need to cleen a bit :-)

![Docking](images/knob.png)

# ofxImGui

ofxAddon that allows you to use [ImGui](https://github.com/ocornut/imgui) in [openFrameworks](https://github.com/openframeworks/openFrameworks).

![Screenshot](images/Screenshot.png)

### Compatibility
Master branch may not be tested on all platforms. See [Releases](https://github.com/jvcleave/ofxImGui/releases/) for more extensively tested versions.

#### Test Platforms
ofxImGui should run on the [latest openFrameworks release and it's OS/IDE requirements](https://openframeworks.cc/download/). These are typically:

 - Mac OSX, Xcode
 - Windows 10, Visual Studio
 - Raspberry Pi
 - Linux Desktop (Ubuntu)


### Examples

#### example-demo    
imgui demo windows with some OF sepcific image loading

#### example-helpers
ofxImGui helper functions for interfacing with `ofParameter`.

#### example-ios  
iOS specific with keyboard input helper.
