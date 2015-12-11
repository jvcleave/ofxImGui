#include "ofxImgui.h"

ofTexture ofxImgui::fontTexture;

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

  style->WindowMinSize            = ImVec2(160, 20);
  style->FramePadding             = ImVec2(4, 2);
  style->ItemSpacing              = ImVec2(6, 2);
  style->ItemInnerSpacing         = ImVec2(6, 4);
  style->Alpha                    = 1.5f;
  style->WindowFillAlphaDefault   = 1.0f;
  style->WindowRounding           = 0.0f;
  style->FrameRounding            = 0.0f;
  style->IndentSpacing            = 6.0f;
  style->ItemInnerSpacing         = ImVec2(2, 4);
  style->ColumnsMinSpacing        = 50.0f;
  style->GrabMinSize              = 14.0f;
  style->GrabRounding             = 0.0f;
  style->ScrollbarSize            = 12.0f;
  style->ScrollbarRounding        = 0.0f;

  style->Colors[ImGuiCol_Text]                  = ImVec4(0.86f, 0.93f, 0.89f, 0.61f);
  style->Colors[ImGuiCol_TextDisabled]          = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
  style->Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
  style->Colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.20f, 0.22f, 0.27f, 0.58f);
  style->Colors[ImGuiCol_Border]                = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
  style->Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  style->Colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
  style->Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
  style->Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_TitleBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
  style->Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
  style->Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
  style->Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
  style->Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.47f, 0.77f, 0.83f, 0.21f);
  style->Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
  style->Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_ComboBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
  style->Colors[ImGuiCol_CheckMark]             = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
  style->Colors[ImGuiCol_SliderGrab]            = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
  style->Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
  style->Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
  style->Colors[ImGuiCol_ButtonActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_Header]                = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
  style->Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
  style->Colors[ImGuiCol_HeaderActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_Column]                = ImVec4(0.47f, 0.77f, 0.83f, 0.32f);
  style->Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
  style->Colors[ImGuiCol_ColumnActive]          = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
  style->Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
  style->Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_CloseButton]           = ImVec4(0.86f, 0.93f, 0.89f, 0.16f);
  style->Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.86f, 0.93f, 0.89f, 0.39f);
  style->Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
  style->Colors[ImGuiCol_PlotLines]             = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
  style->Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
  style->Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
  style->Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
  style->Colors[ImGuiCol_TooltipBg]             = ImVec4(0.47f, 0.77f, 0.83f, 0.72f);
  style->Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);

  io->MouseDrawCursor = false;

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

  io->RenderDrawListsFn  = &ofxImgui::renderDrawLists;
  io->SetClipboardTextFn = &ofxImgui::setClipboardString;
  io->GetClipboardTextFn = &ofxImgui::getClipboardString;

  io->DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());

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

void ofxImgui::renderDrawLists(ImDrawData * draw_data)
{
  for (int n = 0; n < draw_data->CmdListsCount; n++)
  {
    ofMesh mesh;
    vector<ofIndexType> index;
    const ImDrawList * cmd_list = draw_data->CmdLists[n];
    for(size_t i = 0; i < cmd_list->VtxBuffer.size(); i++)
    {
      mesh.addVertex(ofVec3f(cmd_list->VtxBuffer[i].pos.x, cmd_list->VtxBuffer[i].pos.y, 0));
      mesh.addTexCoord(ofVec2f(cmd_list->VtxBuffer[i].uv.x, cmd_list->VtxBuffer[i].uv.y));
      ImColor imColor(cmd_list->VtxBuffer[i].col);
      mesh.addColor(ofFloatColor(imColor.Value.x, imColor.Value.y, imColor.Value.z, imColor.Value.w));
    }

    for(size_t i = 0; i < cmd_list->IdxBuffer.size(); i++)
      mesh.addIndex((ofIndexType)cmd_list->IdxBuffer[i]);

    ofxImgui::fontTexture.bind();
    mesh.draw();
    ofxImgui::fontTexture.unbind();
  }
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
  unsigned char * pixels;
  int width, height;

  GLuint externalTexture;
  glGenTextures(1, &externalTexture);
  glBindTexture(GL_TEXTURE_2D, externalTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if(ofIsGLProgrammableRenderer())
  {
    io->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
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
  } else {
    io->Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_ALPHA,
      width, height,
      0,
      GL_ALPHA,
      GL_UNSIGNED_BYTE,
      pixels
    );
  }
  ofxImgui::fontTexture.texData.textureTarget = GL_TEXTURE_2D;
  ofxImgui::fontTexture.setUseExternalTextureID(externalTexture);

  io->Fonts->TexID = (void *)(intptr_t)ofxImgui::fontTexture.getTextureData().textureID;

  io->Fonts->ClearInputData();
  io->Fonts->ClearTexData();

  return true;
}

void ofxImgui::begin()
{
  float current_time = ofGetElapsedTimef();
  io->DeltaTime = last_time > 0.f ? current_time - last_time : 1.f / 60.f;
  last_time = current_time;

  io->MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());

  ImGui::NewFrame();
}

void ofxImgui::end()
{
  ImGui::Render();
}

ofxImgui::~ofxImgui()
{
  ImGui::GetIO().Fonts->TexID = 0;
  io = NULL;
  ImGui::Shutdown();
}
