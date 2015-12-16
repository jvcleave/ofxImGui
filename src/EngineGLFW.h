#if !defined(TARGET_RASPBERRY_PI)
#pragma once
#include "BaseEngine.h"

class EngineGLFW : public BaseEngine
{
public:
    
    EngineGLFW()
    {
        
    };
    
    //BaseEngine required
    void setup(ImGuiIO*);
    bool createDeviceObjects();
    void onKeyReleased(ofKeyEventArgs& event);
    
    //custom 
    static void programmableRendererDrawLists(ImDrawData * draw_data);
    static void glRendererDrawLists(ImDrawData * draw_data);
    
    static unsigned int vaoHandle;

};
#endif