#pragma once

#include "ofMain.h"
#include "imgui.h"


class ofxImGui
{
public:
    
    ofxImGui();
    ~ofxImGui();
#ifndef TARGET_OPENGLES
    GLFWwindow*  glfwWindow;
#endif
    static ofTexture fontTexture;
    
    
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
    static ofFloatColor convertToFloatColor(ImU32 rgba);

};
