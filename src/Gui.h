#pragma once

#include "ofImage.h"
#include "ofPixels.h"
#include "ofTexture.h"

#include <map>

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

        void setup(BaseTheme* theme = nullptr, bool autoDraw = true, ImGuiConfigFlags customFlags_=ImGuiConfigFlags_None);
		void exit();

		void begin();
		void end();

		void draw();

        bool setDefaultFont(int indexAtlasFont);
        bool setDefaultFont(ImFont* _atlasFont);
        ImFont* addFont(const std::string & fontPath, float fontSize = 13.0f, const ImFontConfig* _fontConfig = nullptr, const ImWchar* _glyphRanges = nullptr, bool _setAsDefaultFont=false );

		void setTheme(BaseTheme* theme);

		GLuint loadImage(ofImage& image);
		GLuint loadImage(const std::string& imagePath);

		GLuint loadPixels(const std::string& imagePath);
		GLuint loadPixels(ofPixels& pixels);

		GLuint loadTexture(const std::string& imagePath);
		GLuint loadTexture(ofTexture& texture, const std::string& imagePath);

        void afterDraw(ofEventArgs& _args); // Listener

    private:
        void render();

#if defined(TARGET_OPENGLES)
        EngineOpenGLES engine;
#elif defined (OF_TARGET_API_VULKAN) 
        EngineVk engine;
#else
        EngineGLFW engine;
#endif
        
        float lastTime=0.f;
        bool autoDraw=true;
        ofEventListener listener;

        BaseTheme* theme=nullptr;

		std::vector<ofTexture*> loadedTextures;

        // Static context instance. All Gui instances share the same context.
        // If you're dealing with dynamic libraries, you might need to pass this over to another ImGui instance.
        ImGuiContext* context = nullptr; // Short-hand value, same as stored in the map
        // todo: rename to ownsContext
        bool ownedContext = false; // Copy of context, set when it needs destruction

        static std::map< ofAppBaseWindow*, ImGuiContext* > imguiContexts; // Holds all available ImGui instances per window.
        static std::map< ImGuiContext*, bool > isRenderingFrame; // isRenderingFrame, per context
        static std::map< ImGuiContext*, bool > sharedModes; // Shared mode, per context
	};
}
