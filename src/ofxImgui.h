#pragma once

#include "ofMain.h"
#include "imgui.h"

namespace ui = ImGui;

class ofxImgui
{
  public:

    ofxImgui();
   ~ofxImgui();

    void setup(ofEventArgs&);
    void begin();
    void end();

    void onKeyPressed(ofKeyEventArgs& event);
    void onKeyReleased(ofKeyEventArgs& event);
    void onMousePressed(ofMouseEventArgs& event);
    void onMouseReleased(ofMouseEventArgs& event);
    void onMouseScrolled(ofMouseEventArgs& event);
    void onWindowResized(ofResizeEventArgs& window);

    bool createDeviceObjects();

    GLuint loadTextureImage2D(ofImage & image);
    GLuint loadTextureImage2D(unsigned char * pixels, int width, int height);

    static const char * getClipboardString();
    static void setClipboardString(const char * text);

    static void programmableRendererDrawLists(ImDrawData * draw_data);
    static void glRendererDrawLists(ImDrawData * draw_data);

    static ofTexture fontTexture;
    static unsigned int vboHandle, vaoHandle, elementsHandle;
    static int attribLocationColor;
    static ofShader shader;
    static ofVbo vbo;

  private:

    float last_time;

    ImGuiIO * io;
    ImGuiStyle * style;
};
