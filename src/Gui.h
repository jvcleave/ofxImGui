#pragma once

#include "ofImage.h"
#include "ofPixels.h"
#include "ofTexture.h"
#include "ofEvent.h"

#include <map>

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

namespace ofxImGui
{
	class Gui
	{
		// to provide access to window->context map
		friend class BaseEngine;
		friend class EngineGLFW;
		friend class EngineOpenFrameworks;
	public:
		Gui();
		~Gui();

        void setup(BaseTheme* theme = nullptr, bool autoDraw = true, ImGuiConfigFlags customFlags_=ImGuiConfigFlags_None, bool _restoreGuiState = false, bool _showImGuiMouseCursor = false );
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
        
        bool autoDraw=true;
        ofEventListener listener;

        BaseTheme* theme=nullptr;

		std::vector<ofTexture*> loadedTextures;

        // Static context instance. All Gui instances share the same context.
        // If you're dealing with dynamic libraries, you might need to pass this over to another ImGui instance.
        ImGuiContext* context = nullptr; // Short-hand value, same as stored in the map
        // todo: rename to ownsContext
        bool ownedContext = false; // Copy of context, set when it needs destruction
		bool ownedWindow = false; // Copy of context, set when it needs destruction
        static std::map< ofAppBaseWindow*, ImGuiContext* > imguiContexts; // Holds all available ImGui instances per window.
        static std::map< ImGuiContext*, bool > isRenderingFrame; // isRenderingFrame, per context
        static std::map< ImGuiContext*, bool > sharedModes; // Shared mode, per context
	};
}
