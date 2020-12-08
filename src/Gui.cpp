#include "Gui.h"

#include "ofAppRunner.h"



namespace ofxImGui
{
	//--------------------------------------------------------------
	Gui::Gui()
		: lastTime(0.0f)
		, theme(nullptr)
	{
        // This performs an ImGui version check (asserts on fail)
        // Probably of interest for DLL code that needs to draw to ImGui.
        IMGUI_CHECKVERSION();

        // Note: ofGetWindowPtr() is not correct at this point in multi-window setups.
        // Cannot call ImGui::CreateContext here
    }

	//--------------------------------------------------------------
	Gui::~Gui()
	{
		exit();
	}

	//--------------------------------------------------------------
    void Gui::setup(BaseTheme* theme_, bool autoDraw_, ImGuiConfigFlags customFlags_, bool allowChaining_)
	{
#ifdef OFXIMGUI_DEBUG
        ofLogNotice("Gui::setup()") << "Setting up ofxImGui in window " << ofGetWindowPtr();
#endif

        // Skip window and context setup if another instance already enabled chaining
        if(!chainingMode){

            // Only setup once
            if(context != nullptr){
                ofLogWarning("ofxImGui") << "Warning, you are calling setup multiple times. Ignoring this second call. Maybe you'd like to enable chaining ?" << std::endl;
                return;
            }

            // Check if context exists
            if( !hasContext() ){

                ofAppBaseWindow* curWindow = ofGetWindowPtr();

                // Window has no context, create new context.
                context = ImGui::CreateContext(); // Todo : pass shared fontatlas instance ?
                ownedContext = true;

#if defined(OFXIMGUI_MULTIWINDOW_IMPL)
                ofAppBaseWindow* curWindow = ofGetWindowPtr();
                imguiContexts[curWindow] = context;
#endif

                //ImGuiViewport* vp = ImGui::FindViewportByPlatformHandle( (GLFWwindow*) curWindow );
                //if(vp) std::cout << "Context viewport = " << vp->GetWorkSize() << std::endl;
                chainingMode = allowChaining_;

                // Note: only the first instance's setup() can set autodraw
                autoDraw = autoDraw_;

//#ifndef OFXIMGUI_ENABLE_OF_BINDINGS
                if( autoDraw && chainingMode && curWindow!=nullptr ){
                    listener = curWindow->events().draw.newListener( this, &Gui::afterDraw, OF_EVENT_ORDER_AFTER_APP );
                }
//#endif
#ifdef OFXIMGUI_DEBUG
                ofLogNotice("Gui::setup()") << "Created context "<< context << " in window " << ofGetWindowPtr();
#endif
            }
            else {
                // Use the already-available context in this window
                context = ImGui::GetCurrentContext();
                ownedContext = false;
#ifdef OFXIMGUI_DEBUG
                ofLogNotice("Gui::setup()") << "Context "<< context << " already exists in window " << ofGetWindowPtr() << ", using the existing context.";
#endif
            }

        }
        else {
            context = ImGui::GetCurrentContext();
            ownedContext = false;
        }

        ImGui::SetCurrentContext(context);
		ImGuiIO& io = ImGui::GetIO();

        // Note : In chaining mode, additional flags can still be set.
		io.ConfigFlags |= customFlags_;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // tmp !
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // tmp !
        io.MouseDrawCursor = false;

#ifdef OFXIMGUI_ENABLE_OF_BINDINGS
        io.DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());
#endif

        // Already-setup contexts exit early
        if( !ownedContext ) return;

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

