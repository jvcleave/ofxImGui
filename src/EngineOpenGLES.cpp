#include "EngineOpenGLES.h"

int EngineOpenGLES::g_ShaderHandle = 0;
int EngineOpenGLES::g_AttribLocationTex = 0;
int EngineOpenGLES::g_AttribLocationProjMtx = 0;
int EngineOpenGLES::g_AttribLocationPosition = 0;
int EngineOpenGLES::g_AttribLocationUV = 0;

void EngineOpenGLES::setup(ImGuiIO* io_)
{
    io = io_;

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
    
    io->RenderDrawListsFn = rendererDrawLists; 
    
    io->SetClipboardTextFn = &BaseEngine::setClipboardString;
    io->GetClipboardTextFn = &BaseEngine::getClipboardString;
    
    createDeviceObjects();
    
    ofAddListener(ofEvents().keyReleased, this, &EngineOpenGLES::onKeyReleased);
    
    //in BaseEngine
    ofAddListener(ofEvents().keyPressed, (BaseEngine*)this, &BaseEngine::onKeyPressed);
    ofAddListener(ofEvents().mousePressed, (BaseEngine*)this, &BaseEngine::onMousePressed);
    ofAddListener(ofEvents().mouseReleased, (BaseEngine*)this, &BaseEngine::onMouseReleased);
    ofAddListener(ofEvents().mouseScrolled, (BaseEngine*)this, &BaseEngine::onMouseScrolled);
    ofAddListener(ofEvents().windowResized, (BaseEngine*)this, &BaseEngine::onWindowResized);
}

bool EngineOpenGLES::createDeviceObjects()
{
    
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
    
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragment_shader);
    shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex_shader);
    
    shader.linkProgram();
    
    
    // Backup GL state
    GLint last_texture, last_array_buffer;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    
    g_ShaderHandle = shader.getProgram();
    
    g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
    g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMat");
    g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
    g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
    attribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");
    
    glGenBuffers(1, &vboHandle);
    glGenBuffers(1, &elementsHandle);
    
    
    // Build texture
    unsigned char* pixels;
    int width, height;
    
    if(ofIsGLProgrammableRenderer())
    {
        // Load as RGBA 32-bits for OpenGL3 because it is more likely to be compatible with user's existing shader.
        io->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height); 
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
        io->Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
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
    
    
    GLuint textureid = loadTextureImage2D(pixels, width, height);
    io->Fonts->TexID = (void *)(intptr_t)textureid;
    
    io->Fonts->ClearTexData();
    
    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    
    return true;
}

void EngineOpenGLES::rendererDrawLists(ImDrawData * draw_data)
{
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
}



void EngineOpenGLES::onKeyReleased(ofKeyEventArgs& event)
{
    int key = event.keycode;
    io->KeysDown[key] = false;
    io->AddInputCharacter((unsigned short)event.codepoint);
    //TODO modifiers?
}

