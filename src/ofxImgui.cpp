#include "ofxImgui.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

ofTexture ofxImgui::fontTexture;
unsigned int ofxImgui::vboHandle = 0, ofxImgui::vaoHandle = 0, ofxImgui::elementsHandle = 0;
int ofxImgui::attribLocationColor = 0;
ofShader ofxImgui::shader;
ofVbo ofxImgui::vbo;

#ifdef TARGET_OPENGLES
#include <dlfcn.h>
typedef void (* glGenVertexArraysType) (GLsizei n, GLuint *arrays);
glGenVertexArraysType glGenVertexArraysFuncAlt = nullptr;
#define glGenVertexArrays glGenVertexArraysFuncAlt

typedef void (* glDeleteVertexArraysType) (GLsizei n, GLuint *arrays);
glDeleteVertexArraysType glDeleteVertexArraysFuncAlt = nullptr;
#define glDeleteVertexArrays glDeleteVertexArraysFuncAlt

typedef void (* glBindVertexArrayType) (GLuint array);
glBindVertexArrayType glBindVertexArrayFuncAlt = nullptr;
#define glBindVertexArray glBindVertexArrayFuncAlt
#endif

ofxImgui::ofxImgui()
:last_time(0.f)
{
  ofAddListener(ofEvents().setup, this, &ofxImgui::setup);
  ofAddListener(ofEvents().keyPressed, this, &ofxImgui::onKeyPressed);
  ofAddListener(ofEvents().keyReleased, this, &ofxImgui::onKeyReleased);
  ofAddListener(ofEvents().mousePressed, this, &ofxImgui::onMousePressed);
  ofAddListener(ofEvents().mouseReleased, this, &ofxImgui::onMouseReleased);
  ofAddListener(ofEvents().mouseScrolled, this, &ofxImgui::onMouseScrolled);
  ofAddListener(ofEvents().windowResized, this, &ofxImgui::onWindowResized);
}

