#include "Gui.h"

#include "ofAppRunner.h"



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
            if(loadedTextures[i])
            {
                delete loadedTextures[i];
                loadedTextures[i] = NULL;
            }
		}
		loadedTextures.clear();

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

	//--------------------------------------------------------------
	void Gui::begin()
	{
		

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
			io.MouseDown[i] = engine.mousePressed[i];
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
