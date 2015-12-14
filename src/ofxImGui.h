#pragma once

#include "ofMain.h"
#include "imgui.h"

namespace ui = ImGui;

class ofxImGui
{
  public:

    ofxImGui();
   ~ofxImGui();

    void setup();
    void begin();
    void end();

    void onKeyPressed(ofKeyEventArgs& event);
    void onKeyReleased(ofKeyEventArgs& event);
    void onMousePressed(ofMouseEventArgs& event);
    void onMouseReleased(ofMouseEventArgs& event);
    void onMouseScrolled(ofMouseEventArgs& event);
    void onWindowResized(ofResizeEventArgs& window);

    bool createDeviceObjects();
    void updateThemeColors();
    void themeColorsWindow(bool * p_opened);

    GLuint loadTextureImage2D(ofImage & image);
    GLuint loadTextureImage2D(unsigned char * pixels, int width, int height);

    static const char * getClipboardString();
    static void setClipboardString(const char * text);

    static void programmableRendererDrawLists(ImDrawData * draw_data);
    static void glRendererDrawLists(ImDrawData * draw_data);
    static void openGLES_RendererDrawLists(ImDrawData * draw_data);
    static ofTexture fontTexture;
    static unsigned int vboHandle;
    static unsigned int vaoHandle;
    static unsigned int elementsHandle;
    static int attribLocationColor;
    static ofShader shader;
    static ofVbo vbo;

    ofColor col_main_text = ofColor::fromHex(0xdbede2);
    ofColor col_main_head = ofColor::fromHex(0xd12d49);
    ofColor col_main_area = ofColor::fromHex(0x333844);
    ofColor col_win_popup = ofColor::fromHex(0x77c4d3);
    ofColor col_win_backg = ofColor::fromHex(0x21232b);

#if defined(TARGET_OPENGLES)
	static GLuint g_FontTexture;
    static double   g_Time;
    static int  g_ShaderHandle;
    static int  g_VertHandle;
    static int  g_FragHandle;
    static int  g_AttribLocationTex;
    static int  g_AttribLocationProjMtx;
    static int  g_AttribLocationPosition;
    static int  g_AttribLocationUV; 
    static int  g_AttribLocationColor;
    static unsigned int g_VboHandle;
    static unsigned int g_ElementsHandle;
#endif
  private:

    float last_time;

    ImGuiIO * io;
    ImGuiStyle * style;
};
