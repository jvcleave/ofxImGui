#include "ofxImGui.h"

ofTexture ofxImGui::fontTexture;

ofxImGui::ofxImGui()
{
    time = 0.0f;
    mouseWheel = 0.0f;

#ifndef TARGET_OPENGLES
    ofAppGLFWWindow* baseWindow = (ofAppGLFWWindow*)ofGetWindowPtr();
    glfwWindow = baseWindow->getGLFWWindow();
#endif    
    
    io = &ImGui::GetIO();
    
    ofAddListener(ofEvents().keyPressed, this, &ofxImGui::onKeyPressed);
    ofAddListener(ofEvents().keyReleased, this, &ofxImGui::onKeyReleased);
    ofAddListener(ofEvents().mousePressed, this, &ofxImGui::onMousePressed);
    ofAddListener(ofEvents().mouseScrolled, this, &ofxImGui::onMouseScrolled);
}


void ofxImGui::setup()
{   
     //Keyboard mapping. ImGui will use those indices to peek into the io->KeyDown[] array.
    io->KeyMap[ImGuiKey_Tab] = OF_KEY_TAB;
    io->KeyMap[ImGuiKey_LeftArrow] = OF_KEY_LEFT;
    io->KeyMap[ImGuiKey_RightArrow] = OF_KEY_RIGHT;
    io->KeyMap[ImGuiKey_UpArrow] = OF_KEY_UP;
    io->KeyMap[ImGuiKey_DownArrow] = OF_KEY_DOWN;
    io->KeyMap[ImGuiKey_PageUp] = OF_KEY_PAGE_UP;
    io->KeyMap[ImGuiKey_PageDown] = OF_KEY_PAGE_DOWN;
    io->KeyMap[ImGuiKey_Home] = OF_KEY_HOME;
    io->KeyMap[ImGuiKey_End] = OF_KEY_END;
    io->KeyMap[ImGuiKey_Delete] = OF_KEY_DEL;
    io->KeyMap[ImGuiKey_Backspace] = OF_KEY_BACKSPACE;
    io->KeyMap[ImGuiKey_Enter] = OF_KEY_RETURN;
    io->KeyMap[ImGuiKey_Escape] = OF_KEY_ESC;
    io->RenderDrawListsFn =  &ofxImGui::renderDrawLists;
    io->SetClipboardTextFn = ofxImGui::setClipboardString;
    io->GetClipboardTextFn = &ofxImGui::getClipboardString;
    
}

void ofxImGui::onKeyPressed(ofKeyEventArgs& event)
{
    io->KeysDown[event.key] = true;
    
}

