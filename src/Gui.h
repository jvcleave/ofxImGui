#pragma once

#include "ofImage.h"
#include "ofPixels.h"
#include "ofTexture.h"
#include "ofEvent.h"

#include <map>
#include <bitset>

#if defined (OFXIMGUI_ENABLE_OF_BINDINGS)
#include "EngineOpenFrameworks.h"
#elif defined (OF_TARGET_API_VULKAN)
#include "EngineVk.h"
//#elif defined(TARGET_OPENGLES) && !defined(TARGET_GLFW_WINDOW)
//#include "EngineOpenGLES.h"
//#elif defined(TARGET_GLFW_WINDOW)
#else
#include "EngineGLFW.h"
//#else
//#pragma error "Sorry, you cannot run ofxImGui on your openFrameworks configuration. Try using GLFW windows for your project."
#endif

#include "DefaultTheme.h"
#include "LinkedList.hpp"
//#include "imgui.h" // for ImFont*
struct ImFont;
//#include "imgui_internal.h" // for ImGuiContext*
struct ImGuiContext;

// Notes on context handling in ImGui
// - Ideally, we should create 1 imgui context per ofApp and one host viewports per ofAppBaseWindow, which actually is not possible.
//   Related issues:
//     - Use of viewports with existing windows : https://github.com/ocornut/imgui/issues/4298
//     - Multiple host viewports (solution: 1 context per window) : https://github.com/ocornut/imgui/issues/3012
// - Currently implemented, the easiest way to achieve this is by providing one ImGui context per ofWindow, letting instances draw to it.
//   The only drawback of this is that ImGui windows (or other draggables) cannot be moved to the other gui.
//   This should not be an issue in most ofApps. Generally in GPU apps it's recommended to keep the GUI to one application window.

// Todo: We could provide a way to intercept/filter OF mouse clicks when they are used by ImGui ?

// A context pointer with some more info
struct ofxImGuiContext {
	public:
		ofxImGuiContext() :
			imguiContext(ImGui::CreateContext()),
			isShared(false),
			isRenderingFrame(false),
			autoDraw(false){}

		ImGuiContext* imguiContext = nullptr;
		bool isShared;
		bool isRenderingFrame;
		bool autoDraw;

		// Explicit not to mess with compilers casting it to anything !
		explicit operator bool() const {
			return imguiContext != nullptr;
		}
		bool operator !() const {
			return imguiContext == nullptr;
		}
};

namespace ofxImGui
{
	enum SetupState : unsigned char;
	std::ostream& operator<<(std::ostream& os, const SetupState& _state);

	class Gui
	{
		// to provide access to window->context map
		friend class BaseEngine;
		friend class EngineGLFW;
		friend class EngineOpenFrameworks;
	public:
		Gui();
		~Gui();

		SetupState setup(BaseTheme* theme = nullptr, bool autoDraw = true, ImGuiConfigFlags customFlags_=ImGuiConfigFlags_None, bool _restoreGuiState = false, bool _showImGuiMouseCursor = false );
		void exit();

        // Todo: remove these ? Adapt them ?
        void setSharedMode(bool _sharedMode=true);
        bool isInSharedMode() const;

		void begin();
		void end();

		void draw();

        bool setDefaultFont(int indexAtlasFont);
        bool setDefaultFont(ImFont* _atlasFont);
        ImFont* addFont(const std::string & fontPath, float fontSize = 13.0f, const ImFontConfig* _fontConfig = nullptr, const ImWchar* _glyphRanges = nullptr, bool _setAsDefaultFont=false );
        ImFont* addFontFromMemory(void* fontData, int fontDataSize, float fontSize = 13.0f, const ImFontConfig* _fontConfig = nullptr, const ImWchar* _glyphRanges = nullptr, bool _setAsDefaultFont=false );

		void setTheme(BaseTheme* theme);

		GLuint loadImage(ofImage& image);
		GLuint loadImage(const std::string& imagePath);

		GLuint loadPixels(const std::string& imagePath);
		GLuint loadPixels(ofPixels& pixels);

		GLuint loadTexture(const std::string& imagePath);
		GLuint loadTexture(ofTexture& texture, const std::string& imagePath);
		// todo: updateFontTexture ?

        void afterDraw(ofEventArgs& _args); // Listener

    private:
        void render();

#if defined (OFXIMGUI_ENABLE_OF_BINDINGS)
        EngineOpenFrameworks engine;
#elif defined (OF_TARGET_API_VULKAN)
        EngineVk engine;
//#elif defined(TARGET_OPENGLES) && !defined(TARGET_GLFW_WINDOW)
//        EngineOpenGLES engine;
#else
        EngineGLFW engine;
#endif
        
		ofEventListener autoDrawListener;

		BaseTheme* theme=nullptr; // Todo: move this into ofxImguiContext ?

		std::vector<ofTexture*> loadedTextures;

        // Static context instance. All Gui instances share the same context.
        // If you're dealing with dynamic libraries, you might need to pass this over to another ImGui instance.
		ofxImGuiContext* context; // Short-hand value, same as stored in the map
		bool isContextOwned = false; // Copy of context, set when it needs destruction

		static LinkedList<ofAppBaseWindow, ofxImGuiContext> imguiContexts; // Window/MasterContext map
	};
}
