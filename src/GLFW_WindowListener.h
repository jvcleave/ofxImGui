#pragma once

#include "ofMain.h"
#include "imgui.h"
#include "ofAppGLFWWindow.h"

// GLFW
#ifdef _WIN32
    #undef APIENTRY
    #define GLFW_EXPOSE_NATIVE_WIN32
    #define GLFW_EXPOSE_NATIVE_WGL
    #include <GLFW/glfw3native.h>
#endif

class GLFW_WindowListener
{
public:
    GLFW_WindowListener();
    GLFWwindow*  glfwWindow;
    
    void onKeyPressed(ofKeyEventArgs& event);
    void onKeyReleased(ofKeyEventArgs& event);
    void onMousePressed(ofMouseEventArgs& event);
    void onMouseScrolled(ofMouseEventArgs& event);
    
    
    float mouseWheel;
    
    void updateFrame();
    double g_Time;
    bool g_MousePressed[3];
    
    ImGuiIO* io;
};
