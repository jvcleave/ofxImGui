#include "EngineGLFW.h"

#if ( !defined(OF_TARGET_API_VULKAN) )

#include "ofAppGLFWWindow.h"
#include "ofGLProgrammableRenderer.h"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_opengl2.h"
#include "imgui.h"


namespace ofxImGui
{
	GLuint EngineGLFW::g_FontTexture = 0;

	//--------------------------------------------------------------
	void EngineGLFW::setup(bool autoDraw)
	{
        if (isSetup){
#ifdef OFXIMGUI_DEBUG
            ofLogNotice("EngineGLFW::setup()") << "Ignoring glfw setup, already bound." << std::endl;
#endif
            return;
        }
#ifdef OFXIMGUI_DEBUG
        else ofLogNotice("EngineGLFW::setup()") << "Setting up GLFW in oF window " << ofGetWindowPtr() << " // GlfwWindow=" << ofGetWindowPtr()->getWindowContext() << " ["<< ofGetWindowPtr()->getWindowSize() <<"]";

        #if defined(OFXIMGUI_DEBUG) && defined(TARGET_OPENGLES) && defined(TARGET_RASPBERRY_PI)
            // oF 0.11 related warnings
            #if defined(TARGET_RASPBERRY_PI_LEGACY)
                #pragma message "WARNING ! ofxImGui detected that you use the RPI in LEGACY mode, which use an EGL window, which ain't supported by any native imgui backend. Please consider using an ofxImGui version before 1.80."
            #else
                #pragma message "You are using the new Rpi GLFW binding. Please ensure that your raspi-config doesn't use the Legacy video drivers for better performance."
            #endif

            // Warn for GL ES 1 usage (unsupported by imgui)
            if( !ofIsGLProgrammableRenderer() ){
                ofLogWarning(__FUNCTION__) << "ofxImGui has not been tested with GL ES 1.0 and there's no official imgui backend for it.";
            }
        #endif // rpi gles
#endif // debug

    #ifdef OFXIMGUI_DEBUG
        #pragma message "ofxImGui compiling with the new native imgui backend."
        //ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with native ImGui GLFW backend";
    #endif

        // Init window
        ofAppBaseWindow* curOfWin = ofGetWindowPtr();
        GLFWwindow* curWin = (GLFWwindow*)curOfWin->getWindowContext();

//#ifdef OFXIMGUI_DEBUG
//        int tmpW = 0; int tmpH=0; if(curWin) glfwGetWindowSize(curWin, &tmpW, &tmpH);
//        ofLogVerbose(__FUNCTION__) << "ofxImGui setting up in GLFWwindow =" << curWin << " ("<< tmpW << "x"<< tmpH << ")";
//#endif

        // Todo: Maybe we need this ?
        //glfwMakeContextCurrent(curWin);

        // Todo: check return value from glfw ?
#if INTERCEPT_GLFW_CALLBACKS == 1
        ImGui_ImplGlfw_InitForOpenGL( curWin, false ); // no more auto binding as it's required to set the right imgui context before calling the backend ones. See https://github.com/ocornut/imgui/blob/9764adc7bb7a582601dd4dd1c34d4fa17ab5c8e8/backends/imgui_impl_glfw.cpp#L142-L145
#else
        ImGui_ImplGlfw_InitForOpenGL( curWin, true );
#endif

        //ImGui::GetIO().DisplaySize;

#if INTERCEPT_GLFW_CALLBACKS == 1
        UniqueWindowData windowData = {
            ImGui::GetCurrentContext(),
            curOfWin,
            // Install manual callbacks
            glfwSetWindowFocusCallback( curWin, GlfwWindowFocusCallback),
            glfwSetCursorEnterCallback( curWin, GlfwCursorEnterCallback),
            glfwSetCursorPosCallback(   curWin, GlfwCursorPosCallback),
            glfwSetMouseButtonCallback( curWin, GlfwMouseButtonCallback),
            glfwSetScrollCallback(      curWin, GlfwScrollCallback),
            glfwSetKeyCallback(         curWin, GlfwKeyCallback),
            glfwSetCharCallback(        curWin, GlfwCharCallback),
            glfwSetMonitorCallback(             GlfwMonitorCallback)
        };
        // Install manual callbacks
        //originalCallbackWindowFocus =   glfwSetWindowFocusCallback( curWin, GlfwWindowFocusCallback);
        //originalCallbackCursorEnter =   glfwSetCursorEnterCallback( curWin, GlfwCursorEnterCallback);
        //originalCallbackCursorPos =     glfwSetCursorPosCallback(   curWin, GlfwCursorPosCallback);
        //originalCallbackMousebutton =   glfwSetMouseButtonCallback( curWin, GlfwMouseButtonCallback);
        //originalCallbackScroll =        glfwSetScrollCallback(      curWin, GlfwScrollCallback);
        //originalCallbackKey =           glfwSetKeyCallback(         curWin, GlfwKeyCallback);
        //originalCallbackChar =          glfwSetCharCallback(        curWin, GlfwCharCallback);
        //originalCallbackMonitor =       glfwSetMonitorCallback(             GlfwMonitorCallback);

        // Save binding for managing glfw callbacks
        if(glfwContexts.find(curWin)==glfwContexts.end()){
            glfwContexts.emplace(curWin, windowData);
        }
        else {
            // Todo: This might be normal in sharedMode, but then this should not be called anyways.
            ofLogError(__PRETTY_FUNCTION__) << "glfwContexts already exists in GLFWwindow* " << curWin << "!";
        }
#endif

        // Seems unnecessary (only needed for popped-out windows?)
        //glfwSetWindowPosCallback(   curWin, ImGui_ImplGlfw_WindowPosCallback);
        //glfwSetWindowSizeCallback(  curWin, ImGui_ImplGlfw_WindowSizeCallback);

        // Init renderer
        if( ofIsGLProgrammableRenderer() ){
            int minor; int major;
            //glfwGetVersion(&minor, &major, nullptr); // Old way, seems to return maximal version with GLFW 3.4
            major = ofGetGLRenderer()->getGLVersionMajor();
            minor = ofGetGLRenderer()->getGLVersionMinor();

            // See imgui_impl_opengl3.cpp
            //----------------------------------------
            // OpenGL    GLSL      GLSL
            // version   version   string
            //----------------------------------------
            //  2.0       110       "#version 110"
            //  2.1       120       "#version 120"
            //  3.0       130       "#version 130"
            //  3.1       140       "#version 140"
            //  3.2       150       "#version 150"
            //  3.3       330       "#version 330 core"
            //  4.0       400       "#version 400 core"
            //  4.1       410       "#version 410 core"
            //  4.2       420       "#version 410 core"
            //  4.3       430       "#version 430 core"
            //  ES 2.0    100       "#version 100"      = WebGL 1.0
            //  ES 3.0    300       "#version 300 es"   = WebGL 2.0
            //----------------------------------------

            // default version empty --> use imgui backend's default
            const char* glsl_version = NULL;

            // Override imgui versions to fit oF versions
#ifdef TARGET_OPENGLES
            if( major==2 )
                glsl_version = "#version 100";
            if( major==3 ) // Note: not yet available in oF !!!
                glsl_version = "#version 300 es";
#else
            if( major==3 ){
                if( minor==0 )      glsl_version="#version 130";
                else if( minor==1 ) glsl_version="#version 140";
                else if( minor==2 ) glsl_version="#version 150";
                else if( minor==3 ) glsl_version="#version 330 core";
            }
            else if( major==4 ){
                if( minor==0 )      glsl_version="#version 400 core";
                else if( minor==1 ) glsl_version="#version 410 core";
                else if( minor==2 ) glsl_version="#version 420 core";
                else if( minor==3 ) glsl_version="#version 430 core";
            }
#endif

#ifdef OFXIMGUI_DEBUG
    #ifdef TARGET_OPENGLES
            ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with programmable OpenGL ES " << major << "." << minor << " and version string «" << glsl_version << "»";
    #else
            ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with programmable OpenGL " << major << "." << minor << " and version string «" << glsl_version << "»";
    #endif
#endif

            ImGui_ImplOpenGL3_Init(glsl_version); // Called by the function below, but needed with these arguments
            ImGui_ImplOpenGL3_CreateDeviceObjects();
            //ImGui_ImplOpenGL3_CreateFontsTexture(); // called by the above function
        }
        else
        {
#ifdef OFXIMGUI_DEBUG
    #ifdef TARGET_OPENGLES
            ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with OpenGL ES 1, which is quite experimental. [ofIsGLProgrammableRenderer()=" << (ofIsGLProgrammableRenderer()?"1":"0") << "]";
    #else
            ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with OpenGL2 and ofIsGLProgrammableRenderer()=" << (ofIsGLProgrammableRenderer()?"1":"0");
    #endif
#endif
            ImGui_ImplOpenGL2_Init();
            ImGui_ImplOpenGL2_CreateDeviceObjects();
        }

        // These are currently called by the imgui backend.
        //GLFWwindow* curWindow=(GLFWwindow*)ofGetWindowPtr()->getWindowContext();
        //glfwSetMouseButtonCallback( curWindow, ImGui_ImplGlfw_MouseButtonCallback);
        //glfwSetScrollCallback(      curWindow, ImGui_ImplGlfw_ScrollCallback);
        //glfwSetKeyCallback(         curWindow, ImGui_ImplGlfw_KeyCallback);
        //glfwSetCharCallback(        curWindow, ImGui_ImplGlfw_CharCallback);
        //glfwSetWindowCloseCallback( curWindow, ImGui_ImplGlfw_WindowCloseCallback);
        //glfwSetWindowPosCallback(   curWindow, ImGui_ImplGlfw_WindowPosCallback);
        //glfwSetWindowSizeCallback(  curWindow, ImGui_ImplGlfw_WindowSizeCallback);

		isSetup = true;
	}

    
    
