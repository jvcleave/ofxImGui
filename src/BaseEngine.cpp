#include "BaseEngine.h"

#include "ofAppBaseWindow.h"
#include "ofAppRunner.h"
#include "imgui.h"

namespace ofxImGui
{
#if defined(OFXIMGUI_ENABLE_OF_BINDINGS)
	int BaseEngine::g_ShaderHandle = 0;
	int BaseEngine::g_VertHandle = 0;
	int BaseEngine::g_FragHandle = 0;

	int BaseEngine::g_UniformLocationTex = 0;
	int BaseEngine::g_UniformLocationProjMtx = 0;
	int BaseEngine::g_AttribLocationUV = 0;
	int BaseEngine::g_AttribLocationPosition = 0;
	int BaseEngine::g_AttribLocationColor = 0;

	unsigned int BaseEngine::g_VboHandle = 0;
	unsigned int BaseEngine::g_VaoHandle = 0;
	unsigned int BaseEngine::g_ElementsHandle = 0;

	std::string BaseEngine::g_ClipboardText = "";

	//--------------------------------------------------------------
	void BaseEngine::onKeyPressed(ofKeyEventArgs& event)
	{
		int key = event.keycode;
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[key] = true;
		//io->AddInputCharacter((unsigned short)event.codepoint);
	}

	//--------------------------------------------------------------
	void BaseEngine::onMouseDragged(ofMouseEventArgs& event)
	{

	}

	//--------------------------------------------------------------
	void BaseEngine::onMousePressed(ofMouseEventArgs& event)
	{
		if (event.button >= 0 && event.button < 5)
		{
			mousePressed[event.button] = true;
		}
	}

	//--------------------------------------------------------------
	void BaseEngine::onMouseReleased(ofMouseEventArgs& event)
	{
		if (event.button >= 0 && event.button < 5)
		{
			mousePressed[event.button] = false;
		}
	}

	//--------------------------------------------------------------
	void BaseEngine::onMouseScrolled(ofMouseEventArgs& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH = event.scrollX;
		io.MouseWheel = event.scrollY;
	}

	//--------------------------------------------------------------
	void BaseEngine::onWindowResized(ofResizeEventArgs& window)
	{
        // Note : Within event callbacks, the ImGui context is not set to that of the current window.
        //        The OfGetWindowPtr() functions works as axpected.
        //        std::cout << "onWindowResized() in ctx= " << ImGui::GetCurrentContext() << ", win=" << ofGetWindowPtr()->getWindowContext() << std::endl;

        //ImGuiIO& io = ImGui::GetIO();
        // Not needed anymore, overruled by imgui_glfw_newframe() via gui.begin()
        //io.DisplaySize = ImVec2((float)window.width, (float)window.height); // Not needed anymore ?

        //GLFWwindow* curWindow=(GLFWwindow*)ofGetWindowPtr()->getWindowContext();
        if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle( (void*)ofGetWindowPtr()->getWindowContext() )){
            viewport->PlatformRequestResize = true;
        }
        // todo: onWindowMove() for viewport->PlatformRequestMove = true;
	}

	//--------------------------------------------------------------
	const char* BaseEngine::getClipboardString(void * userData)
	{
		//return &ofGetWindowPtr()->getClipboardString()[0];
		g_ClipboardText = ofGetWindowPtr()->getClipboardString();
		return g_ClipboardText.c_str();
	}

	//--------------------------------------------------------------
	void BaseEngine::setClipboardString(void * userData, const char * text)
	{
        //ofGetWindowPtr()->setClipboardString(text); // Commented, looks like a useless call
		g_ClipboardText = ofToString(text);
		ofGetWindowPtr()->setClipboardString(g_ClipboardText);
	}
#endif

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

		return new_texture;
	};
}
