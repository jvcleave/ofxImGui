#include "BaseEngine.h"

//#include "ofAppBaseWindow.h"
//#include "ofAppRunner.h"

#include "ofxImGuiConstants.h" // For OFXIMGUI_DEBUG
#include "imgui.h"
#include "ofGLProgrammableRenderer.h"

namespace ofxImGui
{
	//--------------------------------------------------------------
	GLuint BaseEngine::loadTextureImage2D(unsigned char * pixels, int width, int height)
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

		return new_texture; // And who will clean up the garbage ? Seemingly nobody...
	};

	//--------------------------------------------------------------
	bool BaseEngine::setImGuiContext(){
		if( isSetup == false || imguiContext == nullptr){
#ifdef OFXIMGUI_DEBUG
			if(imguiContext == nullptr){
				ofLogWarning("BaseEngine::setImGuiContext") << "The engine is setup but has no context ! :o ";
			}
#endif
			return false;
		}
		imguiContextPrev = ImGui::GetCurrentContext();
		ImGui::SetCurrentContext(imguiContext);
		return true;
	}

	//--------------------------------------------------------------
	void BaseEngine::restoreImGuiContext(){
		if( isSetup == false || imguiContextPrev == nullptr){
			return;
		}
		ImGui::SetCurrentContext(imguiContextPrev);
		imguiContextPrev = nullptr;
	}

	//--------------------------------------------------------------
	const char* BaseEngine::getGLVersionFromOF() const {
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
		return glsl_version;
	}
}
