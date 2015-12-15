#pragma once
#if !defined(TARGET_OPENGLES)
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
    
    static unsigned int vboHandle;
    static unsigned int vaoHandle;
    static unsigned int elementsHandle;
    static int attribLocationColor;
    static ofShader shader;
};
#endif