	//--------------------------------------------------------------
	void EngineGLFW::exit()
	{
		if (!isSetup) return;
        
        if (ofIsGLProgrammableRenderer()){
            //ImGui_ImplOpenGL3_DestroyFontsTexture(); // called by function below
            //ImGui_ImplOpenGL3_DestroyDeviceObjects(); // Called below
            ImGui_ImplOpenGL3_Shutdown(); // called by ImGuiDestroyContext() later ?
        }
        else {
            //ImGui_ImplOpenGL2_DestroyFontsTexture(); // called by function below
            //ImGui_ImplOpenGL2_DestroyDeviceObjects(); // Called below
            ImGui_ImplOpenGL2_Shutdown();
        }
        ImGui_ImplGlfw_Shutdown();

		isSetup = false;
	}

    //--------------------------------------------------------------
    void EngineGLFW::newFrame()
    {
        // Set window ref for later usage
        //ImGuiIO& io = ImGui::GetIO();
        //io.BackendPlatformUserData = (void*) (GLFWwindow*) ofGetWindowPtr()->getWindowContext();
        if (ofIsGLProgrammableRenderer()){
            ImGui_ImplOpenGL3_NewFrame();
        }
        else{
            ImGui_ImplOpenGL2_NewFrame();
        }
        ImGui_ImplGlfw_NewFrame();
    }

