#pragma once
#if !defined(TARGET_RASPBERRY_PI)
#include "BaseEngine.h"

class EngineGLFW : public BaseEngine
{
public:
    
    EngineGLFW()
    {
        
    };
    
    //EngineInterface
    void setup(ImGuiIO*);
    
    void onKeyReleased(ofKeyEventArgs& event);

    bool createDeviceObjects();
   //void updateThemeColors();
    
 
    //custom 
    static void programmableRendererDrawLists(ImDrawData * draw_data);
    static void glRendererDrawLists(ImDrawData * draw_data);
    
    
    static unsigned int vaoHandle;

};
#endif