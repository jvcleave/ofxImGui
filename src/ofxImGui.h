// ImGui GLFW binding with OpenGL
// https://github.com/ocornut/imgui

#include "ofMain.h"
#include "imgui.h"
#include "ofAppGLFWWindow.h"


class ofxImGui
{
public:
    
    ofxImGui();
    bool        setup(bool install_callbacks);
    void        Shutdown();
    void        NewFrame();
    
    // Use if you want to reset your rendering device without losing ImGui state.
    void        InvalidateDeviceObjects();
    bool        CreateDeviceObjects();
    
    // GLFW callbacks (installed by default if you enable 'install_callbacks' during initialization)
    // Provided here if you want to chain callbacks.
    // You can also handle inputs yourself and use those as a reference.
    static void        MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void        ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void        KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void        CharCallback(GLFWwindow* window, unsigned int c);


    static const char* GetClipboardText();
    static void  SetClipboardText(const char* text);
    static void  RenderDrawLists(ImDrawData* draw_data);
    
    // Data
    static GLFWwindow*  glfwWindow;
    double g_Time;
    static bool g_MousePressed[3];
    static float g_MouseWheel;
   // GLuint g_FontTexture;
    
    ofTextureData textureData;
    ofTexture g_FontTexture;
};