    //--------------------------------------------------------------
    void EngineGLFW::endFrame()
    {

    }

	//--------------------------------------------------------------
    void EngineGLFW::render()
	{

        if (ofIsGLProgrammableRenderer()) {
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        else {
            // GLint last_program;
            // glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
            // glUseProgram(0);
            //There are potentially many more states you could need to clear/setup that we can't access from default headers.
            //e.g. glBindBuffer(GL_ARRAY_BUFFER, 0), glDisable(GL_TEXTURE_CUBE_MAP).
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
            //glUseProgram(last_program);
        }

        // Handle multi-viewports
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
            GLFWwindow* backup_current_context = glfwGetCurrentContext(); // Tocheck, maybe not possible on rpi ?

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            // Restore context so we can continue to render with oF
            glfwMakeContextCurrent(backup_current_context);
        }
	}

    bool EngineGLFW::updateFontsTexture(){
        if (ofIsGLProgrammableRenderer()) {
            return ImGui_ImplOpenGL3_CreateFontsTexture();
        }
        else {
            return ImGui_ImplOpenGL2_CreateFontsTexture();
        }
    }

#if INTERCEPT_GLFW_CALLBACKS == 1
    ImGuiContext* EngineGLFW::getImGuiContextOfWindow(const GLFWwindow* window){
        auto found = glfwContexts.find(window);
        if( found != glfwContexts.end()){
            return found->second.imguiContext;
        }
        return nullptr;
    }

    // The code below is a template from the GLFW backend, almost unmodified, but commented.
    void EngineGLFW::GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
//        ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
//        if (bd->PrevUserCallbackMousebutton != NULL && window == bd->Window)
//            bd->PrevUserCallbackMousebutton(window, button, action, mods);

//        ImGui_ImplGlfw_UpdateKeyModifiers(mods);

//        ImGuiIO& io = ImGui::GetIO();
//        if (button >= 0 && button < ImGuiMouseButton_COUNT)
//            io.AddMouseButtonEvent(button, action == GLFW_PRESS);

        // Issue : on the popout windows, the context needs to be set too...

        // We don't know which context is active when this cb is called. And we want to restore it afterwards.
        ImGuiContext* prevCtx = ImGui::GetCurrentContext();
        ImGuiContext* imguiContext = getImGuiContextOfWindow(window);
        if(imguiContext!=nullptr && prevCtx!=imguiContext){
            ImGui::SetCurrentContext(imguiContext);
        }
        // Call backend function
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

        // Warning: gotta call newFrame() to compute the active state (1 frame delay)
        //std::cout << "MouseClickCB --> imgui::WantCapture=" << ImGui::GetIO().WantCaptureMouse << " - Hovered=" << ImGui::IsWindowHovered( ImGuiHoveredFlags_AnyWindow ) << std::endl;
        if(!ImGui::GetIO().WantCaptureMouse){
            // todo: call openframeworks callback
        }

