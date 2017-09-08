#pragma once

#include "ofImage.h"
#include "ofPixels.h"
#include "ofTexture.h"

#include "BaseEngine.h"
#include "BaseTheme.h"

namespace ofxImGui
{
	class Gui
	{
	public:
		Gui();
		~Gui();

		void setup(BaseTheme* theme = nullptr);
		void begin();
		void end();
		void close();

		BaseEngine* engine;
		float lastTime;

		void setTheme(BaseTheme* theme);
		void openThemeColorWindow();

		BaseTheme* theme;

		GLuint loadImage(ofImage& image);
		GLuint loadImage( std::string imagePath);

		GLuint loadPixels( std::string imagePath);
		GLuint loadPixels(ofPixels& pixels);

		GLuint loadTexture( std::string imagePath);
		GLuint loadTexture(ofTexture& texture, std::string imagePath);

		std::vector<ofTexture*> loadedTextures;
	};
}
