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
    void        setup(bool install_callbacks);
    void        Shutdown();
    void        NewFrame();
    
    // Use if you want to reset your rendering device without losing ImGui state.
    void        InvalidateDeviceObjects();
    bool        CreateDeviceObjects();

    static const char* GetClipboardText();
    static void  SetClipboardText(const char* text);
    static void  RenderDrawLists(ImDrawData* draw_data);
    
    // Data
    


    ofTexture g_FontTexture;

    GLFW_WindowListener* windowListener;
    ImGuiIO* io;
};
