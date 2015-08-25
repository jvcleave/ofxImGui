#pragma once

// ImGui GLFW binding with OpenGL
// https://github.com/ocornut/imgui

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
#ifndef TARGET_OPENGLES

    static void  renderDrawLists(ImDrawData* draw_data);
#else
	static int g_ShaderHandle;
   	static int g_AttribLocationTex;
   	static int g_AttribLocationProjMtx;
   	static unsigned int g_VaoHandle;
   	static unsigned int g_VboHandle;
	static unsigned int g_ElementsHandle;

   	
    static void  renderDrawLists_GLES(ImDrawData* draw_data);
#endif

    
   
    //static vector<ofMesh>* meshes;

};
