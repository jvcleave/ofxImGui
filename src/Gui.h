#pragma once

#include "ofImage.h"
#include "ofPixels.h"
#include "ofTexture.h"

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

		void setup(BaseTheme* theme = nullptr, bool autoDraw = true);
		void exit();

		void begin();
		void end();

		void draw();

		void setTheme(BaseTheme* theme);

		GLuint loadImage(ofImage& image);
		GLuint loadImage(const std::string& imagePath);

		GLuint loadPixels(const std::string& imagePath);
		GLuint loadPixels(ofPixels& pixels);

		GLuint loadTexture(const std::string& imagePath);
		GLuint loadTexture(ofTexture& texture, const std::string& imagePath);

	private:        
#if defined(TARGET_OPENGLES)
        EngineOpenGLES engine;
#elif defined (OF_TARGET_API_VULKAN) 
        EngineVk engine;
#else
        EngineGLFW engine;
#endif
        
		float lastTime;
		bool autoDraw;

		BaseTheme* theme;

		std::vector<ofTexture*> loadedTextures;
	};
}
