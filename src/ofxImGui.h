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
    GLuint loadImage(ofImage& image);
    GLuint loadImage(string imagePath);
    
    GLuint loadPixels(string imagePath);
    GLuint loadPixels(ofPixels& pixels);
    
    GLuint loadTexture(string imagePath);
    GLuint loadTexture(ofTexture& texture, string imagePath);
    
    BaseTheme* theme;
    void setTheme(BaseTheme* theme);

    float lastTime;
    vector<ofTexture*> loadedTextures;
    
#if defined(TARGET_OPENGLES)
    EngineOpenGLES* engine;
#else  
    EngineGLFW* engine;
#endif
};
