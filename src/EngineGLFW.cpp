#if !defined(TARGET_RASPBERRY_PI)

#include "EngineGLFW.h"

unsigned int EngineGLFW::vaoHandle = 0;

void EngineGLFW::setup(ImGuiIO* io_)
{
    io = io_;
    
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
    
    io->SetClipboardTextFn = &BaseEngine::setClipboardString;
    io->GetClipboardTextFn = &BaseEngine::getClipboardString;
        
    createDeviceObjects();

    ofAddListener(ofEvents().keyReleased, this, &EngineGLFW::onKeyReleased);
    
    ofAddListener(ofEvents().keyPressed, (BaseEngine*)this, &BaseEngine::onKeyPressed);
    ofAddListener(ofEvents().mousePressed, (BaseEngine*)this, &BaseEngine::onMousePressed);
    ofAddListener(ofEvents().mouseReleased, (BaseEngine*)this, &BaseEngine::onMouseReleased);
    ofAddListener(ofEvents().mouseScrolled, (BaseEngine*)this, &BaseEngine::onMouseScrolled);
    ofAddListener(ofEvents().windowResized, (BaseEngine*)this, &BaseEngine::onWindowResized);
     
}


void EngineGLFW::programmableRendererDrawLists(ImDrawData * draw_data)
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

void EngineGLFW::glRendererDrawLists(ImDrawData * draw_data)
{
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
}

void EngineGLFW::onKeyReleased(ofKeyEventArgs& event)
{
    int key = event.keycode;
    io->KeysDown[key] = false;
    
    io->KeyCtrl  = io->KeysDown[GLFW_KEY_LEFT_CONTROL] || io->KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io->KeyShift = io->KeysDown[GLFW_KEY_LEFT_SHIFT]   || io->KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io->KeyAlt   = io->KeysDown[GLFW_KEY_LEFT_ALT]     || io->KeysDown[GLFW_KEY_RIGHT_ALT];
    if(key < GLFW_KEY_ESCAPE)
    {
        io->AddInputCharacter((unsigned short)event.codepoint);
    }
}




bool EngineGLFW::createDeviceObjects()
{
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
}
#endif