void ofxImgui::setup(ofEventArgs&)
{
  io = &ui::GetIO();
  style = &ui::GetStyle();

  style->WindowMinSize           = ImVec2(160, 65);
  style->FramePadding            = ImVec2(4, 2);
  style->ItemSpacing             = ImVec2(6, 2);
  style->ItemInnerSpacing        = ImVec2(6, 4);
  style->Alpha                   = 1.5f;
  style->WindowFillAlphaDefault  = 1.0f;
  style->WindowRounding          = 0.0f;
  style->FrameRounding           = 0.0f;
  style->IndentSpacing           = 6.0f;
  style->ItemInnerSpacing        = ImVec2(2, 4);
  style->ColumnsMinSpacing       = 50.0f;
  style->GrabMinSize             = 14.0f;
  style->GrabRounding            = 0.0f;
  style->ScrollbarSize           = 12.0f;
  style->ScrollbarRounding       = 0.0f;

  style->Colors[ImGuiCol_Text]                  = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.61f);
  style->Colors[ImGuiCol_TextDisabled]          = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.28f);
  style->Colors[ImGuiCol_WindowBg]              = ImVec4(col_win_backg.r / 255.f, col_win_backg.g / 255.f, col_win_backg.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_ChildWindowBg]         = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 0.58f);
  style->Colors[ImGuiCol_Border]                = ImVec4(col_win_backg.r / 255.f, col_win_backg.g / 255.f, col_win_backg.b / 255.f, 0.00f);
  style->Colors[ImGuiCol_BorderShadow]          = ImVec4(col_win_backg.r / 255.f, col_win_backg.g / 255.f, col_win_backg.b / 255.f, 0.00f);
  style->Colors[ImGuiCol_FrameBg]               = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_FrameBgHovered]        = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.78f);
  style->Colors[ImGuiCol_FrameBgActive]         = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_TitleBg]               = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 0.75f);
  style->Colors[ImGuiCol_TitleBgActive]         = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_MenuBarBg]             = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 0.47f);
  style->Colors[ImGuiCol_ScrollbarBg]           = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.21f);
  style->Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.78f);
  style->Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_ComboBg]               = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_CheckMark]             = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.80f);
  style->Colors[ImGuiCol_SliderGrab]            = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.14f);
  style->Colors[ImGuiCol_SliderGrabActive]      = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_Button]                = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.14f);
  style->Colors[ImGuiCol_ButtonHovered]         = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.86f);
  style->Colors[ImGuiCol_ButtonActive]          = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_Header]                = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.76f);
  style->Colors[ImGuiCol_HeaderHovered]         = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.86f);
  style->Colors[ImGuiCol_HeaderActive]          = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_Column]                = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.32f);
  style->Colors[ImGuiCol_ColumnHovered]         = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.78f);
  style->Colors[ImGuiCol_ColumnActive]          = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_ResizeGrip]            = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.04f);
  style->Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.78f);
  style->Colors[ImGuiCol_ResizeGripActive]      = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_CloseButton]           = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.16f);
  style->Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.39f);
  style->Colors[ImGuiCol_CloseButtonActive]     = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_PlotLines]             = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.63f);
  style->Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_PlotHistogram]         = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.63f);
  style->Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_TextSelectedBg]        = ImVec4(col_main_head.r / 255.f, col_main_head.g / 255.f, col_main_head.b / 255.f, 0.43f);
  style->Colors[ImGuiCol_TooltipBg]             = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.72f);
  style->Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 0.73f);

  io->KeyMap[ImGuiKey_Tab]        = GLFW_KEY_TAB;
  io->KeyMap[ImGuiKey_LeftArrow]  = GLFW_KEY_LEFT;
  io->KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
  io->KeyMap[ImGuiKey_UpArrow]    = GLFW_KEY_UP;
  io->KeyMap[ImGuiKey_DownArrow]  = GLFW_KEY_DOWN;
  io->KeyMap[ImGuiKey_PageUp]     = GLFW_KEY_PAGE_UP;
  io->KeyMap[ImGuiKey_PageDown]   = GLFW_KEY_PAGE_DOWN;
  io->KeyMap[ImGuiKey_Home]       = GLFW_KEY_HOME;
  io->KeyMap[ImGuiKey_End]        = GLFW_KEY_END;
  io->KeyMap[ImGuiKey_Delete]     = GLFW_KEY_DELETE;
  io->KeyMap[ImGuiKey_Backspace]  = GLFW_KEY_BACKSPACE;
  io->KeyMap[ImGuiKey_Enter]      = GLFW_KEY_ENTER;
  io->KeyMap[ImGuiKey_Escape]     = GLFW_KEY_ESCAPE;
  io->KeyMap[ImGuiKey_A]          = GLFW_KEY_A;
  io->KeyMap[ImGuiKey_C]          = GLFW_KEY_C;
  io->KeyMap[ImGuiKey_V]          = GLFW_KEY_V;
  io->KeyMap[ImGuiKey_X]          = GLFW_KEY_X;
  io->KeyMap[ImGuiKey_Y]          = GLFW_KEY_Y;
  io->KeyMap[ImGuiKey_Z]          = GLFW_KEY_Z;

  io->RenderDrawListsFn = ofIsGLProgrammableRenderer() ?
    &ofxImgui::programmableRendererDrawLists :
    &ofxImgui::glRendererDrawLists
  ;

  io->SetClipboardTextFn = &ofxImgui::setClipboardString;
  io->GetClipboardTextFn = &ofxImgui::getClipboardString;

  io->DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());

  io->MouseDrawCursor = false;

  createDeviceObjects();
}

void ofxImgui::onKeyPressed(ofKeyEventArgs& event)
{
  int key = event.keycode;

  io->KeysDown[key] = true;

  io->KeyCtrl  = io->KeysDown[GLFW_KEY_LEFT_CONTROL] || io->KeysDown[GLFW_KEY_RIGHT_CONTROL];
  io->KeyShift = io->KeysDown[GLFW_KEY_LEFT_SHIFT]   || io->KeysDown[GLFW_KEY_RIGHT_SHIFT];
  io->KeyAlt   = io->KeysDown[GLFW_KEY_LEFT_ALT]     || io->KeysDown[GLFW_KEY_RIGHT_ALT];

  if(key < GLFW_KEY_ESCAPE)
    io->AddInputCharacter((unsigned short)event.codepoint);
}

void ofxImgui::onKeyReleased(ofKeyEventArgs& event)
{
  int key = event.keycode;

  io->KeysDown[key] = false;

  io->KeyCtrl  = io->KeysDown[GLFW_KEY_LEFT_CONTROL] || io->KeysDown[GLFW_KEY_RIGHT_CONTROL];
  io->KeyShift = io->KeysDown[GLFW_KEY_LEFT_SHIFT]   || io->KeysDown[GLFW_KEY_RIGHT_SHIFT];
  io->KeyAlt   = io->KeysDown[GLFW_KEY_LEFT_ALT]     || io->KeysDown[GLFW_KEY_RIGHT_ALT];
}

void ofxImgui::onMousePressed(ofMouseEventArgs& event)
{
  if(event.button >= 0 && event.button < 5)
    io->MouseDown[event.button] = true;
}

void ofxImgui::onMouseReleased(ofMouseEventArgs& event)
{
  if(event.button >= 0 && event.button < 5)
    io->MouseDown[event.button] = false;
}

