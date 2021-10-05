#include "Gui.h"

#include "ofAppRunner.h"
#include "ofxWinTouchHook.h"


namespace ofxImGui
{
	//--------------------------------------------------------------
	Gui::Gui()
		: lastTime(0.0f)
		, theme(nullptr)
	{
		ImGui::CreateContext();
	}

	//--------------------------------------------------------------
	Gui::~Gui()
	{
		exit();
	}

	//--------------------------------------------------------------
	void Gui::setup(BaseTheme* theme_, bool autoDraw_)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());
		io.MouseDrawCursor = false;

		ofxWinTouchHook::EnableTouch();

		ofAddListener(ofxWinTouchHook::touchDown, this, &Gui::touchDown);
		ofAddListener(ofxWinTouchHook::touchUp, this, &Gui::touchUp);

		autoDraw = autoDraw_;
		engine.setup(autoDraw);

		if (theme_)
		{
			setTheme(theme_);
		}
		else
		{
			DefaultTheme* defaultTheme = new DefaultTheme();
			setTheme((BaseTheme*)defaultTheme);
		}
	}

	//--------------------------------------------------------------
	void Gui::exit()
	{
		engine.exit();
		if (theme)
		{
			delete theme;
			theme = nullptr;
		}
		for (size_t i = 0; i < loadedTextures.size(); i++)
		{
			if (loadedTextures[i])
			{
				delete loadedTextures[i];
				loadedTextures[i] = NULL;
			}
		}
		loadedTextures.clear();

		ofRemoveListener(ofxWinTouchHook::touchDown, this, &Gui::touchDown);
		ofRemoveListener(ofxWinTouchHook::touchUp, this, &Gui::touchUp);

		ImGui::DestroyContext();
	}

	//--------------------------------------------------------------
	void Gui::setTheme(BaseTheme* theme_)
	{
		if (theme)
		{
			delete theme;
			theme = nullptr;
		}
		theme = theme_;
		theme->setup();
	}



	//--------------------------------------------------------------
	GLuint Gui::loadPixels(ofPixels& pixels)
	{
		return engine.loadTextureImage2D(pixels.getData(), pixels.getWidth(), pixels.getHeight());
	}

	//--------------------------------------------------------------
	GLuint Gui::loadPixels(const std::string& imagePath)
	{
		ofPixels pixels;
		ofLoadImage(pixels, imagePath);
		return loadPixels(pixels);
	}

	//--------------------------------------------------------------
	GLuint Gui::loadImage(ofImage& image)
	{
		return loadPixels(image.getPixels());
	}

	//--------------------------------------------------------------
	GLuint Gui::loadImage(const std::string& imagePath)
	{
		return loadPixels(imagePath);
	}

	//--------------------------------------------------------------
	GLuint Gui::loadTexture(const std::string& imagePath)
	{
		ofDisableArbTex();
		ofTexture* texture = new ofTexture();
		ofLoadImage(*texture, imagePath);
		ofEnableArbTex();
		loadedTextures.push_back(texture);
		return texture->getTextureData().textureID;
	}

	//--------------------------------------------------------------
	GLuint Gui::loadTexture(ofTexture& texture, const std::string& imagePath)
	{
		bool isUsingArb = ofGetUsingArbTex();
		if (isUsingArb)
		{
			ofDisableArbTex();
		}
		ofLoadImage(texture, imagePath);
		if (isUsingArb)
		{
			ofEnableArbTex();
		}
		return texture.getTextureData().textureID;
	}

	void Gui::touchDown(ofTouchEventArgs& touch)
	{
		touchDown_m = true;
		touchX_m = touch.x;
		touchY_m = touch.y;
	}

	void Gui::touchUp(ofTouchEventArgs& touch)
	{
		touchUp_m = true;
	}

	//--------------------------------------------------------------
	void Gui::begin()
	{
		ImGuiIO& io = ImGui::GetIO();

		io.DeltaTime = ofGetLastFrameTime();

		// Update settings
		if(ofGetPreviousMouseX() != ofGetMouseX() || ofGetPreviousMouseY() != ofGetMouseY())
		{
			io.MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());
		}
		
		for (int i = 0; i < 5; i++) {
			io.MouseDown[i] = engine.mousePressed[i];
		}


		// Handle Windows Touch:
		if (touchDown_m)
		{
			io.MousePos = ImVec2((float)touchX_m, (float)touchY_m);
			io.MouseDown[0] = true;
			touchDown_m = false;
		}
		if (releaseTouch_m)
		{
			io.MouseDown[0] = false;
			releaseTouch_m = false;
		}
		if (touchUp_m)
		{
			// Trigger Release on next Frame
			releaseTouch_m = true;
			touchUp_m = false;
		}

		ImGui::NewFrame();
	}

	//--------------------------------------------------------------
	void Gui::end()
	{
		ImGui::Render();
	}

	//--------------------------------------------------------------
	void Gui::draw()
	{
		if (!autoDraw)
		{
			engine.draw();
		}
	}
}
