// ImGui GLFW binding with OpenGL
// https://github.com/ocornut/imgui


#include "ofxImGui.h"

// GLFW
#include <GLFW/glfw3.h>
#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif

// Data
static GLFWwindow*  g_Window = NULL;
static double       g_Time = 0.0f;
static bool         g_MousePressed[3] = { false, false, false };
static float        g_MouseWheel = 0.0f;
static GLuint       g_FontTexture = 0;

// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
static void ImGui_ImplGlfw_RenderDrawLists(ImDrawData* draw_data)
{
    // We are using the OpenGL fixed pipeline to make the example code simpler to read!
    // A probable faster way to render would be to collate all vertices from all cmd_lists into a single vertex buffer.
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context

    // Setup orthographic projection matrix
    const float width = ImGui::GetIO().DisplaySize.x;
    const float height = ImGui::GetIO().DisplaySize.y;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, -1.0f, +1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    
    ofMesh mesh;
    
    // Render command lists
    #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
        const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();
        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
    #undef OFFSETOF

    // Restore modified state
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindTexture(GL_TEXTURE_2D, 0);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}

static const char* ImGui_ImplGlfw_GetClipboardText()
{
    ofAppGLFWWindow* baseWindow = (ofAppGLFWWindow*)ofGetWindowPtr();
    
    return baseWindow->getClipboardString().c_str();
}

static void ImGui_ImplGlfw_SetClipboardText(const char* text)
{
    glfwSetClipboardString(g_Window, text);
}

void ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/)
{
    if (action == GLFW_PRESS && button >= 0 && button < 3)
        g_MousePressed[button] = true;
}

void ImGui_ImplGlfw_ScrollCallback(GLFWwindow*, double /*xoffset*/, double yoffset)
{
    g_MouseWheel += (float)yoffset; // Use fractional mouse wheel, 1.0 unit 5 lines.
}

void ImGui_ImplGlFw_KeyCallback(GLFWwindow*, int key, int, int action, int mods)
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

void ImGui_ImplGlfw_CharCallback(GLFWwindow*, unsigned int c)
{
    ImGuiIO& io = ImGui::GetIO();
    if (c > 0 && c < 0x10000)
        io.AddInputCharacter((unsigned short)c);
}

bool ImGui_ImplGlfw_CreateDeviceObjects()
{
    ImGuiIO& io = ImGui::GetIO();

    // Build texture
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

    // Create texture
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);

    // Store our identifier
    io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

    // Cleanup (don't clear the input data if you want to append new fonts later)
    io.Fonts->ClearInputData();
    io.Fonts->ClearTexData();

    return true;
}

void    ImGui_ImplGlfw_InvalidateDeviceObjects()
{
    if (g_FontTexture)
    {
        glDeleteTextures(1, &g_FontTexture);
        ImGui::GetIO().Fonts->TexID = 0;
        g_FontTexture = 0;
    }
}

bool    ImGui_ImplGlfw_Init(bool install_callbacks)
{
    ofAppGLFWWindow* baseWindow = (ofAppGLFWWindow*)ofGetWindowPtr();
    
    g_Window = baseWindow->getGLFWWindow();

    ImGuiIO& io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = OF_KEY_TAB;                 // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_LeftArrow] = OF_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = OF_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = OF_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = OF_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = OF_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = OF_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = OF_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = OF_KEY_END;
    io.KeyMap[ImGuiKey_Delete] = OF_KEY_DEL;
    io.KeyMap[ImGuiKey_Backspace] = OF_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = OF_KEY_RETURN;
    io.KeyMap[ImGuiKey_Escape] = OF_KEY_ESC;
    io.RenderDrawListsFn = ImGui_ImplGlfw_RenderDrawLists;
    io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;
#ifdef _WIN32
    io.ImeWindowHandle = glfwGetWin32Window(g_Window);
#endif

    if (install_callbacks)
    {
        glfwSetMouseButtonCallback(g_Window, ImGui_ImplGlfw_MouseButtonCallback);
        glfwSetScrollCallback(g_Window, ImGui_ImplGlfw_ScrollCallback);
        glfwSetKeyCallback(g_Window, (GLFWkeyfun)ImGui_ImplGlFw_KeyCallback);
        glfwSetCharCallback(g_Window, ImGui_ImplGlfw_CharCallback);
    }

    return true;
}

void ImGui_ImplGlfw_Shutdown()
{
    ImGui_ImplGlfw_InvalidateDeviceObjects();
    ImGui::Shutdown();
}

void ImGui_ImplGlfw_NewFrame()
{
    if (!g_FontTexture)
        ImGui_ImplGlfw_CreateDeviceObjects();

    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    //int w, h;
    int w = ofGetWindowWidth();
    int h = ofGetWindowHeight();
    io.DisplaySize = ImVec2((float)w, (float)h);
    
    
    
    // Setup time step
    double current_time =  glfwGetTime();
    io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f/60.0f);
    g_Time = current_time;

    // Setup inputs
    // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
    if (glfwGetWindowAttrib(g_Window, GLFW_FOCUSED))
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
        io.MouseDown[i] = g_MousePressed[i] || glfwGetMouseButton(g_Window, i) != 0;    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        g_MousePressed[i] = false;
    }

    io.MouseWheel = g_MouseWheel;
    g_MouseWheel = 0.0f;
    
    
    // Hide OS mouse cursor if ImGui is drawing it
    if(io.MouseDrawCursor) ofHideCursor();
    
    // Start the frame
    ImGui::NewFrame();
}
