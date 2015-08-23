#include "GLFW_WindowListener.h"

bool        GLFW_WindowListener::g_MousePressed[3] = { false, false, false };
float       GLFW_WindowListener::g_MouseWheel = 0.0f;
GLFWwindow* GLFW_WindowListener::glfwWindow = NULL;


GLFW_WindowListener::GLFW_WindowListener()
{
    ofAppGLFWWindow* baseWindow = (ofAppGLFWWindow*)ofGetWindowPtr();
    GLFW_WindowListener::glfwWindow = baseWindow->getGLFWWindow();
    
    
#ifdef _WIN32
    io->ImeWindowHandle = glfwGetWin32Window(GLFW_WindowListener::glfwWindow);
#endif
    
    glfwSetMouseButtonCallback(GLFW_WindowListener::glfwWindow, MouseButtonCallback);
    glfwSetScrollCallback(GLFW_WindowListener::glfwWindow, ScrollCallback);
    glfwSetKeyCallback(GLFW_WindowListener::glfwWindow, (GLFWkeyfun)KeyCallback);
    glfwSetCharCallback(GLFW_WindowListener::glfwWindow, CharCallback);
    
    
}

void GLFW_WindowListener::MouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/)
{
    if (action == GLFW_PRESS && button >= 0 && button < 3)
    {
         GLFW_WindowListener::g_MousePressed[button] = true;
    }
       
}

void GLFW_WindowListener::ScrollCallback(GLFWwindow*, double /*xoffset*/, double yoffset)
{
    GLFW_WindowListener::g_MouseWheel += (float)yoffset; // Use fractional mouse wheel, 1.0 unit 5 lines.
}

void GLFW_WindowListener::KeyCallback(GLFWwindow*, int key, int, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    if (action == GLFW_PRESS)
        io.KeysDown[key] = true;
    if (action == GLFW_RELEASE)
        io.KeysDown[key] = false;
    
    (void)mods; // Modifiers are not reliable across systems
    io.KeyCtrl = io.KeysDown[OF_KEY_LEFT_CONTROL] || io.KeysDown[OF_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[OF_KEY_LEFT_SHIFT] || io.KeysDown[OF_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[OF_KEY_LEFT_ALT] || io.KeysDown[OF_KEY_RIGHT_ALT];
}

void GLFW_WindowListener::CharCallback(GLFWwindow*, unsigned int c)
{
    ImGuiIO& io = ImGui::GetIO();
    if (c > 0 && c < 0x10000)
    {
        io.AddInputCharacter((unsigned short)c);
    }
}

void GLFW_WindowListener::updateCurrentTime(double& g_Time)
{
    ImGuiIO& io = ImGui::GetIO();
    // Setup time step
    float current_time =  ofGetElapsedTimef();
    io.DeltaTime = g_Time > 0.0 ? (current_time - g_Time) : (1.0f/60.0f);
    g_Time =  current_time;
}

void GLFW_WindowListener::updateFrame()
{
    
    ImGuiIO& io = ImGui::GetIO();
    
    // Setup display size (every frame to accommodate for window resizing)
    //int w, h;
    int w = ofGetWindowWidth();
    int h = ofGetWindowHeight();
    io.DisplaySize = ImVec2((float)w, (float)h);
    
    if (glfwGetWindowAttrib(GLFW_WindowListener::glfwWindow, GLFW_FOCUSED))
    {
        double mouse_x, mouse_y;
        mouse_x = (double)ofGetMouseX();
        mouse_y = (double)ofGetMouseY();
        mouse_x *= (float)ofGetScreenWidth() / w;                        // Convert mouse coordinates to pixels
        mouse_y *= (float)ofGetScreenHeight() / h;
        io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);   // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
    }
    else
    {
        io.MousePos = ImVec2(-1,-1);
    }
    for (int i = 0; i < 3; i++)
    {
        io.MouseDown[i] = g_MousePressed[i] || glfwGetMouseButton(GLFW_WindowListener::glfwWindow, i) != 0;    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        g_MousePressed[i] = false;
    }
    
    io.MouseWheel = g_MouseWheel;
    g_MouseWheel = 0.0f;
    
    // Hide OS mouse cursor if ImGui is drawing it
    if(io.MouseDrawCursor) ofHideCursor();
    
}