void ofxImgui::onMouseScrolled(ofMouseEventArgs& event)
{
  io->MouseWheel = event.scrollY;
}

void ofxImgui::onWindowResized(ofResizeEventArgs& window)
{
  io->DisplaySize = ImVec2((float)window.width, (float)window.height);
}

void ofxImgui::glRendererDrawLists(ImDrawData * draw_data)
{
#ifndef TARGET_OPENGLES
  GLint last_blend_src;
  GLint last_blend_dst;
  GLint last_blend_equation_rgb;
  GLint last_blend_equation_alpha;

  glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
  glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
  glGetIntegerv(GL_BLEND_EQUATION_RGB,   &last_blend_equation_rgb);
  glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);

  GLboolean last_enable_blend        = glIsEnabled(GL_BLEND);
  GLboolean last_enable_cull_face    = glIsEnabled(GL_CULL_FACE);
  GLboolean last_enable_depth_test   = glIsEnabled(GL_DEPTH_TEST);
  GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  ImGuiIO * io = &ui::GetIO();
  float fb_height = io->DisplaySize.y * io->DisplayFramebufferScale.y;
  draw_data->ScaleClipRects(io->DisplayFramebufferScale);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0f, io->DisplaySize.x, io->DisplaySize.y, 0.0f, -1.0f, +1.0f);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  for(int n = 0; n < draw_data->CmdListsCount; n++)
  {
    const ImDrawList * cmd_list = draw_data->CmdLists[n];
    const unsigned char * vtx_buffer = (const unsigned char *)&cmd_list->VtxBuffer.front();
    const ImDrawIdx * idx_buffer = &cmd_list->IdxBuffer.front();

    glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void *)(vtx_buffer + OFFSETOF(ImDrawVert, pos)));
    glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void *)(vtx_buffer + OFFSETOF(ImDrawVert, uv)));
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void *)(vtx_buffer + OFFSETOF(ImDrawVert, col)));

    for(int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
    {
      const ImDrawCmd * pcmd = &cmd_list->CmdBuffer[cmd_i];
      if(pcmd->UserCallback)
      {
          pcmd->UserCallback(cmd_list, pcmd);
      } else {
          ofTexture tex;
          tex.texData.textureTarget = GL_TEXTURE_2D;
          tex.setUseExternalTextureID((intptr_t)pcmd->TextureId);
          tex.bind();
          glScissor(
            (int)pcmd->ClipRect.x,
            (int)(fb_height - pcmd->ClipRect.w),
            (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
            (int)(pcmd->ClipRect.w - pcmd->ClipRect.y)
          );
          glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_INT, idx_buffer);
          tex.unbind();
      }
      idx_buffer += pcmd->ElemCount;
    }
  }

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
  glBlendFunc(last_blend_src, last_blend_dst);

  last_enable_blend        ? glEnable(GL_BLEND)        : glDisable(GL_BLEND);
  last_enable_cull_face    ? glEnable(GL_CULL_FACE)    : glDisable(GL_CULL_FACE);
  last_enable_depth_test   ? glEnable(GL_DEPTH_TEST)   : glDisable(GL_DEPTH_TEST);
  last_enable_scissor_test ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();
#endif
}

void ofxImgui::programmableRendererDrawLists(ImDrawData * draw_data)
{
  GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

  glEnable(GL_SCISSOR_TEST);

  shader.begin();

  float width = ofGetWidth();
  float height = ofGetHeight();
  ofMatrix4x4 ortho_projection(
    2.f / width,  0.f,            0.f,  0.f,
    0.f,          2.f / -height,  0.f,  0.f,
    0.f,          0.f,           -1.f,  0.f,
   -1.f,          1.f,            0.f,  1.f
  );
  shader.setUniformMatrix4f("ProjMat", ortho_projection, 1);

  glBindVertexArray(vaoHandle);

  for(int n = 0; n < draw_data->CmdListsCount; n++)
  {
    const ImDrawList * cmd_list = draw_data->CmdLists[n];
    const ImDrawIdx * idx_buffer_offset = 0;

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(
      GL_ARRAY_BUFFER,
      (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert),
      (GLvoid *)&cmd_list->VtxBuffer.front(),
      GL_STREAM_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsHandle);
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx),
      (GLvoid *)&cmd_list->IdxBuffer.front(),
      GL_STREAM_DRAW
    );

    for(const ImDrawCmd * pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
    {
      if(pcmd->UserCallback)
      {
          pcmd->UserCallback(cmd_list, pcmd);
      } else {
          ofTexture tex;
          tex.texData.textureTarget = GL_TEXTURE_2D;
          tex.setUseExternalTextureID((intptr_t)pcmd->TextureId);
          tex.bind();
          glScissor(
            (int)pcmd->ClipRect.x,
            (int)(height - pcmd->ClipRect.w),
            (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
            (int)(pcmd->ClipRect.w - pcmd->ClipRect.y)
          );
          glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_INT, idx_buffer_offset);
          tex.unbind();
      }
      idx_buffer_offset += pcmd->ElemCount;
    }
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  shader.end();

  last_enable_scissor_test ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
}

