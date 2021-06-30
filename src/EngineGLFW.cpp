#include "EngineGLFW.h"

#if ( !defined(OF_TARGET_API_VULKAN) )

#include "ofAppGLFWWindow.h"
#include "ofGLProgrammableRenderer.h"
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_opengl2.h"



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
        else ofLogNotice("EngineGLFW::setup()") << "Setting up GLFW in oF window size " << ImGui::GetIO().DisplaySize << " // Ptr=" << ofGetWindowPtr() << " ["<< ofGetWindowPtr()->getWindowSize() <<"]" << std::endl;

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
        ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLFW with native ImGui GLFW backend";
    #endif

        // Init window
        GLFWwindow* curWin = (GLFWwindow*)ofGetWindowPtr()->getWindowContext();
        ImGui_ImplGlfw_InitForOpenGL( curWin, true );

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
        ImGuiIO& io = ImGui::GetIO();
        io.BackendPlatformUserData = (void*) (GLFWwindow*) ofGetWindowPtr()->getWindowContext();
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
}

#endif
