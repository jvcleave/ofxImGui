#include "ofxImGui.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

unsigned int ofxImGui::vboHandle = 0;
unsigned int ofxImGui::vaoHandle = 0;
unsigned int ofxImGui::elementsHandle = 0;
int ofxImGui::attribLocationColor = 0;
ofShader ofxImGui::shader;

#if defined(TARGET_OPENGLES)
int ofxImGui::g_ShaderHandle = 0;
int ofxImGui::g_VertHandle = 0;
int ofxImGui::g_FragHandle = 0;
int ofxImGui::g_AttribLocationTex = 0;
int ofxImGui::g_AttribLocationProjMtx = 0;
int ofxImGui::g_AttribLocationPosition = 0;
int ofxImGui::g_AttribLocationUV = 0;
#endif

ofxImGui::ofxImGui()
:last_time(0.f)
{

}

void ofxImGui::setup()
{
  io = &ImGui::GetIO();
  style = &ImGui::GetStyle();

  style->WindowMinSize            = ImVec2(160, 65);
  style->FramePadding             = ImVec2(4, 2);
  style->ItemSpacing              = ImVec2(6, 2);
  style->ItemInnerSpacing         = ImVec2(6, 4);
  style->Alpha                    = 1.0f;
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

#if defined(TARGET_OPENGLES)
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
  
  io->RenderDrawListsFn = openGLES_RendererDrawLists; 
#else

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
  
  if(ofIsGLProgrammableRenderer())
  {
    io->RenderDrawListsFn = programmableRendererDrawLists;  
  }
  else
  {
    io->RenderDrawListsFn = glRendererDrawLists;  
  }
#endif
  io->SetClipboardTextFn = &ofxImGui::setClipboardString;
  io->GetClipboardTextFn = &ofxImGui::getClipboardString;

  io->DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());

  io->MouseDrawCursor = false;

  createDeviceObjects();
  updateThemeColors();
  
  ofAddListener(ofEvents().keyPressed, this, &ofxImGui::onKeyPressed);
  ofAddListener(ofEvents().keyReleased, this, &ofxImGui::onKeyReleased);
  ofAddListener(ofEvents().mousePressed, this, &ofxImGui::onMousePressed);
  ofAddListener(ofEvents().mouseReleased, this, &ofxImGui::onMouseReleased);
  ofAddListener(ofEvents().mouseScrolled, this, &ofxImGui::onMouseScrolled);
  ofAddListener(ofEvents().windowResized, this, &ofxImGui::onWindowResized);
}

void ofxImGui::updateThemeColors()
{
  style->Colors[ImGuiCol_Text]                  = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 1.00f);
  style->Colors[ImGuiCol_TextDisabled]          = ImVec4(col_main_text.r / 255.f, col_main_text.g / 255.f, col_main_text.b / 255.f, 0.58f);
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
  style->Colors[ImGuiCol_TooltipBg]             = ImVec4(col_win_popup.r / 255.f, col_win_popup.g / 255.f, col_win_popup.b / 255.f, 0.92f);
  style->Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(col_main_area.r / 255.f, col_main_area.g / 255.f, col_main_area.b / 255.f, 0.73f);
}

