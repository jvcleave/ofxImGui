#include "BaseEngine.h"

//#include "ofAppBaseWindow.h"
//#include "ofAppRunner.h"

#include "ofxImGuiConstants.h" // For OFXIMGUI_DEBUG
#include "imgui.h"
#include "ofGLProgrammableRenderer.h"

// Needed on Rpi
#if defined (TARGET_GLFW_WINDOW)
#include "GLFW/glfw3.h" 
#endif

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
		const char* glsl_version = nullptr;

		// Override imgui versions to fit oF versions
#ifdef TARGET_OPENGLES
		if( major==2 )
			glsl_version = "#version 100"; // Note: no "es" at the end
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
			else if( minor==4 ) glsl_version="#version 440 core";
			else if( minor==5 ) glsl_version="#version 450 core";
			else if( minor==6 ) glsl_version="#version 460 core";
		}
#endif
		return glsl_version;
	}

	// Maps OF keys to ImGui keys
	// Openframeworks keys are platform-specific
	ImGuiKey BaseEngine::oFKeyToImGuiKey(int key)
	{
		switch (key)
		{
			// Openframeworks keycodes are platform and windowing system dependant.
			// But there are some universal ones
			case OF_KEY_TAB:       return ImGuiKey_Tab;
			case OF_KEY_RETURN:    return ImGuiKey_Enter;
			case OF_KEY_ESC:       return ImGuiKey_Escape;
			case OF_KEY_BACKSPACE: return ImGuiKey_Backspace;
			case OF_KEY_DEL:       return ImGuiKey_Delete;
			case OF_KEY_F1:        return ImGuiKey_F1;
			case OF_KEY_F2:        return ImGuiKey_F2;
			case OF_KEY_F3:        return ImGuiKey_F3;
			case OF_KEY_F4:        return ImGuiKey_F4;
			case OF_KEY_F5:        return ImGuiKey_F5;
			case OF_KEY_F6:        return ImGuiKey_F6;
			case OF_KEY_F7:        return ImGuiKey_F7;
			case OF_KEY_F8:        return ImGuiKey_F8;
			case OF_KEY_F9:        return ImGuiKey_F9;
			case OF_KEY_F10:       return ImGuiKey_F10;
			case OF_KEY_F11:       return ImGuiKey_F11;
			case OF_KEY_F12:       return ImGuiKey_F12;
			case OF_KEY_LEFT:      return ImGuiKey_LeftArrow;
			case OF_KEY_UP:        return ImGuiKey_UpArrow;
			case OF_KEY_RIGHT:     return ImGuiKey_RightArrow;
			case OF_KEY_DOWN:      return ImGuiKey_DownArrow;
			case OF_KEY_PAGE_UP:   return ImGuiKey_PageUp;
			case OF_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
			case OF_KEY_HOME:      return ImGuiKey_Home;
			case OF_KEY_END:       return ImGuiKey_End;
			case OF_KEY_INSERT:    return ImGuiKey_Insert;

#if defined (TARGET_GLFW_WINDOW)
			case GLFW_KEY_TAB: return ImGuiKey_Tab;
			case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
			case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
			case GLFW_KEY_UP: return ImGuiKey_UpArrow;
			case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
			case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
			case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
			case GLFW_KEY_HOME: return ImGuiKey_Home;
			case GLFW_KEY_END: return ImGuiKey_End;
			case GLFW_KEY_INSERT: return ImGuiKey_Insert;
			case GLFW_KEY_DELETE: return ImGuiKey_Delete;
			case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
			case GLFW_KEY_SPACE: return ImGuiKey_Space;
			case GLFW_KEY_ENTER: return ImGuiKey_Enter;
			case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
			case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
			case GLFW_KEY_COMMA: return ImGuiKey_Comma;
			case GLFW_KEY_MINUS: return ImGuiKey_Minus;
			case GLFW_KEY_PERIOD: return ImGuiKey_Period;
			case GLFW_KEY_SLASH: return ImGuiKey_Slash;
			case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
			case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
			case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
			case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
			case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
			case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
			case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
			case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
			case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
			case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
			case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
			case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
			case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
			case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
			case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
			case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
			case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
			case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
			case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
			case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
			case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
			case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
			case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
			case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
			case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
			case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
			case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
			case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
			case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
			case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
			case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
			case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
			case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
			case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
			case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
			case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
			case GLFW_KEY_MENU: return ImGuiKey_Menu;
			case GLFW_KEY_0: return ImGuiKey_0;
			case GLFW_KEY_1: return ImGuiKey_1;
			case GLFW_KEY_2: return ImGuiKey_2;
			case GLFW_KEY_3: return ImGuiKey_3;
			case GLFW_KEY_4: return ImGuiKey_4;
			case GLFW_KEY_5: return ImGuiKey_5;
			case GLFW_KEY_6: return ImGuiKey_6;
			case GLFW_KEY_7: return ImGuiKey_7;
			case GLFW_KEY_8: return ImGuiKey_8;
			case GLFW_KEY_9: return ImGuiKey_9;
			case GLFW_KEY_A: return ImGuiKey_A;
			case GLFW_KEY_B: return ImGuiKey_B;
			case GLFW_KEY_C: return ImGuiKey_C;
			case GLFW_KEY_D: return ImGuiKey_D;
			case GLFW_KEY_E: return ImGuiKey_E;
			case GLFW_KEY_F: return ImGuiKey_F;
			case GLFW_KEY_G: return ImGuiKey_G;
			case GLFW_KEY_H: return ImGuiKey_H;
			case GLFW_KEY_I: return ImGuiKey_I;
			case GLFW_KEY_J: return ImGuiKey_J;
			case GLFW_KEY_K: return ImGuiKey_K;
			case GLFW_KEY_L: return ImGuiKey_L;
			case GLFW_KEY_M: return ImGuiKey_M;
			case GLFW_KEY_N: return ImGuiKey_N;
			case GLFW_KEY_O: return ImGuiKey_O;
			case GLFW_KEY_P: return ImGuiKey_P;
			case GLFW_KEY_Q: return ImGuiKey_Q;
			case GLFW_KEY_R: return ImGuiKey_R;
			case GLFW_KEY_S: return ImGuiKey_S;
			case GLFW_KEY_T: return ImGuiKey_T;
			case GLFW_KEY_U: return ImGuiKey_U;
			case GLFW_KEY_V: return ImGuiKey_V;
			case GLFW_KEY_W: return ImGuiKey_W;
			case GLFW_KEY_X: return ImGuiKey_X;
			case GLFW_KEY_Y: return ImGuiKey_Y;
			case GLFW_KEY_Z: return ImGuiKey_Z;
			case GLFW_KEY_F1: return ImGuiKey_F1;
			case GLFW_KEY_F2: return ImGuiKey_F2;
			case GLFW_KEY_F3: return ImGuiKey_F3;
			case GLFW_KEY_F4: return ImGuiKey_F4;
			case GLFW_KEY_F5: return ImGuiKey_F5;
			case GLFW_KEY_F6: return ImGuiKey_F6;
			case GLFW_KEY_F7: return ImGuiKey_F7;
			case GLFW_KEY_F8: return ImGuiKey_F8;
			case GLFW_KEY_F9: return ImGuiKey_F9;
			case GLFW_KEY_F10: return ImGuiKey_F10;
			case GLFW_KEY_F11: return ImGuiKey_F11;
			case GLFW_KEY_F12: return ImGuiKey_F12;
#endif
			default: return ImGuiKey_None;
		}

		return ImGuiKey_None;
	}
}
