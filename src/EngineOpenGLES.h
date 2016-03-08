#pragma once

#include "BaseEngine.h"

class EngineOpenGLES : public BaseEngine
{
public:
    ~EngineOpenGLES()
    {
        exit();
    }
    
    // BaseEngine required
    void setup() override;
    void exit() override;
    bool createDeviceObjects() override;
    void invalidateDeviceObjects() override;

    void onKeyReleased(ofKeyEventArgs& event) override;

    // Custom 
    static void rendererDrawLists(ImDrawData * draw_data);
    
    static ofShader g_Shader;
};