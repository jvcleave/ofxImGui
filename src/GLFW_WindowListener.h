#pragma once

#include "ofMain.h"
#include "imgui.h"
#include "ofAppGLFWWindow.h"
/*
// GLFW
#include <GLFW/glfw3.h>
#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif
*/
class GLFW_WindowListener
{
public:
    GLFW_WindowListener();
    static GLFWwindow*  glfwWindow;
    // GLFW callbacks (installed by default if you enable 'install_callbacks' during initialization)
    // Provided here if you want to chain callbacks.
    // You can also handle inputs yourself and use those as a reference.
    static void        MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void        ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    //static void        KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void        KeyCallback(GLFWwindow*, int, int, unsigned int, int, int);
    static void        CharCallback(GLFWwindow* window, unsigned int c);
    
    
    static bool g_MousePressed[3];
    static float g_MouseWheel;
    
    void updateFrame();
    double g_Time;

};
