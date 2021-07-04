#include "EngineOpenGLES.h"

// Since the new backends, the GLES renderer is not used anymore, and rpis use the glfw backend with a GL or GLES renderer, having their respective backends too.
#if defined(TARGET_OPENGLES) && !defined(TARGET_GLFW_WINDOW)// && defined(USE_GLFW_WINDOW) //&& !defined(TARGET_RASPBERRY_PI_LEGACY)

//#include "ofAppiOSWindow.h"
//#include "imgui_impl_osx.h"
//#include "imgui_impl_opengl3.h"
#include "ofAppRunner.h"
#include "ofGLProgrammableRenderer.h"

namespace ofxImGui
{
	ofShader EngineOpenGLES::g_Shader;

	//--------------------------------------------------------------
	void EngineOpenGLES::setup(bool autoDraw)
	{
		if (isSetup) return;

		ImGuiIO& io = ImGui::GetIO();

#ifdef OFXIMGUI_DEBUG
        ofLogVerbose(__FUNCTION__) << "ofxImGui loading GLES with native imgui bindings";
        #pragma message "ofxImGui compiling with GLES renderer and native imgui bindings."
#endif
		// TODO
		// Init window
		//ofAppiOSWindow* curWin = (ofAppiOSWindow*)ofGetWindowPtr()->getWindowContext();
		//ImGui_ImplIOS_Init();
		
		isSetup = true;
	}

	//--------------------------------------------------------------
	void EngineOpenGLES::exit()
	{
		if (!isSetup) return;
		
		isSetup = false;
	}

	//--------------------------------------------------------------
	void EngineOpenGLES::render()
	{
		// todo

        // Handle viewports
        // todo: check if this works
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            // This line has to be ported to GLES
            //GLFWwindow* backup_current_context = glfwGetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            // Restore context so we can continue to render with oF
            // This line has to be ported to GLES
            //glfwMakeContextCurrent(backup_current_context);
        }
	}
} // end namespace ofxImGui

#endif

