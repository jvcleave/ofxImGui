#pragma once

#include "ofMain.h"
#include "ofApp.h"
#include "ofxImGui.h"
#include "ofConstants.h"
#include <cstdio>

#ifdef OFXIMGUI_DEBUG
#pragma message "Compiling with debug flags. Below you'll find some build information."
#if defined(IMGUI_IMPL_OPENGL_ES2)
#pragma message "IMGUI_IMPL_OPENGL_ES2"
#endif
#if defined(IMGUI_IMPL_OPENGL_ES3)
#pragma message "IMGUI_IMPL_OPENGL_ES3"
#endif
#if defined(TARGET_OPENGLES)
#pragma message "TARGET_OPENGLES"
#endif
#if defined(TARGET_OPENGL) // 404 ?
#pragma message "TARGET_OPENGL"
#endif
#if defined(TARGET_PROGRAMMABLE_GL) // 404, Not set by of ?
#pragma message "TARGET_PROGRAMMABLE_GL"
#endif
#if defined(TARGET_GLFW)
#pragma message "TARGET_GLFW"
#endif
#if defined(TARGET_RASPBERRY_PI)
#pragma message "TARGET_RASPBERRY_PI"
#endif
#if defined(TARGET_RASPBERRY_PI_LEGACY)
#pragma message "TARGET_RASPBERRY_PI_LEGACY"
#endif
#if defined(TARGET_LINUX)
#pragma message "TARGET_LINUX"
#endif
#if defined(TARGET_OSX)
#pragma message "TARGET_OSX"
#endif
#if defined(TARGET_WIN32)
#pragma message "TARGET_WIN32"
#endif
#if defined(TARGET_OF_IOS)
#pragma message "TARGET_OF_IOS"
#endif
#if defined(TARGET_ANDROID)
#pragma message "TARGET_ANDROID"
#endif
#if defined(TARGET_EMSCRIPTEN)
#pragma message "TARGET_EMSCRIPTEN"
#endif
#if defined(OF_TARGET_API_VULKAN)
#pragma message "OF_TARGET_API_VULKAN"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GLEW"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GLAD"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GLAD2"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GL3W"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GLBINDING3"
#endif
#if defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#pragma message "IMGUI_IMPL_OPENGL_LOADER_GLBINDING2"
#endif
#endif // OFXIMGUI_DEBUG

// Comment below to use manual drawing
#define USE_AUTODRAW

// Only on GLFW platforms (TARGET_GLFW_WINDOW is only set on rpi, default on other platforms)
#if !defined( TARGET_RASPBERRY_PI ) || defined( TARGET_GLFW_WINDOW )
	#define GLFW_WINDOWING_SYSTEM_IS_USED

    // Needed to get the macro definitions since OF0.12
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

	#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3 // Only GLFW > 3.3 has gamepad/joystick functions
		#define GLFW_HAS_GAMEPAD_SUPPORT
	#endif
#endif

class ofApp : public ofBaseApp {

	public:
        ofApp() {}

		void setup() {
            ImGuiConfigFlags flags = ImGuiConfigFlags_None;
            flags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            flags |= ImGuiConfigFlags_NavEnableKeyboard;
            flags |= ImGuiConfigFlags_ViewportsEnable;

            #ifdef USE_AUTODRAW
            gui.setup(nullptr, true, flags);
            #else
            gui.setup(nullptr, false, flags);
            #endif

#ifndef TARGET_OPENGLES
            ofEnableGLDebugLog();
#endif

            ofLogNotice("Renderer Type") << ofGetCurrentRenderer()->getType().c_str();
            ofLogNotice("Window Context") << ofGetCurrentWindow()->getWindowContext();
            auto window = ofGetCurrentWindow();
            if( dynamic_cast< ofAppBaseWindow* >(window.get()) )
                ofLogNotice("Window is Base");
//            if( dynamic_cast< ofAppGLWindow* >(window.get()) )
//                ofLogNotice("Window is GL");
//            if( dynamic_cast< ofGLESWindowSettings * >(window.get()->renderer()->getWindowContext()) )
//                ofLogNotice("Window is GLES");
//#ifdef TARGET_OPENGLES
            if( auto glfwWin = dynamic_cast< ofAppGLFWWindow* >(window.get()) ){
                ofLogNotice("Window is GLFW");
                auto GLFWsettings = glfwWin->getSettings();
#if defined TARGET_OPENGLES
                if( dynamic_cast< ofGLESWindowSettings* >( &GLFWsettings ) ){
                    ofLogNotice("OpenGL is GLES") << "Version " << GLFWsettings.glesVersion;
                }
#else //if defined TARGET_OPENGL
                if( dynamic_cast< ofGLWindowSettings* >( &GLFWsettings ) ){
                    ofLogNotice("OpenGL is GL") << "Version " << GLFWsettings.glVersionMajor << "." << GLFWsettings.glVersionMinor ;
                }
#endif

#ifdef GLFW_HAS_GAMEPAD_SUPPORT
                // Load custom gamepad mapping
                // GLFW uses the same format as the SDL database. Also allows porting new devices.
                // The default XBOX 360 binding doesn't work with 360controller driver on osx, this one does.
                // See: https://github.com/SDL-mirror/SDL/blob/master/src/joystick/SDL_gamecontrollerdb.h
                glfwUpdateGamepadMappings("030000005e0400008e02000000000000,X360 Controller (custom),a:b0,b:b1,back:b9,dpdown:b12,dpleft:b13,dpright:b14,dpup:b11,guide:b10,leftshoulder:b4,leftstick:b6,lefttrigger:a2,leftx:a0,lefty:a1,rightshoulder:b5,rightstick:b7,righttrigger:a5,rightx:a3,righty:a4,start:b8,x:b2,y:b3,platform:Mac OS X,");
#endif
            }
//            if( dynamic_cast< ofAppGlutWindow* >(window.get()) )
//                ofLogNotice("Window is Glut");
//            if( dynamic_cast< ofAppEGLWindow* >(window.get()) )
//                ofLogNotice("Window is EGL");
//            if( dynamic_cast< ofAppNoWindow* >(window.get()) )
//                ofLogNotice("Window is GLFW");
		}

		void draw() {
            // Start drawing to ImGui (newFrame)
			gui.begin();

			// Official ofxImGui debug window & doctor
			gui.drawOfxImGuiDebugWindow();

            // The GUI hasn't been rendered yet : we can still draw below it
            ofDrawBitmapStringHighlight( ofToString("Render order test :"), 10, 20);
            ofDrawBitmapStringHighlight( ofToString(" - I'm below the GUI"), 10, 40);

            #ifndef OFXIMGUI_DEBUG
            ofDrawBitmapStringHighlight("You need to define OFXIMGUI_DEBUG to compile the inspector !", 10, 80);
            #endif

            // End our ImGui Frame.
            // Also Renders if using autoDraw AND non-slave mode
            gui.end();

#ifndef USE_AUTODRAW
            gui.draw(); // <-- In manual mode, you can choose to render imgui at a given moment in your rendering pipeline
#endif

#ifdef USE_AUTODRAW
            ofDrawBitmapStringHighlight( " - In autodraw mode, you don't have rendering-order control...", 10, 60); // <-- This text will be below the gui as ofxImGui renders after your ofApp::draw
#else
            ofDrawBitmapStringHighlight( " - I should be over the Gui ! (autoDraw is off)", 10, 60); // <-- This text will be over the gui
#endif
		}

        void update(){

        }

    private:
        ofxImGui::Gui gui;
};
