#include "Gui.h"

#include "ofAppRunner.h"



namespace ofxImGui
{
	//--------------------------------------------------------------
    Gui::Gui()
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
    // Must be called from the oF window that's gonna display the gui.
    // Creates 1 imgui context per oF window (glfw context).
    // Links the context handle to the correct window's imguicontext.
    void Gui::setup(BaseTheme* theme_, bool autoDraw_, ImGuiConfigFlags customFlags_, bool _restoreGuiState, bool _showImGuiMouseCursor )
	{
#ifdef OFXIMGUI_DEBUG
        ofLogVerbose("Gui::setup()") << "Setting up ofxImGui [" << this << "] in window " << ofGetWindowPtr();
#endif
        // Instance already setup ?
        if(context!=nullptr){
#ifdef OFXIMGUI_DEBUG
            ofLogWarning("Gui::setup()") << "This Gui instance is already setup, you are calling it twice ! (ignoring this 2nd call)";
#endif
            return;
        }

        // Check for existing context in current oF window.
        ofAppBaseWindow* curWindow = ofGetWindowPtr();

        // Curwindow cannot be null
        if(curWindow==nullptr){
            #ifdef OFXIMGUI_DEBUG
                ofLogError("Gui::setup()") << "You have to call me within an active oF window context !" << std::endl;
            #endif
            return;
        }

        if( imguiContexts.find( curWindow ) != imguiContexts.end() ){
            context = imguiContexts[curWindow];//ImGui::GetCurrentContext();
            ownedContext = false;
            sharedModes[context] = true; // tells master that the context is shared

            // Enable listener if autoDraw is on
            // To be fully tested. It might be problematic that it's registering to the slave instance here.
            if(autoDraw){
                listener.unsubscribe();
                listener = curWindow->events().draw.newListener( this, &Gui::afterDraw, OF_EVENT_ORDER_AFTER_APP );
            }

#ifdef OFXIMGUI_DEBUG
            ofLogVerbose("Gui::setup()") << "Context " << context << " already exists in window " << curWindow << ", using the existing context as a shared one." << std::endl;
            if(!autoDraw) ofLogWarning("Gui::setup()") << "You are using manual rendering. This might cause a crash; to fix, ensure that you call the render function after all ofxImGui::Gui instances have send ImGui commands.";
            //else ofLogNotice("Gui::setup()") << "Autodraw now happens after ofApp:draw() instead of when ofxImGui::end() is called." << std::endl;
#endif
        }
        // Create context for this window
        else {
            context = ImGui::CreateContext(); // Todo : pass shared fontatlas instance ?
            ownedContext = true;
            imguiContexts[curWindow] = context;

            // Note: only the first instance's setup() can set settings
            autoDraw = autoDraw_;
            sharedModes[context] = false;

            if( autoDraw && curWindow!=nullptr ){
                listener = curWindow->events().draw.newListener( this, &Gui::afterDraw, OF_EVENT_ORDER_AFTER_APP );
            }

#ifdef OFXIMGUI_DEBUG
            ofLogVerbose("Gui::setup()") << "Created context "<< context << " in window " << ofGetWindowPtr() << " ["<< ofGetWindowPtr()->getWindowSize() <<"]";
#endif
        }

        ImGui::SetCurrentContext(context);
		ImGuiIO& io = ImGui::GetIO();

        // Note : In chaining mode, additional flags can still be set.
        io.ConfigFlags |= customFlags_;

        // Already-setup contexts exit early
        if( !ownedContext ) return;

        // Mouse cursor drawing (disabled by default, oF uses the system mouse)
        io.MouseDrawCursor = _showImGuiMouseCursor;

        // Handle gui state saving
        if(!_restoreGuiState) io.IniFilename = nullptr;

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
        // Unregister the afterDraw() callback (if used)
        listener.unsubscribe();

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
            auto it = imguiContexts.begin();
            while( it != imguiContexts.end() && it->second != context ) it++;
            if( it != imguiContexts.end() ) imguiContexts.erase(it);

            auto frameEntry = isRenderingFrame.find(context);
            if( frameEntry != isRenderingFrame.end()){
                isRenderingFrame.erase(frameEntry);
            }

            ImGui::DestroyContext(context);
            ownedContext = false;
        }
        context = nullptr;
		
	}

    //--------------------------------------------------------------
    // In some rare cases you might wish to enable or disable this manually. (in most cases it's automatic)
    void Gui::setSharedMode(bool _sharedMode) {
        if(!context) return;
        sharedModes[context]=_sharedMode;
    }
    // Returns true if the context is setup and in shared mode.
    bool Gui::isInSharedMode() const {
        if( !context ) return false;
        auto ret = sharedModes.find(context);
        if( ret == sharedModes.end()) return false; // Should never happen
        return ret->second;
    }

    //--------------------------------------------------------------
    bool Gui::setDefaultFont(int indexAtlasFont) {
        if(context==nullptr){
            ofLogWarning() << "You must load fonts after gui.setup() ! (ignoring this call)";
            return false;
        }

        ImGui::SetCurrentContext(context);
        ImGuiIO& io = ImGui::GetIO();
        if( indexAtlasFont < 0 ){
            io.FontDefault = NULL; // default value, uses 0
        }
        else if (indexAtlasFont < io.Fonts->Fonts.size()) {
            io.FontDefault = io.Fonts->Fonts[indexAtlasFont];
            return true;
        }
        else {
            io.FontDefault = io.Fonts->Fonts[0];
        }
        return false;
    }

    bool Gui::setDefaultFont(ImFont* _atlasFont){
        if(context==nullptr){
            ofLogWarning() << "You must load fonts after gui.setup() ! (ignoring this call)";
            return false;
        }

        ImGui::SetCurrentContext(context);
        ImGuiIO& io = ImGui::GetIO();

        if( _atlasFont == nullptr ){
            io.FontDefault = NULL; // default value, uses 0
        }
        else {
            for(int i=0; i<io.Fonts->Fonts.size(); ++i){
                if(io.Fonts->Fonts[i] == _atlasFont){
                    // Set font
                    io.FontDefault = _atlasFont;
                    return true; // break;
                }
            }

            // None found --> set default
            io.FontDefault = NULL;
        }
        return false;
    }

  //--------------------------------------------------------------
  ImFont* Gui::addFont(const std::string & fontPath, float fontSize, const ImFontConfig* _fontConfig, const ImWchar* _glyphRanges, bool _setAsDefaultFont ) {

      if(context==nullptr){
          ofLogWarning() << "You must load fonts after gui.setup() ! (ignoring this call)";
          return nullptr;
      }

	//ImFontConfig structure allows you to configure oversampling.
	//By default OversampleH = 3 and OversampleV = 1 which will make your font texture data 3 times larger
	//than necessary, so you may reduce that to 1.

        ImGui::SetCurrentContext(context);
		ImGuiIO& io = ImGui::GetIO();
        std::string filePath = ofFilePath::getAbsolutePath(fontPath);

        // ensure default font gets loaded once
        if(io.Fonts->Fonts.size()==0) io.Fonts->AddFontDefault();

        ImFont* font = io.Fonts->AddFontFromFileTTF(filePath.c_str(), fontSize, _fontConfig, _glyphRanges);

		if (io.Fonts->Fonts.size() > 0) {
            io.Fonts->Build();
            if( engine.updateFontsTexture() ){
                if(_setAsDefaultFont) setDefaultFont(font);
                return font;
            }
            else return nullptr;
		}
		else {
            return nullptr;
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
        if(context==nullptr){
#ifdef OFXIMGUI_DEBUG
            ofLogWarning("Gui::begin()") << "Context is not set. Please call setup() first !";
#endif
            return;
        }

        // Ignore 2nd call to begin(), to allow chaining
        if( sharedModes.find(context) != sharedModes.end() && sharedModes[context] && isRenderingFrame.find(context) != isRenderingFrame.end() && isRenderingFrame[context] == true ){
            // Already began context
            return;
        }

        ImGui::SetCurrentContext(context);

        //std::cout << "New Frame in context " << context << " in window " << ofGetWindowPtr() << " (" << ofGetWindowPtr()->getWindowSize().x << ")" << std::endl;
        engine.newFrame();
        ImGui::NewFrame();
        //ImGui::AddUpdateViewport();

        isRenderingFrame[context] = true;
	}

	//--------------------------------------------------------------
	void Gui::end()
    {
        if(context==nullptr){
#ifdef OFXIMGUI_DEBUG
            ofLogWarning("Gui::end()") << "Context is not set. Please call setup() first !";
#endif
            return;
        }

        // Let context open in shared mode.
        if( sharedModes[context]==true ){
#ifdef OFXIMGUI_DEBUG
            if( !isRenderingFrame[context] ){
                ofLogWarning("Gui::end()") << "The Gui already rendered, or forgot to call Gui::Begin() ! Please ensure you render the gui after other instances have rendered.";
            }
#endif
            return;
        }


        // Only render in autodraw mode.
        if(autoDraw){
            // Protection : don't endFrame() twice.
            if( !isRenderingFrame[context] ){
                return;
            }

            // End submitting to ImGui
            engine.endFrame();
            ImGui::EndFrame();

            //render(); // Now called after ofApp::draw() using a callback.
        }
        // Otherwise end the frame. User chooses when to render (later) using Gui::draw()
        // Note: You cannot resume using ImGui::NewFrame() without flushing the pipeline.
        else {
            // Protection : don't endFrame() twice.
            if( !isRenderingFrame[context] ){
                return;
            }

            engine.endFrame(); // (Does nothing...)
            ImGui::EndFrame();
        }
    }

    //--------------------------------------------------------------
    // Note: render() is private, never called by the end user.
    void Gui::render(){
        if( context==nullptr ) return;

        ImGui::SetCurrentContext(context);
        ImGui::Render();
        engine.render();

        isRenderingFrame[context] = false;
    }

	//--------------------------------------------------------------
    void Gui::draw()
	{
		if(!context) return;

        // Just for clarity, we require draw() not to be used in autoDraw mode.
        if( autoDraw ){
#ifdef OFXIMGUI_DEBUG
            ofLogWarning("ofxImGui::Gui::draw()") << "Please don't call draw() in autoDraw mode.";
#endif
            return;
        }
        else if(isRenderingFrame[context]==true){
            render();
		}
	}


    void Gui::afterDraw( ofEventArgs& ){

        // This function is registered after ofApp::draw() to honor autodraw in shared context mode.
        if(context && isRenderingFrame[context] ){
            // Autodraw renders here if sharedMode is on
            if( (autoDraw && sharedModes[context]==true) ) render();
            // Render if manual render was forgotten (do we want this to happen??)
            else if( sharedModes[context]==false ) render();
        };
    }

    // Initialise statics
    std::map< ofAppBaseWindow*, ImGuiContext* > Gui::imguiContexts = {};
    std::map< ImGuiContext*, bool > Gui::isRenderingFrame = {};
    std::map< ImGuiContext*, bool > Gui::sharedModes = {};
}

