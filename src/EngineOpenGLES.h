#pragma once

#if defined(TARGET_OPENGLES)
#include "BaseEngine.h"

class EngineOpenGLES : public BaseEngine
{
public:
    
    EngineOpenGLES()
    {
        
    };
    //EngineInterface
    void setup(ImGuiIO*);

    
    void onKeyPressed(ofKeyEventArgs& event);
    void onKeyReleased(ofKeyEventArgs& event);
    void onMousePressed(ofMouseEventArgs& event);
    void onMouseReleased(ofMouseEventArgs& event);
    void onMouseScrolled(ofMouseEventArgs& event);
    void onWindowResized(ofResizeEventArgs& window);
    
    
    bool createDeviceObjects();
    
    
    //custom 

    static void rendererDrawLists(ImDrawData * draw_data);
    
    static unsigned int vboHandle;
    static unsigned int vaoHandle;
    static unsigned int elementsHandle;
    static int attribLocationColor;
    static ofShader shader;
    
    static int  g_ShaderHandle;
    static int  g_AttribLocationTex;
    static int  g_AttribLocationProjMtx;
    static int  g_AttribLocationPosition;
    static int  g_AttribLocationUV; 
    
};
#endif