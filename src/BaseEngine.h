#pragma once
#include "ofMain.h"
#include "imgui.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

class BaseEngine
{
public:
    BaseEngine()
        : isSetup(false)
    {}

    virtual ~BaseEngine()
    {}    
    
    virtual void setup()=0;
    virtual void exit()=0;
    virtual bool createDeviceObjects()=0;
    virtual void invalidateDeviceObjects()=0;

    virtual void onMousePressed(ofMouseEventArgs& event);
    virtual void onMouseReleased(ofMouseEventArgs& event);
    virtual void onMouseScrolled(ofMouseEventArgs& event);
    virtual void onKeyPressed(ofKeyEventArgs& event);
    virtual void onKeyReleased(ofKeyEventArgs& event)=0;
    virtual void onWindowResized(ofResizeEventArgs& window);

    GLuint loadTextureImage2D(unsigned char * pixels, int width, int height);
    
    static const char* getClipboardString();
    static void setClipboardString(const char * text);

    static int g_ShaderHandle;
    static int g_VertHandle;
    static int g_FragHandle;

    static int g_AttribLocationTex;
    static int g_AttribLocationProjMtx;
    static int g_AttribLocationPosition;
    static int g_AttribLocationUV;
    static int g_AttribLocationColor;

    static unsigned int g_VboHandle;
    static unsigned int g_ElementsHandle;

protected:
    bool isSetup;
};

