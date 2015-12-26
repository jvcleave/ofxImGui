#pragma once

#include "ofMain.h"
#include "imgui.h"

#include "BaseTheme.h"

#if defined(TARGET_OPENGLES)
    #include "EngineOpenGLES.h"
#else
    #include "EngineGLFW.h"
#endif


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
private:

    float lastTime;
    
#if defined(TARGET_OPENGLES)
    EngineOpenGLES* engine;
#else  
    EngineGLFW* engine;
#endif
    ImGuiIO* io;
};
