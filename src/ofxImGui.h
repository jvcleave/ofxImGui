#pragma once

#include "ofMain.h"
#include "imgui.h"

#include "StyleCinder.h"

#if defined(TARGET_OPENGLES)
    #include "EngineOpenGLES.h"
#else
    #include "EngineGLFW.h"
#endif

namespace ui = ImGui;

class ofxImGui
{
public:

    ofxImGui();
    ~ofxImGui();
    void setup();
    void begin();
    void end();
    void close();
    
    GLuint loadImage(string imagePath);
    StyleCinder cinderStyle;
private:

    float last_time;
    
#if defined(TARGET_OPENGLES)
    EngineOpenGLES* engine;
#else  
    EngineGLFW* engine;
#endif
    ImGuiIO* io;
};
