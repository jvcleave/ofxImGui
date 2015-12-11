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
  io = &ImGui::GetIO();

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
  for (int n = 0; n <  draw_data->CmdListsCount; n++)
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
    glEnable(GL_SCISSOR_TEST);
    for(size_t i = 0; i < cmd_list->CmdBuffer.size(); i++)
    {
      const ImDrawCmd * pcmd = &cmd_list->CmdBuffer[i];
      if(pcmd->UserCallback)
      {
        pcmd->UserCallback(cmd_list, pcmd);
      } else {
        glScissor(
          (int)pcmd->ClipRect.x,
          (int)(ofGetHeight() - pcmd->ClipRect.w),
          (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
          (int)(pcmd->ClipRect.w - pcmd->ClipRect.y)
        );
        mesh.draw();
      }
    }
    glDisable(GL_SCISSOR_TEST);
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
