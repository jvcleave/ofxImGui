#pragma once

#include "ofImage.h"
#include "ofPixels.h"
#include "ofTexture.h"

#ifdef OFXIMGUI_MULTIWINDOW_IMPL
#include <map>
#endif

#if defined(TARGET_OPENGLES)
#include "EngineOpenGLES.h"
#elif defined (OF_TARGET_API_VULKAN)
#include "EngineVk.h"
#else
#include "EngineGLFW.h"
#endif

#include "DefaultTheme.h"

namespace ofxImGui
{
	class Gui
	{
	public:
		Gui();
		~Gui();

        void setup(BaseTheme* theme = nullptr, bool autoDraw = true, ImGuiConfigFlags customFlags_=ImGuiConfigFlags_None, bool allowChaining_=false);
		void exit();

		void begin();
		void end();

		void draw();

        void SetDefaultFont(int indexAtlasFont);
        int addFont(const std::string & fontPath, float fontSize = 13.0f);

		void setTheme(BaseTheme* theme);

		GLuint loadImage(ofImage& image);
		GLuint loadImage(const std::string& imagePath);

		GLuint loadPixels(const std::string& imagePath);
		GLuint loadPixels(ofPixels& pixels);

		GLuint loadTexture(const std::string& imagePath);
		GLuint loadTexture(ofTexture& texture, const std::string& imagePath);

        void afterDraw(ofEventArgs& _args); // Listener

    private:
        //ImGuiContext* makeContext();
        bool hasContext();
        void render();

#if defined(TARGET_OPENGLES)
        EngineOpenGLES engine;
#elif defined (OF_TARGET_API_VULKAN) 
        EngineVk engine;
#else
        EngineGLFW engine;
#endif
        
		float lastTime;
		bool autoDraw;
        static bool isRenderingManualFrame;
        static bool chainingMode; // Allows calling gui.begin() & gui.end() multiple times per frame.
        ofEventListener listener;

		BaseTheme* theme;

		std::vector<ofTexture*> loadedTextures;

        // Static context instance. All Gui instances share the same context.
        // If you're dealing with dynamic libraries, you might need to pass this over to another ImGui instance.
        //static ImGuiContext* context;

        ImGuiContext* context = nullptr; // Short-hand value, same as stored in the map
        bool ownedContext = false; // Copy of context, set when it needs destruction
#ifdef OFXIMGUI_MULTIWINDOW_IMPL
        static std::map< ofAppBaseWindow*, ImGuiContext* > imguiContexts; // Holds all available ImGui instances per window.
#endif
	};
}