        // Source : https://github.com/yumataesu/ofxImGui_v3/blob/23ff3e02ae3b99cb3db449b950c2f3e34424fbc8/src/ofxImGui.cpp#L12-L18
        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        //std::cout << "Fonts= " << io.Fonts->Fonts.size() << std::endl;
	}

	//--------------------------------------------------------------
	void Gui::exit()
	{
//#ifndef OFXIMGUI_ENABLE_OF_BINDINGS
        // Unregister the afterDraw() callback (if used)
        listener.unsubscribe();
//#endif

        // Exit engine
        if(ownedContext){
            ImGui::SetCurrentContext(context);
            engine.exit();
        }

        // Theme
		if (theme)
		{
			delete theme;
			theme = nullptr;
		}

        // Textures
		for (size_t i = 0; i < loadedTextures.size(); i++)
		{
            if(loadedTextures[i])
            {
                delete loadedTextures[i];
                loadedTextures[i] = NULL;
            }
		}
		loadedTextures.clear();

        // Destroy context
        if(ownedContext){
#if defined(OFXIMGUI_MULTIWINDOW_IMPL)
            auto it = imguiContexts.begin();
            while( it != imguiContexts.end() && it->second != context ) it++;
            if( it != imguiContexts.end() ) imguiContexts.erase(it);
#endif
            ImGui::DestroyContext(context);
            context = nullptr;
            ownedContext = false;
        }
		
	}

  //--------------------------------------------------------------
  void Gui::SetDefaultFont(int indexAtlasFont) {
    ImGui::SetCurrentContext(context);
	ImGuiIO& io = ImGui::GetIO();
    if( indexAtlasFont < 0 ){
        io.FontDefault = NULL; // default value, uses 0
    }
    else if (indexAtlasFont < io.Fonts->Fonts.size()) {
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


        ImGui::SetCurrentContext(context);
		ImGuiIO& io = ImGui::GetIO();
        std::string filePath = ofFilePath::getAbsolutePath(fontPath);

		char charFontPath[256];
		strcpy(charFontPath, filePath.c_str());
        // ensure default font gets loaded
        if(io.Fonts->Fonts.size()==0) io.Fonts->AddFontDefault();
        ImFont* font = io.Fonts->AddFontFromFileTTF(charFontPath, fontSize, NULL, io.Fonts->GetGlyphRangesDefault());
        //ImFont* font = io.Fonts->AddFontFromFileTTF(charFontPath, fontSize, NULL, polishCharRanges);

		if (io.Fonts->Fonts.size() > 0) {
            io.Fonts->Build();
			return io.Fonts->Fonts.size() - 1;
		}
		else {
            return -1; // Fixed: now returns -1 to differentiate a fail from 0-value key
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
        // ImGui::DestroyContext();
        ImGui::SetCurrentContext(context);
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
        ImGui::SetCurrentContext(context);

#ifndef OFXIMGUI_MULTIWINDOW_IMPL
        // Only initialise once per frame
        if(chainingMode && isRenderingManualFrame){
            return;
        }
#endif

#if defined(OFXIMGUI_ENABLE_OF_BINDINGS)
//        ImGuiIO& io = ImGui::GetIO();

//        io.DeltaTime = ofGetLastFrameTime();
//        io.DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());

//        // Update mouse
//        io.MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());
//        for (int i = 0; i < 5; i++) {
//            io.MouseDown[i] = engine.mousePressed[i];
//        }
#endif

        //std::cout << "New Frame in context " << context << " in window " << ofGetWindowPtr() << " (" << ofGetWindowPtr()->getWindowSize().x << ")" << std::endl;
        engine.newFrame();
        ImGui::NewFrame();
        //ImGui::AddUpdateViewport();

#ifndef OFXIMGUI_MULTIWINDOW_IMPL
        isRenderingManualFrame = true;
#endif
	}

	//--------------------------------------------------------------
	void Gui::end()
    {
#ifndef OFXIMGUI_MULTIWINDOW_IMPL
        // Disable/ignore in chaining mode
        if(chainingMode){
            // This allows calling end() and begin() multiple times per frame until we render manually, while ensuring auto mode works.

            return;
        }
#endif

        // Only render in autodraw mode.
        if(autoDraw){
            render();
        }
        // Otherwise end the frame. User chooses when to render later with Gui::draw()
        // Note: You cannot resume using ImGui::NewFrame() without flushing the pipeline.
        else {
            engine.endFrame();
            ImGui::EndFrame();
        }
    }

    //--------------------------------------------------------------
    void Gui::render(){
        ImGui::SetCurrentContext(context);
        ImGui::Render();
        engine.render();

        // Should this be moved to engine code ?
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            //GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            //glfwMakeContextCurrent(backup_current_context);
        }
        isRenderingManualFrame = false;
    }

	//--------------------------------------------------------------
    void Gui::draw()
	{
		if (!autoDraw)
		{
            render();
		}
	}


    void Gui::afterDraw( ofEventArgs& ){

        // This function is registered after ofApp::draw() to honor autodraw in chaining mode.
        if(autoDraw && chainingMode && isRenderingManualFrame){
            render();
        };
    }

    // Returns if a context is setup (in current window with mutiwindow)
    bool Gui::hasContext(){
#if defined(OFXIMGUI_MULTIWINDOW_IMPL)
        ofAppBaseWindow* curWindow = ofGetWindowPtr();
        if( curWindow == nullptr ) return false;
        auto windowContext = imguiContexts.find(curWindow);
        return ( windowContext == imguiContexts.end() ) ? false : true;
#else
        return (context != nullptr);
#endif
    }

    bool Gui::chainingMode=false;
    bool Gui::isRenderingManualFrame=false;

    // Context singleton
    //ImGuiContext* Gui::context = nullptr;
#if defined(OFXIMGUI_MULTIWINDOW_IMPL)
    std::map< ofAppBaseWindow*, ImGuiContext* > Gui::imguiContexts = {};
#endif
}

