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
    }
    virtual ~BaseEngine()
    {
        io = NULL;
    }
    
    ImGuiIO* io; 
    
    
    virtual void setup(ImGuiIO*)=0;
    virtual bool createDeviceObjects()=0;
    
    virtual void onKeyReleased(ofKeyEventArgs& event)=0;
    
    
    void onKeyPressed(ofKeyEventArgs& event);
    void onMousePressed(ofMouseEventArgs& event);
    void onMouseReleased(ofMouseEventArgs& event);
    void onMouseScrolled(ofMouseEventArgs& event);
    void onWindowResized(ofResizeEventArgs& window);

    GLuint loadTextureImage2D(unsigned char * pixels, int width, int height);
    GLuint loadTextureImage2D(ofPixels& pixels);

    
    static const char* getClipboardString();
    static void setClipboardString(const char * text);
    
    static unsigned int vboHandle;
    static unsigned int elementsHandle;
    static int attribLocationColor;
    static ofShader shader;
};

