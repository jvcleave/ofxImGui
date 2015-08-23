#pragma once

// ImGui GLFW binding with OpenGL
// https://github.com/ocornut/imgui

#include "ofMain.h"
#include "imgui.h"
#include "GLFW_WindowListener.h"


class ofxImGui
{
public:
    
    ofxImGui();
    ~ofxImGui();
    void        setup();
    void        begin();
    void        end();
        
    // Use if you want to reset your rendering device without losing ImGui state.
    void        InvalidateDeviceObjects();
    bool        CreateDeviceObjects();

    static const char* GetClipboardText();
    static void  SetClipboardText(const char* text);
    static void  RenderDrawLists(ImDrawData* draw_data);
    
    ofTexture fontTexture;

    GLFW_WindowListener* windowListener;
    ImGuiIO* io;
};
