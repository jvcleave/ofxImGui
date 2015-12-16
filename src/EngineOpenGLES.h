#pragma once

#include "BaseEngine.h"

class EngineOpenGLES : public BaseEngine
{
public:
    
    EngineOpenGLES()
    {
        
    };
    
    //BaseEngine required
    void setup(ImGuiIO*);
    bool createDeviceObjects();
    void onKeyReleased(ofKeyEventArgs& event);

    //custom 
    static void rendererDrawLists(ImDrawData * draw_data);
    
    static int  g_ShaderHandle;
    static int  g_AttribLocationTex;
    static int  g_AttribLocationProjMtx;
    static int  g_AttribLocationPosition;
    static int  g_AttribLocationUV; 
    
};