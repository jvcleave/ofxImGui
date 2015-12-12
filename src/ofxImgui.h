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

    ImVec4 col_main_text = ImVec4(0.86f, 0.93f, 0.89f, 0.f);
    ImVec4 col_main_back = ImVec4(0.82f, 0.18f, 0.29f, 0.f);
    ImVec4 col_win_inact = ImVec4(0.20f, 0.22f, 0.27f, 0.f);
    ImVec4 col_win_popup = ImVec4(0.47f, 0.77f, 0.83f, 0.f);
    ImVec4 col_win_backg = ImVec4(0.13f, 0.14f, 0.17f, 0.f);

  private:

    float last_time;

    ImGuiIO * io;
    ImGuiStyle * style;
};