void ofxImGui::themeColorsWindow(bool * p_opened)
{
  if(*p_opened)
  {
    ImGui::SetNextWindowSize(ImVec2(421, 192), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Theme Colors", p_opened);

    static float set_main_text[3] =
    {
      col_main_text.r / 255.f,
      col_main_text.g / 255.f,
      col_main_text.b / 255.f
    };
    bool changed_col_main_text = ImGui::ColorEdit3("Text",
     &set_main_text[0]
    );
    col_main_text = ofColor(
      set_main_text[0] * 255.f,
      set_main_text[1] * 255.f,
      set_main_text[2] * 255.f
    );

    static float set_main_head[3] =
    {
      col_main_head.r / 255.f,
      col_main_head.g / 255.f,
      col_main_head.b / 255.f
    };
    bool changed_col_main_head = ImGui::ColorEdit3("Headers",
     &set_main_head[0]
    );
    col_main_head = ofColor(
      set_main_head[0] * 255.f,
      set_main_head[1] * 255.f,
      set_main_head[2] * 255.f
    );

    static float set_main_area[3] =
    {
      col_main_area.r / 255.f,
      col_main_area.g / 255.f,
      col_main_area.b / 255.f
    };
    bool changed_col_main_area = ImGui::ColorEdit3("Areas",
     &set_main_area[0]
    );
    col_main_area = ofColor(
      set_main_area[0] * 255.f,
      set_main_area[1] * 255.f,
      set_main_area[2] * 255.f
    );

    static float set_win_popup[3] =
    {
      col_win_popup.r / 255.f,
      col_win_popup.g / 255.f,
      col_win_popup.b / 255.f
    };
    bool changed_col_win_popup = ImGui::ColorEdit3("Popups",
     &set_win_popup[0]
    );
    col_win_popup = ofColor(
      set_win_popup[0] * 255.f,
      set_win_popup[1] * 255.f,
      set_win_popup[2] * 255.f
    );

    static float set_win_backg[3] =
    {
      col_win_backg.r / 255.f,
      col_win_backg.g / 255.f,
      col_win_backg.b / 255.f
    };
    bool changed_col_win_backg = ImGui::ColorEdit3("Background",
     &set_win_backg[0]
    );
    col_win_backg = ofColor(
      set_win_backg[0] * 255.f,
      set_win_backg[1] * 255.f,
      set_win_backg[2] * 255.f
    );

    if(changed_col_main_text ||
       changed_col_main_head ||
       changed_col_main_area ||
       changed_col_win_popup ||
       changed_col_win_backg)
    {
      updateThemeColors();
    }

    ImGui::End();
  }
}

void ofxImGui::onKeyPressed(ofKeyEventArgs& event)
{
  int key = event.keycode;

  io->KeysDown[key] = true;

#if defined(TARGET_OPENGLES)
  io->AddInputCharacter((unsigned short)event.codepoint);
  //TODO add modifier keys?
#else
  io->KeyCtrl  = io->KeysDown[GLFW_KEY_LEFT_CONTROL] || io->KeysDown[GLFW_KEY_RIGHT_CONTROL];
  io->KeyShift = io->KeysDown[GLFW_KEY_LEFT_SHIFT]   || io->KeysDown[GLFW_KEY_RIGHT_SHIFT];
  io->KeyAlt   = io->KeysDown[GLFW_KEY_LEFT_ALT]     || io->KeysDown[GLFW_KEY_RIGHT_ALT];
  if(key < GLFW_KEY_ESCAPE)
  {
    io->AddInputCharacter((unsigned short)event.codepoint);
  }
#endif
}

void ofxImGui::onKeyReleased(ofKeyEventArgs& event)
{
  int key = event.keycode;


  io->KeysDown[key] = false;
  
#if defined(TARGET_OPENGLES)
  //TODO Check backspace on GLFW
  io->AddInputCharacter((unsigned short)event.codepoint);
#else
  io->KeyCtrl  = io->KeysDown[GLFW_KEY_LEFT_CONTROL] || io->KeysDown[GLFW_KEY_RIGHT_CONTROL];
  io->KeyShift = io->KeysDown[GLFW_KEY_LEFT_SHIFT]   || io->KeysDown[GLFW_KEY_RIGHT_SHIFT];
  io->KeyAlt   = io->KeysDown[GLFW_KEY_LEFT_ALT]     || io->KeysDown[GLFW_KEY_RIGHT_ALT];
#endif
  
}

void ofxImGui::onMousePressed(ofMouseEventArgs& event)
{
  if(event.button >= 0 && event.button < 5)
    io->MouseDown[event.button] = true;
}

void ofxImGui::onMouseReleased(ofMouseEventArgs& event)
{
  if(event.button >= 0 && event.button < 5)
    io->MouseDown[event.button] = false;
}

void ofxImGui::onMouseScrolled(ofMouseEventArgs& event)
{
  io->MouseWheel = event.scrollY;
}

void ofxImGui::onWindowResized(ofResizeEventArgs& window)
{
  io->DisplaySize = ImVec2((float)window.width, (float)window.height);
}

void ofxImGui::openGLES_RendererDrawLists(ImDrawData * draw_data)
{
#if defined(TARGET_OPENGLES)
  GLint last_program, last_texture, last_array_buffer, last_element_array_buffer;
    glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
    
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glActiveTexture(GL_TEXTURE0);
    
    float width = ofGetWidth();
    float height = ofGetHeight();
    
    
    
    // Setup orthographic projection matrix
    const float ortho_projection[4][4] =
    {
        { 2.0f/width,           0.0f,                   0.0f, 0.0f },
        { 0.0f,                  2.0f/-height,           0.0f, 0.0f },
        { 0.0f,                  0.0f,                  -1.0f, 0.0f },
        {-1.0f,                  1.0f,                   0.0f, 1.0f },
    };
    glUseProgram(g_ShaderHandle);
    glUniform1i(g_AttribLocationTex, 0);
    glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
    
    // Render command lists
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glEnableVertexAttribArray(g_AttribLocationPosition);
    glEnableVertexAttribArray(g_AttribLocationUV);
    glEnableVertexAttribArray(attribLocationColor);
    
    glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
    
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;
        
        glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);
        
        for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
        {
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(height-pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer_offset);

            }
            idx_buffer_offset += pcmd->ElemCount;
        }
    }
    
    // Restore modified state
    glDisableVertexAttribArray(g_AttribLocationPosition);
    glDisableVertexAttribArray(g_AttribLocationUV);
    glDisableVertexAttribArray(attribLocationColor);
    glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    glDisable(GL_SCISSOR_TEST);
#endif
}

