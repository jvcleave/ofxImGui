#include "ofxImGui.h"

ofTexture ofxImGui::fontTexture;

ofxImGui::ofxImGui()
{
  time = 0.0f;
  mouseWheel = 0.0f;

  io = &ImGui::GetIO();

  ofAddListener(ofEvents().setup, this, &ofxImGui::setup);
  ofAddListener(ofEvents().keyPressed, this, &ofxImGui::onKeyPressed);
  ofAddListener(ofEvents().keyReleased, this, &ofxImGui::onKeyReleased);
  ofAddListener(ofEvents().mousePressed, this, &ofxImGui::onMousePressed);
  ofAddListener(ofEvents().mouseScrolled, this, &ofxImGui::onMouseScrolled);
}


void ofxImGui::setup(ofEventArgs&)
{   
  io->KeyMap[ImGuiKey_Tab]        = OF_KEY_TAB;
  io->KeyMap[ImGuiKey_LeftArrow]  = OF_KEY_LEFT;
  io->KeyMap[ImGuiKey_RightArrow] = OF_KEY_RIGHT;
  io->KeyMap[ImGuiKey_UpArrow]    = OF_KEY_UP;
  io->KeyMap[ImGuiKey_DownArrow]  = OF_KEY_DOWN;
  io->KeyMap[ImGuiKey_PageUp]     = OF_KEY_PAGE_UP;
  io->KeyMap[ImGuiKey_PageDown]   = OF_KEY_PAGE_DOWN;
  io->KeyMap[ImGuiKey_Home]       = OF_KEY_HOME;
  io->KeyMap[ImGuiKey_End]        = OF_KEY_END;
  io->KeyMap[ImGuiKey_Delete]     = OF_KEY_DEL;
  io->KeyMap[ImGuiKey_Backspace]  = OF_KEY_BACKSPACE;
  io->KeyMap[ImGuiKey_Enter]      = OF_KEY_RETURN;
  io->KeyMap[ImGuiKey_Escape]     = OF_KEY_ESC;

  io->RenderDrawListsFn  = &ofxImGui::renderDrawLists;
  io->SetClipboardTextFn =  ofxImGui::setClipboardString;
  io->GetClipboardTextFn = &ofxImGui::getClipboardString;
}

void ofxImGui::onKeyPressed(ofKeyEventArgs& event)
{
  io->KeysDown[event.key] = true;

  int key = event.key;

  if(key != OF_KEY_DEL &&
     key != OF_KEY_MODIFIER &&
     key != OF_KEY_RETURN &&
     key != OF_KEY_ESC &&
     key != OF_KEY_TAB &&
     key != OF_KEY_COMMAND &&
     key != OF_KEY_BACKSPACE &&
     key != OF_KEY_DEL &&
     key != OF_KEY_F1 &&
     key != OF_KEY_F2 &&
     key != OF_KEY_F3 &&
     key != OF_KEY_F4 &&
     key != OF_KEY_F5 &&
     key != OF_KEY_F6 &&
     key != OF_KEY_F7 &&
     key != OF_KEY_F8 &&
     key != OF_KEY_F9 &&
     key != OF_KEY_F10 &&
     key != OF_KEY_F11 &&
     key != OF_KEY_F12 &&
     key != OF_KEY_LEFT &&
     key != OF_KEY_UP &&
     key != OF_KEY_RIGHT &&
     key != OF_KEY_DOWN &&
     key != OF_KEY_PAGE_UP &&
     key != OF_KEY_PAGE_DOWN &&
     key != OF_KEY_HOME &&
     key != OF_KEY_END &&
     key != OF_KEY_INSERT &&
     key != OF_KEY_CONTROL &&
     key != OF_KEY_ALT &&
     key != OF_KEY_SHIFT &&
     key != OF_KEY_SUPER)
  {
    io->AddInputCharacter((unsigned short)event.codepoint);
  }
}

void ofxImGui::onKeyReleased(ofKeyEventArgs& event)
{
  io->KeysDown[event.key] = false;
}

void ofxImGui::onMousePressed(ofMouseEventArgs& event)
{
  if (event.button >= 0 && event.button < 3)
  {
    mousePressed[event.button] = true;
  }
}

void ofxImGui::onMouseScrolled(ofMouseEventArgs& event)
{    
  mouseWheel += (float)event.y;
}

void ofxImGui::renderDrawLists(ImDrawData* draw_data)
{
  for (int n = 0; n <  draw_data->CmdListsCount; n++)
  {
    ofVboMesh mesh;
    vector<ofIndexType> index;
    const ImDrawList* cmd_list = draw_data->CmdLists[n];
    for(size_t i = 0; i<cmd_list->VtxBuffer.size(); i++)
    {
      mesh.addVertex(ofVec3f(cmd_list->VtxBuffer[i].pos.x, cmd_list->VtxBuffer[i].pos.y, 0));
      mesh.addTexCoord(ofVec2f(cmd_list->VtxBuffer[i].uv.x, cmd_list->VtxBuffer[i].uv.y));
      ImColor imColor(cmd_list->VtxBuffer[i].col);
      mesh.addColor(ofFloatColor(imColor.Value.x, imColor.Value.y, imColor.Value.z, imColor.Value.w));
    }

    for(size_t i = 0; i<cmd_list->IdxBuffer.size(); i++)
    {
      mesh.addIndex((ofIndexType)cmd_list->IdxBuffer[i]);
    }

    ofxImGui::fontTexture.bind();
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
        mesh.getVbo().drawElements(GL_TRIANGLES, mesh.getNumIndices());
      }
    }
    glDisable(GL_SCISSOR_TEST);
    ofxImGui::fontTexture.unbind();
  }
}

const char * ofxImGui::getClipboardString()
{    
  return ofGetWindowPtr()->getClipboardString().c_str();
}

void ofxImGui::setClipboardString(const char* text)
{
  ofGetWindowPtr()->setClipboardString(text);
}

bool ofxImGui::createDeviceObjects()
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
      width,
      height,
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
      width,
      height,
      0,
      GL_ALPHA,
      GL_UNSIGNED_BYTE,
      pixels
    );
  }
  ofxImGui::fontTexture.texData.textureTarget = GL_TEXTURE_2D;
  ofxImGui::fontTexture.setUseExternalTextureID(externalTexture);

  io->Fonts->TexID = (void *)(intptr_t)ofxImGui::fontTexture.getTextureData().textureID;

  io->Fonts->ClearInputData();
  io->Fonts->ClearTexData();

  return true;
}

void ofxImGui::begin()
{
  if(!ofxImGui::fontTexture.isAllocated())
  {
    createDeviceObjects();
  }

  updateFrame();

  ImGui::NewFrame();
}

void ofxImGui::updateFrame()
{
  float current_time =  ofGetElapsedTimef();

  if(time > 0.0)
  {
    io->DeltaTime = current_time - time;
  } else {
    io->DeltaTime = 1.0f / 60.0f;
  }

  time = current_time;

  io->DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());
  io->MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());

  for (int i = 0; i < 3; i++)
  {
    io->MouseDown[i] = mousePressed[i] || ofGetMousePressed(i);
    mousePressed[i] = false;
  }

  io->MouseWheel = mouseWheel;
  mouseWheel = 0.0f;

  if(io->MouseDrawCursor)
  {
    ofHideCursor();
  }
}

void ofxImGui::end()
{
  ImGui::Render();
}

ofxImGui::~ofxImGui()
{
  ImGui::GetIO().Fonts->TexID = 0;
  io = NULL;
  ImGui::Shutdown();
}
