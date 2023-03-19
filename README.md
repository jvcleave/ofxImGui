# ofxImGui

ofxAddon that allows you to use [ImGui](https://github.com/ocornut/imgui) in [openFrameworks](https://github.com/openframeworks/openFrameworks).

![Screenshot](images/Screenshot.png)

> #### Develop Branch
> The current `master` branch embeds an ImGui version is quite old and newer versions are not compatible anymore.  
> A refactored version of ofxImGui that you can try is available in the [develop branch](https://github.com/jvcleave/ofxImGui/tree/develop), it comes with a lot of extra (optional) features and new ImGui API methods and UI widgets.  
> **We're looking for feedback** in [#110](https://github.com/jvcleave/ofxImGui/issues/110). _(Tested: Desktops Win/Linux/Mac, Rpi and iOS Simulator)._


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


[![Build status](https://travis-ci.org/jvcleave/ofxImGui.svg?branch=master)](https://travis-ci.org/jvcleave/ofxImGui)
