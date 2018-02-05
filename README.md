# ofxImGui

ofxAddon that allows you to use [ImGui](https://github.com/ocornut/imgui) in [openFrameworks](https://github.com/openframeworks/openFrameworks).

### Compatibility
Master branch may not be tested on all platforms. See [Releases](https://github.com/jvcleave/ofxImGui/releases/) for more extensively tested versions.

Release versions tested with OF 0.9.x on
 - Raspberry Pi 1 and 2
 - Mac OS 10.11, Xcode 7
 - iOS 9.2 and 8.1.2, Xcode 7
 - Linux Desktop
 - Windows 10, Visual Studio 2015



#### Upgrade Notes
Versions 1.50 and higher use the keyword `ofxImGui` as a namespace for all classes in the addon, while previous versions just had a class called `ofxImGui`. If you're upgrading from a previous version, make sure to replace all instances of class `ofxImGui` with `ofxImGui::Gui`.

### Examples

![Screenshot](images/Screenshot.png)

#### example-demo    
Works on Desktop, uses imgui demo windows.

#### example-helpers
Works on Desktop, uses ofxImGui helper functions for interfacing with `ofParameter`.

#### example-ios  
iOS specific with keyboard input helper.

### Basic usage

inside **ofApp.h** :
```
#include "ofxImGui.h"

// other stuff...

void drawImGui();
ofxImGui::Gui gui;

// your custom parameters
float GUI_threshold;
float GUI_max_distance;
float GUI_bg_alpha;
int GUI_max_points;
```
**ofApp.cpp**

```
//--------------------------------------------------------------
void ofApp::draw(){
 
 // your stuff..
 
 drawImGui();
 
}

//--------------------------------------------------------------
void ofApp::drawImGui(){

 gui.begin();
	
 auto main_settings = ofxImGui::Settings();
 main_settings.windowPos = ofVec2f(10, 500); // set position
 main_settings.windowSize = ofVec2f(0.0f, 0.0f); // auto resize based on parameters dimensions

 ofxImGui::BeginWindow("GUI", main_settings, false);
 
 ImGui::SliderFloat("Threshold", &GUI_threshold, 0.0f, 200.0f);
 ImGui::SliderInt("Max Points", &GUI_max_points, 10, 512);
 ImGui::SliderFloat("Max Distance", &GUI_max_distance, 0.5f, 100.0f);
 ImGui::SliderFloat("Bg Alpha", &GUI_bg_alpha, 0.0f, 255.0f);
 
 ofxImGui::EndWindow(main_settings);
	
 gui.end();
}
```
