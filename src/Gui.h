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

		void setup(BaseTheme* theme = nullptr, bool autoDraw = true);
		void exit();

		void begin();
		void end();

		void draw();

		void setTheme(BaseTheme* theme);
		void openThemeColorWindow();

		GLuint loadImage(ofImage& image);
		GLuint loadImage(const std::string& imagePath);

		GLuint loadPixels(const std::string& imagePath);
		GLuint loadPixels(ofPixels& pixels);

		GLuint loadTexture(const std::string& imagePath);
		GLuint loadTexture(ofTexture& texture, const std::string& imagePath);

	private:
		BaseEngine* engine;

		float lastTime;
		bool autoDraw;

		BaseTheme* theme;

		std::vector<ofTexture*> loadedTextures;
	};
}
