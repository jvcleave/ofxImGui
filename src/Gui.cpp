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
  void Gui::enableDocking()
  {
    dockingEnabled = true;
    // only work with Docking branch of ImGui
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
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

    if (dockingEnabled) beginDocking();
	}

  //--------------------------------------------------------------
  void Gui::beginDocking()
  {
    // Initialize Docking
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
      // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Name", NULL, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
      ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
      ofLogError() << "Docking not enabled\n";
    }
  }

	//--------------------------------------------------------------
	void Gui::end()
	{
    if (dockingEnabled) endDocking();
		ImGui::Render();
	}

  //--------------------------------------------------------------
  void Gui::endDocking()
  {
    ImGui::End();
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