void ofxImGui::glRendererDrawLists(ImDrawData * draw_data)
{
#if !defined(TARGET_OPENGLES)
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

  ImGuiIO * io = &ImGui::GetIO();
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

void ofxImGui::programmableRendererDrawLists(ImDrawData * draw_data)
{
#if !defined(TARGET_OPENGLES)
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
#endif //end TARGET_OPENGLES check
}

const char * ofxImGui::getClipboardString()
{
  return &ofGetWindowPtr()->getClipboardString()[0];
}

void ofxImGui::setClipboardString(const char * text)
{
  ofGetWindowPtr()->setClipboardString(text);
}

bool ofxImGui::createDeviceObjects()
{
#ifndef TARGET_OPENGLES
  if(ofIsGLProgrammableRenderer())
  {



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


  unsigned char * pixels;
  int width, height;
  io->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

  GLuint textureid = loadTextureImage2D(pixels, width, height);
  io->Fonts->TexID = (ImTextureID)(intptr_t)textureid;

  io->Fonts->ClearTexData();

  return true;
  
#else
  string header = "";
  
  string vertex_shader = header + R"(
  
  uniform mat4 ProjMat;
  
  attribute vec2 Position;
  attribute vec2 UV;
  attribute vec4 Color;
  
  varying vec2 Frag_UV;
  varying vec4 Frag_Color;
  
  void main()
  {
    Frag_UV = UV;
    Frag_Color = Color;
    gl_Position = ProjMat * vec4(Position.xy, 0, 1);
  }
  
  )";
  
  string fragment_shader = header + R"(
  
  uniform sampler2D Texture;
  
  varying vec2 Frag_UV;
  varying vec4 Frag_Color;
  
  void main()
  {
    gl_FragColor = Frag_Color * texture2D( Texture, Frag_UV);
  }
  
  )";
  
  // Backup GL state
  GLint last_texture, last_array_buffer;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  
  const GLchar *vertex_shader_es =
  "uniform mat4 ProjMtx;\n"
  "attribute vec2 Position;\n"
  "attribute vec2 UV;\n"
  "attribute vec4 Color;\n"
  "varying vec2 Frag_UV;\n"
  "varying vec4 Frag_Color;\n"
  "void main()\n"
  "{\n"
  "	Frag_UV = UV;\n"
  "	Frag_Color = Color;\n"
  "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
  "}\n";
  
  const GLchar* fragment_shader_es =
  "precision mediump float;\n"
  "uniform sampler2D Texture;\n"
  "varying vec2 Frag_UV;\n"
  "varying vec4 Frag_Color;\n"
  "void main()\n"
  "{\n"
  "	gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV);\n"
  "}\n";
  
  g_ShaderHandle = glCreateProgram();
  g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
  g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(g_VertHandle, 1, &vertex_shader_es, 0);
  glShaderSource(g_FragHandle, 1, &fragment_shader_es, 0);
  glCompileShader(g_VertHandle);
  glCompileShader(g_FragHandle);
  glAttachShader(g_ShaderHandle, g_VertHandle);
  glAttachShader(g_ShaderHandle, g_FragHandle);
  glLinkProgram(g_ShaderHandle);
  
  g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
  g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
  g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
  g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
  attribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");
  
  glGenBuffers(1, &vboHandle);
  glGenBuffers(1, &elementsHandle);
  
  ImGuiIO& io = ImGui::GetIO();
  
  // Build texture
  unsigned char* pixels;
  int width, height;
  //io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.
  
  if(ofIsGLProgrammableRenderer())
  {
    // Load as RGBA 32-bits for OpenGL3 because it is more likely to be compatible with user's existing shader.
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height); 
    glTexImage2D(GL_TEXTURE_2D, 
                 0, 
                 GL_RGBA, 
                 width, 
                 height, 
                 0, 
                 GL_RGBA, 
                 GL_UNSIGNED_BYTE, 
                 pixels);
    
  }else
  {
    io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
    glTexImage2D(GL_TEXTURE_2D, 
                 0,
                 GL_ALPHA,
                 width,
                 height, 
                 0,
                 GL_ALPHA, 
                 GL_UNSIGNED_BYTE, 
                 pixels);
  }
  
  // Create OpenGL texture
  GLuint g_FontTexture = 0;
  glGenTextures(1, &g_FontTexture);
  glBindTexture(GL_TEXTURE_2D, g_FontTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  
  // Store our identifier
  io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;
  
  // Cleanup (don't clear the input data if you want to append new fonts later)
  io.Fonts->ClearInputData();
  io.Fonts->ClearTexData();
  
  // Restore modified GL state
  glBindTexture(GL_TEXTURE_2D, last_texture);
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  
  return true;
#endif
}

GLuint ofxImGui::loadTextureImage2D(ofImage & image)
{
  return loadTextureImage2D(
    image.getPixels().getData(),
    image.getWidth(),
    image.getHeight()
  );
}

GLuint ofxImGui::loadTextureImage2D(unsigned char * pixels, int width, int height)
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

void ofxImGui::begin()
{
  float current_time = ofGetElapsedTimef();
  io->DeltaTime = last_time > 0.f ? current_time - last_time : 1.f / 60.f;
  last_time = current_time;

  io->MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());

  ImGui::NewFrame();
}

void ofxImGui::end()
{
  ImGui::Render();
}

ofxImGui::~ofxImGui()
{
  io->Fonts->TexID = 0;
  io = nullptr;
  style = nullptr;
  ImGui::Shutdown();
}
