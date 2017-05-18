#include "Gui.h"

#include "ofAppRunner.h"

#if defined(TARGET_OPENGLES)
#include "EngineOpenGLES.h"
#elif defined (OF_TARGET_API_VULKAN)
#include "EngineVk.h"
#else
#include "EngineGLFW.h"
#endif

namespace ofxImGui
{
	//--------------------------------------------------------------
	Gui::Gui()
		: lastTime(0.0f)
		, engine(nullptr)
		, theme(nullptr)
	{}

	//--------------------------------------------------------------
	void Gui::setup(BaseTheme* theme_)
	{
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());
		io.MouseDrawCursor = false;

#if defined(TARGET_OPENGLES)
		engine = new EngineOpenGLES();
#elif defined (OF_TARGET_API_VULKAN) 
		engine = new EngineVk();
#else 
	engine = new EngineGLFW();
#endif

		engine->setup();

		if (theme_)
		{
			setTheme(theme_);
		}
		else
		{
			setTheme(new BaseTheme());
		}
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
		theme->updateColors();
		theme->setup();
	}

	//--------------------------------------------------------------
	void Gui::openThemeColorWindow()
	{
		theme->themeColorsWindow(true);
	}

	//--------------------------------------------------------------
	GLuint Gui::loadPixels(ofPixels& pixels)
	{
		return engine->loadTextureImage2D(pixels.getData(), pixels.getWidth(), pixels.getHeight());
	}

	//--------------------------------------------------------------
	GLuint Gui::loadPixels(string imagePath)
	{
		if (!engine) return -1;
		ofPixels pixels;
		ofLoadImage(pixels, imagePath);
		return loadPixels(pixels);
	}

	//--------------------------------------------------------------
	GLuint Gui::loadImage(ofImage& image)
	{
		if (!engine) return -1;
		return loadPixels(image.getPixels());
	}

	//--------------------------------------------------------------
	GLuint Gui::loadImage(string imagePath)
	{
		return loadPixels(imagePath);
	}

	//--------------------------------------------------------------
	GLuint Gui::loadTexture(string imagePath)
	{
		ofDisableArbTex();
		ofTexture* texture = new ofTexture();
		ofLoadImage(*texture, imagePath);
		ofEnableArbTex();
		loadedTextures.push_back(texture);
		return texture->getTextureData().textureID;
	}

	//--------------------------------------------------------------
	GLuint Gui::loadTexture(ofTexture& texture, string imagePath)
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

	//--------------------------------------------------------------
	void Gui::begin()
	{
		if (!engine)
		{
			ofLogError(__FUNCTION__) << "setup() call required, calling it for you";
			setup();
		}

		ImGuiIO& io = ImGui::GetIO();

		float currentTime = ofGetElapsedTimef();
		if (lastTime > 0.f)
		{
			io.DeltaTime = currentTime - lastTime;
		}
		else
		{
			io.DeltaTime = 1.0f / 60.f;
		}
		lastTime = currentTime;

		// Update settings
		io.MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());
		for (int i = 0; i < 5; i++) {
			io.MouseDown[i] = engine->mousePressed[i];

			// Update for next frame; set to false only if the mouse has been released
			engine->mousePressed[i] = !engine->mouseReleased;
		}
		ImGui::NewFrame();
	}

	//--------------------------------------------------------------
	void Gui::end()
	{
		ImGui::Render();
	}

	//--------------------------------------------------------------
	void Gui::close()
	{
		if (engine)
		{
			delete engine;
			engine = nullptr;
		}
		//if(io)
		//{
		//    io->Fonts->TexID = 0;
		//    io = nullptr;
		//}
		if (theme)
		{
			delete theme;
			theme = nullptr;
		}
		for (size_t i = 0; i < loadedTextures.size(); i++)
		{
			delete loadedTextures[i];
		}
		loadedTextures.clear();
	}

	//--------------------------------------------------------------
	Gui::~Gui()
	{
		close();
		ImGui::Shutdown();
	}
}
