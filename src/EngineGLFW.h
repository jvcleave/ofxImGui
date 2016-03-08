#if !defined(TARGET_RASPBERRY_PI)
#pragma once
#include "BaseEngine.h"

class EngineGLFW : public BaseEngine
{
public: 
    ~EngineGLFW()
    {
        exit();
    }
    
    // BaseEngine required
    void setup() override;
    void exit() override;
    bool createDeviceObjects() override;
    void invalidateDeviceObjects() override;

    bool createFontsTexture();
    
    void onKeyReleased(ofKeyEventArgs& event) override;
    void onMousePressed(ofMouseEventArgs& event) override;
    void onMouseReleased(ofMouseEventArgs& event) override;
    
    // Custom 
    static void programmableRenderDrawLists(ImDrawData * draw_data);
    static void fixedRenderDrawLists(ImDrawData * draw_data);

    static GLuint g_FontTexture;
    static unsigned int g_VaoHandle;
};
#endif