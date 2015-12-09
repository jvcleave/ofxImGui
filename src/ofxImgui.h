#pragma once

#include "ofMain.h"
#include "imgui.h"

class ofxImgui
{
  public:

    ofxImgui();
   ~ofxImgui();

    static ofTexture fontTexture;

    void setup(ofEventArgs&);
    void begin();
    void end();

    void onKeyPressed(ofKeyEventArgs& event);
    void onKeyReleased(ofKeyEventArgs& event);
    void onMousePressed(ofMouseEventArgs& event);
    void onMouseScrolled(ofMouseEventArgs& event);

    void updateFrame();
    bool createDeviceObjects();

    static const char * getClipboardString();
    static void setClipboardString(const char * text);

    static void renderDrawLists(ImDrawData * draw_data);
    static ofFloatColor convertToFloatColor(ImU32 rgba);

    ImGuiIO * io;
    ImGuiStyle * style;
    double time;
    float mouseWheel;
    bool mousePressed[3];
};
