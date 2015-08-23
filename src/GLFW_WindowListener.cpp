#include "GLFW_WindowListener.h"



GLFW_WindowListener::GLFW_WindowListener()
{
    g_Time = 0.0f;
    mouseWheel = 0.0f;
    
    ofAppGLFWWindow* baseWindow = (ofAppGLFWWindow*)ofGetWindowPtr();
    glfwWindow = baseWindow->getGLFWWindow();
    
    io = &ImGui::GetIO();
    
    ofAddListener(ofEvents().keyPressed, this, &GLFW_WindowListener::onKeyPressed);
    ofAddListener(ofEvents().keyReleased, this, &GLFW_WindowListener::onKeyReleased);
    ofAddListener(ofEvents().mousePressed, this, &GLFW_WindowListener::onMousePressed);
    ofAddListener(ofEvents().mouseScrolled, this, &GLFW_WindowListener::onMouseScrolled);

    
#ifdef _WIN32
    io->ImeWindowHandle = glfwGetWin32Window(glfwWindow);
#endif
    
}


void GLFW_WindowListener::onKeyPressed(ofKeyEventArgs& event)
{
    io->KeysDown[event.key] = true;
    
}

void GLFW_WindowListener::onKeyReleased(ofKeyEventArgs& event)
{
    
    io->KeysDown[event.key] = false;
    io->AddInputCharacter((unsigned short)event.codepoint);
    
}

void GLFW_WindowListener::onMousePressed(ofMouseEventArgs& event)
{
    if (event.button >= 0 && event.button < 3)
    {
        g_MousePressed[event.button] = true;
    }
}

void GLFW_WindowListener::onMouseScrolled(ofMouseEventArgs& event)
{    
    GLFW_WindowListener::mouseWheel += (float)event.y;
}

void GLFW_WindowListener::updateFrame()
{
        
    // Setup time step
    float current_time =  ofGetElapsedTimef();
    
    if(g_Time > 0.0 )
    {
        io->DeltaTime =  current_time - g_Time;
    }else
    {
        io->DeltaTime =1.0f/60.0f;
    }

    g_Time =  current_time;
    
    // Setup display size (every frame to accommodate for window resizing)
    //int w, h;
    int w = ofGetWindowWidth();
    int h = ofGetWindowHeight();
    io->DisplaySize = ImVec2((float)w, (float)h);
    
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
    
    for (int i = 0; i < 3; i++)
    {
        /*
         If a mouse press event came, always pass it as "mouse held this frame", 
         so we don't miss click-release events that are shorter than 1 frame.
         */
        
        io->MouseDown[i] = g_MousePressed[i] || ofGetMousePressed(i);  
        g_MousePressed[i] = false;
    }
    
    io->MouseWheel = mouseWheel;
    mouseWheel = 0.0f;
    
    // Hide OS mouse cursor if ImGui is drawing it
    if(io->MouseDrawCursor)
    {
        ofHideCursor();
    }
    
}