        // Restore
        if(prevCtx!=nullptr && prevCtx!=imguiContext){
            ImGui::SetCurrentContext(prevCtx);
        }
    }

    void EngineGLFW::GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
//        ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
//        if (bd->PrevUserCallbackScroll != NULL && window == bd->Window)
//            bd->PrevUserCallbackScroll(window, xoffset, yoffset);

//        ImGuiIO& io = ImGui::GetIO();
//        io.AddMouseWheelEvent((float)xoffset, (float)yoffset);
    }

    void EngineGLFW::GlfwKeyCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods){
//        ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
//        if (bd->PrevUserCallbackKey != NULL && window == bd->Window)
//            bd->PrevUserCallbackKey(window, keycode, scancode, action, mods);

//        if (action != GLFW_PRESS && action != GLFW_RELEASE)
//            return;

//        // Workaround: X11 does not include current pressed/released modifier key in 'mods' flags. https://github.com/glfw/glfw/issues/1630
//        if (int keycode_to_mod = ImGui_ImplGlfw_KeyToModifier(keycode))
//            mods = (action == GLFW_PRESS) ? (mods | keycode_to_mod) : (mods & ~keycode_to_mod);
//        ImGui_ImplGlfw_UpdateKeyModifiers(mods);

//        if (keycode >= 0 && keycode < IM_ARRAYSIZE(bd->KeyOwnerWindows))
//            bd->KeyOwnerWindows[keycode] = (action == GLFW_PRESS) ? window : NULL;

//        keycode = ImGui_ImplGlfw_TranslateUntranslatedKey(keycode, scancode);

//        ImGuiIO& io = ImGui::GetIO();
//        ImGuiKey imgui_key = ImGui_ImplGlfw_KeyToImGuiKey(keycode);
//        io.AddKeyEvent(imgui_key, (action == GLFW_PRESS));
//        io.SetKeyEventNativeData(imgui_key, keycode, scancode); // To support legacy indexing (<1.87 user code)
    }

    void EngineGLFW::GlfwWindowFocusCallback(GLFWwindow* window, int focused) {
//        ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
//        if (bd->PrevUserCallbackWindowFocus != NULL && window == bd->Window)
//            bd->PrevUserCallbackWindowFocus(window, focused);

//        ImGuiIO& io = ImGui::GetIO();
//        io.AddFocusEvent(focused != 0);
    }

    void EngineGLFW::GlfwCursorPosCallback(GLFWwindow* window, double x, double y) {
//        ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
//        if (bd->PrevUserCallbackCursorPos != NULL && window == bd->Window)
//            bd->PrevUserCallbackCursorPos(window, x, y);

//        ImGuiIO& io = ImGui::GetIO();
//        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//        {
//            int window_x, window_y;
//            glfwGetWindowPos(window, &window_x, &window_y);
//            x += window_x;
//            y += window_y;
//        }
//        io.AddMousePosEvent((float)x, (float)y);
//        bd->LastValidMousePos = ImVec2((float)x, (float)y);
    }

    // Workaround: X11 seems to send spurious Leave/Enter events which would make us lose our position,
    // so we back it up and restore on Leave/Enter (see https://github.com/ocornut/imgui/issues/4984)
    void EngineGLFW::GlfwCursorEnterCallback(GLFWwindow* window, int entered) {
//        ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
//        if (bd->PrevUserCallbackCursorEnter != NULL && window == bd->Window)
//            bd->PrevUserCallbackCursorEnter(window, entered);

//        ImGuiIO& io = ImGui::GetIO();
//        if (entered)
//        {
//            bd->MouseWindow = window;
//            io.AddMousePosEvent(bd->LastValidMousePos.x, bd->LastValidMousePos.y);
//        }
//        else if (!entered && bd->MouseWindow == window)
//        {
//            bd->LastValidMousePos = io.MousePos;
//            bd->MouseWindow = NULL;
//            io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
//        }
    }

    void EngineGLFW::GlfwCharCallback(GLFWwindow* window, unsigned int c) {
//        ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
//        if (bd->PrevUserCallbackChar != NULL && window == bd->Window)
//            bd->PrevUserCallbackChar(window, c);

//        ImGuiIO& io = ImGui::GetIO();
//        io.AddInputCharacter(c);
    }

    void EngineGLFW::GlfwMonitorCallback(GLFWmonitor*, int) {
//        ImGui_ImplGlfw_Data* bd = ImGui_ImplGlfw_GetBackendData();
//        bd->WantUpdateMonitors = true;
    }

    std::map<const GLFWwindow*, UniqueWindowData> EngineGLFW::glfwContexts = {};

#endif
}



#endif
