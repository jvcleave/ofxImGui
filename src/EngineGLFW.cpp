#include "ofxImGuiConstants.h"
#include "EngineGLFW.h"

#ifdef OFXIMGUI_BACKEND_GLFW

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
	void EngineGLFW::setup(ofAppBaseWindow* _window, ImGuiContext* _imguiContext, bool autoDraw)
	{
        if (isSetup){
#ifdef OFXIMGUI_DEBUG
            ofLogNotice("EngineGLFW::setup()") << "Ignoring glfw setup, already bound." << std::endl;
#endif
            return;
        }
#ifdef OFXIMGUI_DEBUG
		else ofLogNotice("EngineGLFW::setup()") << ((void*)this) << " Setting up GLFW in oF window " << ofGetWindowPtr() << " // GlfwWindow=" << ofGetWindowPtr()->getWindowContext() << " ["<< ofGetWindowPtr()->getWindowSize() <<"]";

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

		// Store a reference to the current imgui context for event handling
		imguiContext = _imguiContext;

        // Init window
		ofAppBaseWindow* curOfWin = _window;
        GLFWwindow* curWin = (GLFWwindow*)curOfWin->getWindowContext();

		// Check if the undelying window is valid too
		if(curWin == nullptr){
			ofLogError("EngineGLFW::setup()") << "Sorry, for now ofxImGui needs to be setup in a valid window object.";
			return;
		}

		// Maybe we need this in the future ?
		// For now OF ensures it's already the active window so it's useless.
        //glfwMakeContextCurrent(curWin);

        // Todo: check return value from glfw ?
		// no more auto binding as it's required to set the right imgui context before calling the backend ones. See https://github.com/ocornut/imgui/blob/9764adc7bb7a582601dd4dd1c34d4fa17ab5c8e8/backends/imgui_impl_glfw.cpp#L142-L145
		ImGui_ImplGlfw_InitForOpenGL( curWin, (INTERCEPT_GLFW_CALLBACKS == 1)?false:true );

        //ImGui::GetIO().DisplaySize;

		// Register events manually if needed
#if INTERCEPT_GLFW_CALLBACKS == 1
		// Use GLFW window user pointer API to retrieve an instance, events are static/non-member
		// Error: OF needs the userpointer, or we'd have to set it back, too messy.
		//glfwSetWindowUserPointer(curWin, (void*)this);

		// Replace callbacks and store original callbacks
		// Note: the set functions return the previously installe callbacks or nullptr if none.
		originalOFCallbacks = {
			glfwSetWindowFocusCallback( curWin, GlfwWindowFocusCallbackGlobal),
			glfwSetCursorEnterCallback( curWin, GlfwCursorEnterCallbackGlobal),
			glfwSetCursorPosCallback(   curWin, GlfwCursorPosCallbackGlobal),
			glfwSetMouseButtonCallback( curWin, GlfwMouseButtonCallbackGlobal),
			glfwSetScrollCallback(      curWin, GlfwScrollCallbackGlobal),
			glfwSetKeyCallback(         curWin, GlfwKeyCallbackGlobal),
			glfwSetCharCallback(        curWin, GlfwCharCallbackGlobal),
			glfwSetMonitorCallback(             GlfwMonitorCallbackGlobal)
		};

		// Seems unnecessary (only needed for popped-out windows, when viewports are enabled ?)
		//glfwSetWindowPosCallback(   curWin, ImGui_ImplGlfw_WindowPosCallback);
		//glfwSetWindowSizeCallback(  curWin, ImGui_ImplGlfw_WindowSizeCallback);

		// Register context and listen to window destruction
		enginesMap.add(curWin, this);
		ofAddListener(curOfWin->events().exit, this, &EngineGLFW::onWindowExit);

		// This might give better performance if enabled. To be fully tested.
		//ImGui_ImplGlfw_SetCallbacksChainForAllWindows(true);

		ofLogNotice("EngineGLFW::setup()") << "Replaced the openFrameworks' GLFW event listeners by the ofxImGui ones which will then call the OF and ImGui ones while adding multi-context support.";
#else
		ofLogNotice("EngineGLFW::setup()") << "Replaced the openFrameworks' GLFW event listeners by the imgui_impl_glfw ones. You will not have multi-window nor multi-context support. This can be enabled by defining INTERCEPT_GLFW_CALLBACKS=1.";
#endif

        // Init renderer
        if( ofIsGLProgrammableRenderer() ){
			const char* glsl_version = getGLVersionFromOF();

#ifdef OFXIMGUI_DEBUG
            int major = ofGetGLRenderer()->getGLVersionMajor();
            int minor = ofGetGLRenderer()->getGLVersionMinor();
    #ifdef TARGET_OPENGLES
			ofLogVerbose("EngineGLFW::setup()") << "ofxImGui loading GLFW with programmable OpenGL ES " << major << "." << minor << " and version string «" << glsl_version << "»";
    #else
			ofLogVerbose("EngineGLFW::setup()") << "ofxImGui loading GLFW with programmable OpenGL " << major << "." << minor << " and version string «" << glsl_version << "»";
    #endif
#endif

            ImGui_ImplOpenGL3_Init(glsl_version); // Called by the function below, but needed with these arguments
			ImGui_ImplOpenGL3_CreateDeviceObjects();
        }
        else
        {
#ifdef OFXIMGUI_DEBUG
    #ifdef TARGET_OPENGLES
			ofLogVerbose("EngineGLFW::setup()") << "ofxImGui loading GLFW with OpenGL ES 1, which is quite experimental. [ofIsGLProgrammableRenderer()=" << (ofIsGLProgrammableRenderer()?"1":"0") << "]";
    #else
			ofLogVerbose("EngineGLFW::setup()") << "ofxImGui loading GLFW with OpenGL2 and ofIsGLProgrammableRenderer()=" << (ofIsGLProgrammableRenderer()?"1":"0");
    #endif
#endif
            ImGui_ImplOpenGL2_Init();
            ImGui_ImplOpenGL2_CreateDeviceObjects();
        }

		isSetup = true;

	}

    
    
	//--------------------------------------------------------------
	void EngineGLFW::exit()
	{
		if (!isSetup) return;

		// Ensure context is set, when shutting down from a callback for example.
		if(!setImGuiContext()){
#ifdef OFXIMGUI_DEBUG
			ofLogWarning("EngineGLFW::exit()") << "Couldn't set context on exit... there's probably something wrong. There's unexpected behaviour ahead, good luck !";
#endif
			return;
		}

		ofRemoveListener(ofGetCurrentWindow()->events().exit, this, &EngineGLFW::onWindowExit);

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

	//--------------------------------------------------------------
    bool EngineGLFW::updateFontsTexture(){
        if (ofIsGLProgrammableRenderer()) {
            return ImGui_ImplOpenGL3_CreateFontsTexture();
        }
        else {
            return ImGui_ImplOpenGL2_CreateFontsTexture();
        }
    }

	//--------------------------------------------------------------
	void EngineGLFW::onWindowExit(ofEventArgs& event){
		std::cout << "Window Exit! [engine]" << (void*)this << " // [window]" << ofGetWindowPtr() << std::endl;
		std::cout << "ofApp = " << (void*)ofGetAppPtr() << " // [window]" << ofGetAppPtr() << std::endl;

		// Set imgui context of this window
		if(!setImGuiContext()) return;

		// Call Destroy platform windows(otherwise pop-out windows remain visible but unresponsive)
		//ImGui_ImplGlfw_Shutdown();
		//ImGui_ImplGlfw_ShutdownPlatformInterface();

		// Todo: Notify global GLFW event listeners to not forward calls anymore ? Windows platforms seem to receuve a focus callback afterwards

		// When using quit, exit() makes pollEvents crash later. Try to call it before.
		//ofGetMainLoop()->pollEvents();

		// Dummy exit ? What if context is shared ?
		//exit();
#if INTERCEPT_GLFW_CALLBACKS == 1
		enginesMap.remove((GLFWwindow*)ofGetWindowPtr()->getWindowContext());
#endif


		// Set unloaded state
		imguiContext = nullptr;
		isSetup = false;

		// Todo: notify child/slaves about gui destruction ? gui.begin() should return false if the gui has gone ?
		std::cout << "Exit done" << std::endl;
	}

#if INTERCEPT_GLFW_CALLBACKS == 1
	//--------------------------------------------------------------
    // The code below is a template from the GLFW backend, almost unmodified, but commented.
    void EngineGLFW::GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
		// Call the original callback
		if(originalOFCallbacks.originalCallbackMousebutton) originalOFCallbacks.originalCallbackMousebutton(window, button, action, mods);

		// Set context
		if(!setImGuiContext()) return;

        // Call backend function
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

		// Warning: gotta call newFrame() to compute the active state (1 frame delay?)
		if(!ImGui::GetIO().WantCaptureMouse){
			// todo: call openframeworks callback
        }

        // Restore
		restoreImGuiContext();
    }

	//--------------------------------------------------------------
	void EngineGLFW::GlfwMouseButtonCallbackGlobal(GLFWwindow* window, int button, int action, int mods){
		//EngineGLFW* self = static_cast<EngineGLFW*>(glfwGetWindowUserPointer(window));
		EngineGLFW* self = enginesMap.findData(window);
		if(self) self->GlfwMouseButtonCallback(window, button, action, mods);
#ifdef OFXIMGUI_DEBUG
		else ofLogWarning("EngineGLFW::GlfwMouseButtonCallbackGlobal") << "Missed a callback due to unknown window = " << window;
#endif
	}

	//--------------------------------------------------------------
    void EngineGLFW::GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		// Call the original callback
		if(originalOFCallbacks.originalCallbackScroll) originalOFCallbacks.originalCallbackScroll(window, xoffset, yoffset);

		// Set context
		if(!setImGuiContext()) return;

		// Call backend function
		ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

		// Restore
		restoreImGuiContext();
    }

	//--------------------------------------------------------------
	void EngineGLFW::GlfwScrollCallbackGlobal(GLFWwindow* window, double xoffset, double yoffset) {
		EngineGLFW* self = enginesMap.findData(window);
		if(self) self->GlfwScrollCallback(window, xoffset, yoffset);
#ifdef OFXIMGUI_DEBUG
		else ofLogWarning("EngineGLFW::GlfwScrollCallbackGlobal") << "Missed a callback due to unknown window = " << window;
#endif
	}

	//--------------------------------------------------------------
    void EngineGLFW::GlfwKeyCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods){
		// Call the original callback
		if(originalOFCallbacks.originalCallbackKey) originalOFCallbacks.originalCallbackKey(window, keycode, scancode, action, mods);

		// Set context
		if(!setImGuiContext()) return;

		// Call backend function
		ImGui_ImplGlfw_KeyCallback(window, keycode, scancode, action, mods);

		// Restore
		restoreImGuiContext();
    }

	//--------------------------------------------------------------
	void EngineGLFW::GlfwKeyCallbackGlobal(GLFWwindow* window, int keycode, int scancode, int action, int mods){
		EngineGLFW* self = enginesMap.findData(window);
		if(self) self->GlfwKeyCallback(window, keycode, scancode, action, mods);
#ifdef OFXIMGUI_DEBUG
		else ofLogWarning("EngineGLFW::GlfwKeyCallbackGlobal") << "Missed a callback due to unknown window = " << window;
#endif
	}

	//--------------------------------------------------------------
    void EngineGLFW::GlfwWindowFocusCallback(GLFWwindow* window, int focused) {
//		if(ofGetMainLoop()->exitEvent)
//			if(ofGetMainLoop()->getCurrentWindow()->events().notifyExit() != nullptr)

		// Call the original callback
		if(originalOFCallbacks.originalCallbackWindowFocus) originalOFCallbacks.originalCallbackWindowFocus(window, focused);

		// Set context
		if(!setImGuiContext()) return;

		// Call backend function
		ImGui_ImplGlfw_WindowFocusCallback(window, focused);

		// Restore
		restoreImGuiContext();
    }

	//--------------------------------------------------------------
	void EngineGLFW::GlfwWindowFocusCallbackGlobal(GLFWwindow* window, int focused) {
		EngineGLFW* self = enginesMap.findData(window);
		if(self) self->GlfwWindowFocusCallback(window, focused);
#ifdef OFXIMGUI_DEBUG
		else ofLogWarning("EngineGLFW::GlfwWindowFocusCallbackGlobal") << "Missed a callback due to unknown window = " << window;
#endif
	}

	//--------------------------------------------------------------
	void EngineGLFW::GlfwCursorPosCallback(GLFWwindow* window, double x, double y) {
		// Call the original callback
		if(originalOFCallbacks.originalCallbackCursorPos) originalOFCallbacks.originalCallbackCursorPos(window, x, y);

		// Set context
		if(!setImGuiContext()) return;

		// Call backend function
		ImGui_ImplGlfw_CursorPosCallback(window, x, y);

		// Restore
		restoreImGuiContext();
    }

	//--------------------------------------------------------------
	void EngineGLFW::GlfwCursorPosCallbackGlobal(GLFWwindow* window, double x, double y) {
		EngineGLFW* self = enginesMap.findData(window);
		if(self) self->GlfwCursorPosCallback(window, x, y);
#ifdef OFXIMGUI_DEBUG
		else ofLogWarning("EngineGLFW::GlfwCursorPosCallbackGlobal") << "Missed a callback due to unknown window = " << window;
#endif
	}

	//--------------------------------------------------------------
    // Workaround: X11 seems to send spurious Leave/Enter events which would make us lose our position,
    // so we back it up and restore on Leave/Enter (see https://github.com/ocornut/imgui/issues/4984)
    void EngineGLFW::GlfwCursorEnterCallback(GLFWwindow* window, int entered) {
		// Call the original callback
		if(originalOFCallbacks.originalCallbackCursorEnter) originalOFCallbacks.originalCallbackCursorEnter(window, entered);

		// Set context
		if(!setImGuiContext()) return;

		// Call backend function
		ImGui_ImplGlfw_CursorEnterCallback(window, entered);

		// Restore
		restoreImGuiContext();
    }

	//--------------------------------------------------------------
	void EngineGLFW::GlfwCursorEnterCallbackGlobal(GLFWwindow* window, int entered) {
		EngineGLFW* self = enginesMap.findData(window);
		if(self) self->GlfwCursorEnterCallback(window, entered);
#ifdef OFXIMGUI_DEBUG
		else ofLogWarning("EngineGLFW::GlfwCursorEnterCallbackGlobal") << "Missed a callback due to unknown window = " << window;
#endif
	}

	//--------------------------------------------------------------
	void EngineGLFW::GlfwCharCallback(GLFWwindow* window, unsigned int c) {

		// Call the original callback
		if(originalOFCallbacks.originalCallbackChar) originalOFCallbacks.originalCallbackChar(window, c);

		// Set context
		if(!setImGuiContext()) return;

		// Call backend function
		ImGui_ImplGlfw_CharCallback(window, c);

		// Restore
		restoreImGuiContext();
    }

	//--------------------------------------------------------------
	void EngineGLFW::GlfwCharCallbackGlobal(GLFWwindow* window, unsigned int c) {
		EngineGLFW* self = enginesMap.findData(window);
		if(self) self->GlfwCharCallback(window, c);
#ifdef OFXIMGUI_DEBUG
		else ofLogWarning("EngineGLFW::GlfwCharCallbackGlobal") << "Missed a callback due to unknown window = " << window;
#endif
	}

	//--------------------------------------------------------------
	void EngineGLFW::GlfwMonitorCallback(GLFWmonitor* mon, int id) {
		// Call the original callback
		if(originalOFCallbacks.originalCallbackMonitor) originalOFCallbacks.originalCallbackMonitor(mon, id);

		// Set context
		if(!setImGuiContext()) return;

		// Call backend function
		ImGui_ImplGlfw_MonitorCallback(mon, id);

		// Restore
		restoreImGuiContext();
	}

	//--------------------------------------------------------------
	void EngineGLFW::GlfwMonitorCallbackGlobal(GLFWmonitor* mon, int id) {
		// To prevent storing an extra monitor map, and because windows can change their monitor, simply trigger all engines to resize.
		auto item = enginesMap.getFirst();
#ifdef OFXIMGUI_DEBUG
		if(!item) ofLogWarning("EngineGLFW::GlfwMonitorCallbackGlobal") << "Missed a callback due to no engines  = " << mon;
#endif
		while(item){
			item->data->GlfwMonitorCallbackGlobal(mon, id);
			item = item->getNext();
		}
	}

	LinkedList<GLFWwindow, EngineGLFW*> EngineGLFW::enginesMap = {};

#endif
}



#endif
