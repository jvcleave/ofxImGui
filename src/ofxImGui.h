#pragma once

// ImGui GLFW binding with OpenGL
// https://github.com/ocornut/imgui

#include "ofMain.h"
#include "imgui.h"

#define USING_GLFW 0

class ofxImGui
{
public:
    
    ofxImGui();
    ~ofxImGui();
#if USING_GLFW
    GLFWwindow*  glfwWindow;
#endif
    ofTexture fontTexture;
    
    
    void        setup();
    void        begin();
    void        end();
    
    
    void onKeyPressed(ofKeyEventArgs& event);
    void onKeyReleased(ofKeyEventArgs& event);
    void onMousePressed(ofMouseEventArgs& event);
    void onMouseScrolled(ofMouseEventArgs& event);
    
    
    float mouseWheel;
    
    void updateFrame();
    double time;
    bool mousePressed[3];
    
    ImGuiIO* io;
    
    
    // Use if you want to reset your rendering device without losing ImGui state.
    bool createDeviceObjects();

    static const char* getClipboardString();
    static void  setClipboardString(const char* text);
    static void  renderDrawLists(ImDrawData* draw_data);
    
   
    //static vector<ofMesh>* meshes;
    
    
};