void ofxImGui::onKeyReleased(ofKeyEventArgs& event)
{
    
    io->KeysDown[event.key] = false;
    io->AddInputCharacter((unsigned short)event.codepoint);
    
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


//https://github.com/ocornut/imgui/commit/59d498f3d0319dab32b3f4842c6e5f2da6d68830

ofFloatColor ofxImGui::convertToFloatColor(ImU32 rgba)
{
    float sc = 1.0f/255.0f;
    
    ofFloatColor result;
    result.r = (float)(rgba&0xFF) * sc; 
    result.g = (float)((rgba>>8)&0xFF) * sc; 
    result.b = (float)((rgba>>16)&0xFF) * sc; 
    result.a = (float)(rgba >> 24) * sc;
    return result;
}

void ofxImGui::renderDrawLists(ImDrawData* draw_data)
{
    vector<ofVboMesh> meshes;
    for (int n = 0; n <  draw_data->CmdListsCount; n++)
    {
        ofVboMesh mesh;
        vector<ofVec3f> verts;
        vector<ofVec2f> texCoords;
        vector<ofFloatColor> colors;
        vector<ofIndexType> index;
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        for(size_t i = 0; i<cmd_list->VtxBuffer.size(); i++)
        {
            verts.push_back(ofVec3f(cmd_list->VtxBuffer[i].pos.x, cmd_list->VtxBuffer[i].pos.y, 0));
            texCoords.push_back(ofVec2f(cmd_list->VtxBuffer[i].uv.x, cmd_list->VtxBuffer[i].uv.y));
            colors.push_back(ofxImGui::convertToFloatColor(cmd_list->VtxBuffer[i].col));
            
        }
        
        for(size_t i = 0; i<cmd_list->IdxBuffer.size(); i++)
        {
            index.push_back((ofIndexType)cmd_list->IdxBuffer[i] );
        }
        mesh.addVertices(verts);
        mesh.addTexCoords(texCoords);
        mesh.addColors(colors);
        mesh.addIndices(index);  
        meshes.push_back(mesh);
        
        
        
        
    }
    ofxImGui::fontTexture.bind();
    glEnable(GL_SCISSOR_TEST);
    for (int n = 0; n <  meshes.size(); n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                
                glScissor((int)pcmd->ClipRect.x, 
                          (int)(ofGetHeight() - pcmd->ClipRect.w), 
                          (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), 
                          (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                
                meshes[n].getVbo().drawElements(GL_TRIANGLES, meshes[n].getNumIndices());
                
                
            }
        }
    }
    glDisable(GL_SCISSOR_TEST);
    ofxImGui::fontTexture.unbind();
       

}

const char* ofxImGui::getClipboardString()
{    
    return ofGetWindowPtr()->getClipboardString().c_str();
}

void ofxImGui::setClipboardString(const char* text)
{
    ofGetWindowPtr()->setClipboardString(text);
}

#define STRINGIFY(x) #x
bool ofxImGui::createDeviceObjects()
{
    ofLogVerbose() << "ofIsGLProgrammableRenderer(): " << ofIsGLProgrammableRenderer();
    // Build texture
    unsigned char* pixels;
    int width, height;
    
    //GL_RGB8
    
    GLuint externalTexture;
    glGenTextures(1, &externalTexture);
    glBindTexture(GL_TEXTURE_2D, externalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
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
    ofxImGui::fontTexture.texData.textureTarget = GL_TEXTURE_2D;
    ofxImGui::fontTexture.setUseExternalTextureID(externalTexture);

    
#if 0    
    ofTextureData texData;
    texData.textureTarget = GL_TEXTURE_2D;
    int glFormat;
    int pixelType = GL_UNSIGNED_BYTE;
    ofPixelFormat pixFormat;
    if(ofIsGLProgrammableRenderer())
    {
        // Load as RGBA 32-bits for OpenGL3 because it is more likely to be compatible with user's existing shader.
        io->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height); 
        texData.glInternalFormat = GL_RGBA;
        
        glFormat = GL_RGBA;
        pixFormat = OF_PIXELS_RGBA;
        
    }else
    {
        texData.glInternalFormat = GL_RGB8;
        glFormat = GL_ALPHA;
        io->Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);
        pixFormat = OF_PIXELS_GRAY_ALPHA;
        
    }
    texData.width = width;
    texData.height = height;
    ofPixels myPixels;
    myPixels.setFromExternalPixels(pixels, width, height, pixFormat);

    
    ofxImGui::fontTexture.allocate(myPixels);
    
    
#endif
    
    // Store our identifier
    io->Fonts->TexID = (void *)(intptr_t)ofxImGui::fontTexture.getTextureData().textureID;
    
    // Cleanup (don't clear the input data if you want to append new fonts later)
    io->Fonts->ClearInputData();
    io->Fonts->ClearTexData();
    
    return true;
}

void ofxImGui::begin()
{
    if (!ofxImGui::fontTexture.isAllocated())
    {
         createDeviceObjects();
    }
    
    // Setup inputs
    // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
    updateFrame();
    
    // Start the frame
    ImGui::NewFrame();
}

void ofxImGui::updateFrame()
{
    
    // Setup time step
    float current_time =  ofGetElapsedTimef();
    
    if(time > 0.0 )
    {
        io->DeltaTime =  current_time - time;
    }else
    {
        io->DeltaTime =1.0f/60.0f;
    }
    
    time =  current_time;
    
    // Setup display size (every frame to accommodate for window resizing)
    //int w, h;
    int w = ofGetWindowWidth();
    int h = ofGetWindowHeight();
    io->DisplaySize = ImVec2((float)w, (float)h);
    
#if USING_GLFW
    if (glfwGetWindowAttrib(glfwWindow, GLFW_FOCUSED))
    {
        double mouse_x, mouse_y;
        mouse_x = (double)ofGetMouseX();
        mouse_y = (double)ofGetMouseY();
        mouse_x *= (float)ofGetScreenWidth() / w; // Convert mouse coordinates to pixels
        mouse_y *= (float)ofGetScreenHeight() / h;
        
        // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
        io->MousePos = ImVec2((float)mouse_x, (float)mouse_y);   
    }
    else
    {
        io->MousePos = ImVec2(-1,-1);
    }
#else
    io->MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());   
#endif
    for (int i = 0; i < 3; i++)
    {
        /*
         If a mouse press event came, always pass it as "mouse held this frame", 
         so we don't miss click-release events that are shorter than 1 frame.
         */
        
        io->MouseDown[i] = mousePressed[i] || ofGetMousePressed(i);  
        mousePressed[i] = false;
    }
    
    io->MouseWheel = mouseWheel;
    mouseWheel = 0.0f;
    
    // Hide OS mouse cursor if ImGui is drawing it
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
    io=NULL;
    ImGui::Shutdown();
}






