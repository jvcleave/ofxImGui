#pragma once
#include "ofMain.h"
#include "imgui.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

class BaseEngine
{
public:
    
    BaseEngine()
    {
        io = NULL;
        style = NULL;
    }
    
    ImGuiIO* io;
    ImGuiStyle* style;
    
    
    virtual void setup(ImGuiIO*)=0;
    virtual bool createDeviceObjects()=0;
    
    virtual void onKeyReleased(ofKeyEventArgs& event)=0;
    
    
    void onKeyPressed(ofKeyEventArgs& event);
    void onMousePressed(ofMouseEventArgs& event);
    void onMouseReleased(ofMouseEventArgs& event);
    void onMouseScrolled(ofMouseEventArgs& event);
    void onWindowResized(ofResizeEventArgs& window);

    GLuint loadTextureImage2D(ofImage & image);
    GLuint loadTextureImage2D(unsigned char * pixels, int width, int height);
    
    
    static const char* getClipboardString();
    static void setClipboardString(const char * text);
};

