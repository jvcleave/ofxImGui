#include "Gui.h"

#include "ofAppRunner.h"



namespace ofxImGui
{
	//--------------------------------------------------------------
	Gui::Gui()
		: lastTime(0.0f)
		, theme(nullptr)
	{
		context = ImGui::CreateContext();
	}

	//--------------------------------------------------------------
	Gui::~Gui()
	{
		exit();
	}

	//--------------------------------------------------------------
	void Gui::setup(BaseTheme* theme_, bool autoDraw_, ImGuiConfigFlags customFlags_)
	{
		ImGui::SetCurrentContext(context);
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= customFlags_;
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

		ImGui::DestroyContext(context);
	}

  //--------------------------------------------------------------
  void Gui::SetDefaultFont(int indexAtlasFont) {
	ImGuiIO& io = ImGui::GetIO();
	if (indexAtlasFont < io.Fonts->Fonts.size()) {
	  io.FontDefault = io.Fonts->Fonts[indexAtlasFont];
	}
	else {
	  io.FontDefault = io.Fonts->Fonts[0];
	}
  }

  //--------------------------------------------------------------
  int Gui::addFont(const std::string & fontPath, float fontSize) {

	//ImFontConfig structure allows you to configure oversampling.
	//By default OversampleH = 3 and OversampleV = 1 which will make your font texture data 3 times larger
	//than necessary, so you may reduce that to 1.

		static const ImWchar polishCharRanges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x0100, 0x01FF, // Polish characters
			0,
		};

		ImGuiIO& io = ImGui::GetIO();
		std::string filePath = ofFilePath::getAbsolutePath(fontPath);

		char charFontPath[256];
		strcpy(charFontPath, filePath.c_str());
		//io.Fonts->AddFontFromFileTTF(fontPath, fontSize, NULL, io.Fonts->GetGlyphRangesDefault());
		ImFont* font = io.Fonts->AddFontFromFileTTF(charFontPath, fontSize, NULL, polishCharRanges);

		if (io.Fonts->Fonts.size() > 0) {
			return io.Fonts->Fonts.size() - 1;
		}
		else {
			return 0;
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
		// Only initialise once per frame
		if(!autoDraw && isRenderingManualFrame) return;

		ImGui::SetCurrentContext(context);
		ImGuiIO& io = ImGui::GetIO();

        io.DeltaTime = ofGetLastFrameTime();

		// Update settings
		io.MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());
		for (int i = 0; i < 5; i++) {
			io.MouseDown[i] = engine.mousePressed[i];
		}
		ImGui::NewFrame();
		isRenderingManualFrame = true;
	}

	//--------------------------------------------------------------
	void Gui::end()
	{
		// Only render in autodraw mode.
		// This allows calling end() and begin() multiple times per frame until we render, while ensuring auto mode works.
		if(autoDraw){
			ImGui::Render();
		}
	}

	//--------------------------------------------------------------
	void Gui::draw()
	{
		if (!autoDraw)
		{
			//engine.draw();
			ImGui::Render();
			isRenderingManualFrame = false;
		}
	}
}