const char * ofxImgui::getClipboardString()
{
  return &ofGetWindowPtr()->getClipboardString()[0];
}

void ofxImgui::setClipboardString(const char * text)
{
  ofGetWindowPtr()->setClipboardString(text);
}

bool ofxImgui::createDeviceObjects()
{
  if(ofIsGLProgrammableRenderer())
  {

#ifndef TARGET_OPENGLES

    string header = "#version 330\n";

    string vertex_shader = header + R"(

      uniform mat4 ProjMat;

      in vec2 Position;
      in vec2 UV;
      in vec4 Color;

      out vec2 Frag_UV;
      out vec4 Frag_Color;

      void main()
      {
        Frag_UV = UV;
        Frag_Color = Color;
        gl_Position = ProjMat * vec4(Position.xy, 0, 1);
      }

    )";

    string fragment_shader = header + R"(

      uniform sampler2D Texture;

      in vec2 Frag_UV;
      in vec4 Frag_Color;

      out vec4 Out_Color;

      void main()
      {
        Out_Color = Frag_Color * texture( Texture, Frag_UV.st);
      }

    )";

#else

    string header = "";

    string vertex_shader = header + R"(

      uniform mat4 ProjMat;

      in vec2 Position;
      in vec2 UV;
      in vec4 Color;

      out vec2 Frag_UV;
      out vec4 Frag_Color;

      void main()
      {
        Frag_UV = UV;
        Frag_Color = Color;
        gl_Position = ProjMat * vec4(Position.xy, 0, 1);
      }

    )";

    string fragment_shader = header + R"(

      uniform sampler2D Texture;

      in vec2 Frag_UV;
      in vec4 Frag_Color;

      out vec4 Out_Color;

      void main()
      {
        Out_Color = Frag_Color * texture( Texture, Frag_UV.st);
      }

    )";

#endif

    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment_shader);
    shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex_shader);

    shader.linkProgram();

    glGenBuffers(1, &vboHandle);
    glGenBuffers(1, &elementsHandle);

    glGenBuffers(1, &vboHandle);
    glGenBuffers(1, &elementsHandle);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);

    shader.bindAttribute(ofShader::POSITION_ATTRIBUTE, "Position");
    shader.bindAttribute(ofShader::TEXCOORD_ATTRIBUTE, "UV");

    glEnableVertexAttribArray(shader.getAttributeLocation("Color"));
    shader.setAttribute2fv("Position", 0, sizeof(ImDrawVert));
    shader.setAttribute2fv("UV", (float*)8, sizeof(ImDrawVert));

    glVertexAttribPointer(
      attribLocationColor,
      4,
      GL_UNSIGNED_BYTE,
      GL_TRUE,
      sizeof(ImDrawVert),
      (float*)16
    );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
#ifdef TARGET_OPENGLES
  #undef glGenVertexArrays
  #undef glBindVertexArray
  #undef glDeleteVertexArrays
#endif

  unsigned char * pixels;
  int width, height;
  io->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

  GLuint textureid = loadTextureImage2D(pixels, width, height);
  io->Fonts->TexID = (ImTextureID)(intptr_t)textureid;

  io->Fonts->ClearTexData();

  return true;
}

GLuint ofxImgui::loadTextureImage2D(ofImage & image)
{
  return loadTextureImage2D(
    image.getPixels().getData(),
    image.getWidth(),
    image.getHeight()
  );
}

GLuint ofxImgui::loadTextureImage2D(unsigned char * pixels, int width, int height)
{
  GLint last_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

  GLuint new_texture;
  glGenTextures(1, &new_texture);
  glBindTexture(GL_TEXTURE_2D, new_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    width, height,
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    pixels
  );

  glBindTexture(GL_TEXTURE_2D, last_texture);

  return new_texture;
}

void ofxImgui::begin()
{
  float current_time = ofGetElapsedTimef();
  io->DeltaTime = last_time > 0.f ? current_time - last_time : 1.f / 60.f;
  last_time = current_time;

  io->MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());

  ui::NewFrame();
}

void ofxImgui::end()
{
  ui::Render();
}

ofxImgui::~ofxImgui()
{
  io->Fonts->TexID = 0;
  io = nullptr;
  style = nullptr;
  ui::Shutdown();
}
