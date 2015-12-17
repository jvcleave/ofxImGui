#pragma once

#include "ofMain.h"
#include "imgui.h"

#include "BaseTheme.h"

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
    void setup(BaseTheme* theme=NULL);
    void begin();
    void end();
    void close();
    
    void openThemeColorWindow();
    GLuint loadImage(string imagePath);
    
    BaseTheme* theme;
    void setTheme(BaseTheme* theme);
    bool doCinderTheme;
private:

    float last_time;
    
#if defined(TARGET_OPENGLES)
    EngineOpenGLES* engine;
#else  
    EngineGLFW* engine;
#endif
    ImGuiIO* io;
};
