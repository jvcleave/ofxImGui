#include "Gui.h"

#include "ofAppRunner.h"
#include "ofAppGLFWWindow.h"

//#include "imgui.h"
//#include "backends/imgui_impl_glfw.h"


// Todo: Texture/Pixel loading functions: Stop using ofDisableArbTex and use appropriate textureSettings instead.

namespace ofxImGui
{
	enum SetupState : unsigned char {
		Error = 0, // Keep to 0 so that it evaluates to false ?
		Slave = 1 << 1,
		Master = 1 << 2,
		// Success flag
		Success = Slave | Master, // Use like if(mState & Success)
	};
	std::ostream& operator<<(std::ostream& os, const SetupState& _state)
	{
		os << std::bitset<sizeof(SetupState)*8>(_state).to_string() << " (" << ((unsigned int)_state) << ")";
		return os;
	}

	//--------------------------------------------------------------
	Gui::Gui() : context(nullptr)
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
    // Todo: an optional ofAppBaseWindow* to specify a manual context instead of using the current active ofApp window.
	// Returns a state indcating if the setup happened as slave or master
	SetupState Gui::setup(BaseTheme* theme_, bool autoDraw_, ImGuiConfigFlags customFlags_, bool _restoreGuiState, bool _showImGuiMouseCursor )
	{
#ifdef OFXIMGUI_DEBUG
        ofLogVerbose("Gui::setup()") << "Setting up ofxImGui [" << this << "] in window " << ofGetWindowPtr();
#endif
        // Instance already setup ?
		if( context!=nullptr ){
#ifdef OFXIMGUI_DEBUG
            ofLogWarning("Gui::setup()") << "This Gui instance is already setup, you are calling it twice ! (ignoring this 2nd call)";
#endif
			return isContextOwned ? SetupState::Master : SetupState::Slave;
        }

        // Check for existing context in current oF window.
        // We only create one ImGuiContext per ofAppWindow (1 for each glfwWindow instance handled by oF)
        ofAppBaseWindow* curWindow = ofGetWindowPtr();

        // Curwindow cannot be null
        if(curWindow==nullptr){
            ofLogError("Gui::setup()") << "You have to call setup() from within an active oF window context ! Try calling setup() at ofApp::setup() or later.";
			return SetupState::Error;
        }
		// Check if the undelying window is valid too
        else if((void*)curWindow->getWindowContext() == nullptr){
			ofLogError("Gui::setup()") << "Sorry, for now ofxImGui needs to be setup in a valid window object.";
			return SetupState::Error;
        }

		// Grab existing ImGui Context
		ImGuiContext* existingImGuiContext = ImGui::GetCurrentContext(); // Null on first call ever

		// Does this ofBaseWindow already have an ofxImGuiContext ?
		ofxImGuiContext* ofWindowContext = imguiContexts.findData( curWindow );
		if( ofWindowContext != nullptr ){
			ofWindowContext->isShared = true; // tells master that the context became shared

			// Keep reference of master context
			context = ofWindowContext;
			isContextOwned = false;

#ifdef OFXIMGUI_DEBUG
			ofLogNotice("Gui::setup()") << "Context " << context << "/" << context->imguiContext << " already exists in window " << curWindow << ", using the existing context as a shared one.";
			if(autoDraw_) ofLogWarning("Gui::setup()") << "You requested to enable Autodraw, but this is a Slave setting. Not enabling autoDraw.";
#endif
        }
        // Create a unique context for this window
        else {
			context = new ofxImGuiContext();
			isContextOwned = true;

			// Register
			imguiContexts.add(curWindow, context);

			// Enable autodraw
			if( autoDraw_ && curWindow!=nullptr ){
				context->autoDraw = true;
				autoDrawListener = curWindow->events().draw.newListener( this, &Gui::afterDraw, OF_EVENT_ORDER_AFTER_APP );
            }

#ifdef OFXIMGUI_DEBUG
            ofLogVerbose("Gui::setup()") << "Created context "<< context << " in window " << ofGetWindowPtr() << " ["<< ofGetWindowPtr()->getWindowSize() <<"]";
#endif
        }

		// Set context and grab IO
		ImGui::SetCurrentContext(context->imguiContext);
        ImGuiIO& io = ImGui::GetIO();

		// Dummy call that will crash if the io is invalid --> ie for easier debugging
		(void)io;

        // Note : In chaining mode, additional flags can still be set.
        io.ConfigFlags |= customFlags_;

		// Already-setup window --> slaves exit early
		if( !isContextOwned ) {
#ifdef OFXIMGUI_DEBUG
            ofLogVerbose("Gui::setup()") << "Context is not owned, skipping some settings and not binding the engine again.";
#endif
			return SetupState::Slave;
        }

		// Master may set more settings
		// Mouse cursor drawing (disabled by default, oF uses the system mouse)
		io.MouseDrawCursor = _showImGuiMouseCursor;

		// Handle gui state saving
		if(!_restoreGuiState)
			io.IniFilename = nullptr;

		engine.setup( curWindow, context->autoDraw);

		if (theme_)
		{
			setTheme(theme_);
		}
		else
		{
			DefaultTheme* defaultTheme = new DefaultTheme();
			setTheme((BaseTheme*)defaultTheme);
		}

		return SetupState::Master;
	}

	//--------------------------------------------------------------
	void Gui::exit()
	{
        // Exit engine
		if(isContextOwned){

			// Unregister the afterDraw() callback (if used)
			autoDrawListener.unsubscribe();

			ImGui::SetCurrentContext(context->imguiContext);
            engine.exit();

			// Theme
			if (theme)
			{
				delete theme;
				theme = nullptr;
			}
        }

		// Textures (slaves can have some too)
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
		if(isContextOwned){
			// What if slaves exit() after the master ?
			// Todo: maybe leave it alive ? Transfer ownership to slave ?
			if(context!=nullptr){

				// Unregister
				ofAppBaseWindow* myOfWindow = imguiContexts.findKey(context);
				if(myOfWindow!=nullptr){
					imguiContexts.remove(myOfWindow);
				}
				else {
					ofLogWarning("Gui::exit()") << "Master could not unregister the context !";
				}

#ifdef OFXIMGUI_DEBUG
			ofLogNotice("Gui::exit()") << "Destroyed master context" << context << " that was bound to window " << myOfWindow << " (together with ImGuiContext" << context->imguiContext << ").";
#endif

				// Destroy
				ImGui::DestroyContext(context->imguiContext);

				// Set context to null so that slaves can know the context is gone.
				context->imguiContext = nullptr;

				isContextOwned = false;
				delete context;
			}
        }
		else {
#ifdef OFXIMGUI_DEBUG
			ofLogNotice("Gui::exit()") << "Destroyed slave context " << context << ".";
#endif

			// Slaves set their context to nullptr without destroying anything
			context = nullptr;
		}
		
	}

    //--------------------------------------------------------------
	// In some rare cases you might wish to enable this manually. (in most cases it's automatic)
    void Gui::setSharedMode(bool _sharedMode) {
		if(!_sharedMode || !context || !context->imguiContext) return;
		context->isShared = true;
    }
	// Returns true if the context is setup in shared mode
	// (when multiple Gui instances share the same context).
    bool Gui::isInSharedMode() const {
		if( !context ) return false;
		return context->isShared;
    }

    //--------------------------------------------------------------
    bool Gui::setDefaultFont(int indexAtlasFont) {
        if(context==nullptr){
            ofLogWarning() << "You must load fonts after gui.setup() ! (ignoring this call)";
            return false;
        }

		ImGui::SetCurrentContext(context->imguiContext);
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

		ImGui::SetCurrentContext(context->imguiContext);
        ImGuiIO& io = ImGui::GetIO();

		// Don't override default font with nullptr
		if( _atlasFont != nullptr ){
            for(int i=0; i<io.Fonts->Fonts.size(); ++i){
                if(io.Fonts->Fonts[i] == _atlasFont){
                    // Set font
                    io.FontDefault = _atlasFont;
					return true;
                }
            }
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

		ImGui::SetCurrentContext(context->imguiContext);
		ImGuiIO& io = ImGui::GetIO();
        std::string filePath = ofFilePath::getAbsolutePath(fontPath);

        // ensure default font gets loaded once
        if(io.Fonts->Fonts.size()==0) io.Fonts->AddFontDefault();

        ImFont* font = io.Fonts->AddFontFromFileTTF(filePath.c_str(), fontSize, _fontConfig, _glyphRanges);

		if (io.Fonts->Fonts.size() > 0) {
            io.Fonts->Build();
            if( engine.updateFontsTexture() ){
                // Set default font when there's none yet, or as requested
                if(_setAsDefaultFont || io.FontDefault == nullptr) setDefaultFont(font);
                return font;
            }
            else return nullptr;
		}
		return nullptr;
	}
	//--------------------------------------------------------------
	ImFont* Gui::addFontFromMemory(void* fontData, int fontDataSize, float fontSize, const ImFontConfig* _fontConfig, const ImWchar* _glyphRanges, bool _setAsDefaultFont ) {

		if(context==nullptr){
		  ofLogWarning() << "You must load fonts after gui.setup() ! (ignoring this call)";
		  return nullptr;
		}

		//ImFontConfig structure allows you to configure oversampling.
		//By default OversampleH = 3 and OversampleV = 1 which will make your font texture data 3 times larger
		//than necessary, so you may reduce that to 1.

		ImGui::SetCurrentContext(context->imguiContext);
		ImGuiIO& io = ImGui::GetIO();

		// ensure default font gets loaded once
		if(io.Fonts->Fonts.size()==0) io.Fonts->AddFontDefault();

		ImFont* font = io.Fonts->AddFontFromMemoryTTF( fontData, fontDataSize, fontSize, _fontConfig, _glyphRanges);

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

		ImGui::SetCurrentContext(context->imguiContext);
		theme->setup();

        // Source : https://github.com/yumataesu/ofxImGui_v3/blob/23ff3e02ae3b99cb3db449b950c2f3e34424fbc8/src/ofxImGui.cpp#L12-L18
        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
		if(
				// Viewports are enabled
				(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				// Platform&Renderer backends both support viewports
				&& (ImGui::GetIO().BackendFlags & (ImGuiBackendFlags_PlatformHasViewports))
				&& (ImGui::GetIO().BackendFlags & (ImGuiBackendFlags_RendererHasViewports))
		){
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
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
		if( context->isRenderingFrame == true ){
            // Already began context
			if(!context->isShared){
				// Notify misuse if shared context is off
				static bool userWasWarned = false;
				if(!userWasWarned){
					ofLogWarning("Gui::begin()") << "The frame already began, and the context is not shared, you're probably not orchestrating begin/end calls correctly !";
					userWasWarned = true;
				}
			}
            return;
        }

		ImGui::SetCurrentContext(context->imguiContext);

        // Help people loading fonts incorrectly
        ImGuiIO& io = ImGui::GetIO();
        IM_ASSERT( io.Fonts->IsBuilt() );

        //std::cout << "New Frame in context " << context << " in window " << ofGetWindowPtr() << " (" << ofGetWindowPtr()->getWindowSize().x << ")" << std::endl;
        engine.newFrame();
        ImGui::NewFrame();

		context->isRenderingFrame = true;
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
		if( context->isShared==true ){
#ifdef OFXIMGUI_DEBUG
			if( !context->isRenderingFrame ){
                ofLogWarning("Gui::end()") << "The Gui already rendered, or forgot to call Gui::Begin() ! Please ensure you render the gui after other instances have rendered.";
            }
#endif
            return;
        }


        // Only render in autodraw mode.
		if(context->autoDraw){
            // Protection : don't endFrame() twice.
			if( !context->isRenderingFrame ){
                return;
            }

            // End submitting to ImGui
            //engine.endFrame();
            ImGui::EndFrame();

            //render(); // Now called after ofApp::draw() using a callback.
        }
        // Otherwise end the frame. User chooses when to render (later) using Gui::draw()
        // Note: You cannot resume using ImGui::NewFrame() without flushing the pipeline.
        else {
            // Protection : don't endFrame() twice.
			if( !context->isRenderingFrame ){
                return;
            }

            //engine.endFrame(); // (Does nothing...)
            ImGui::EndFrame();
        }
    }

    //--------------------------------------------------------------
    // Note: render() is private, never called by the end user.
    void Gui::render(){
        if( context==nullptr ) return;

		ImGui::SetCurrentContext(context->imguiContext);
        ImGui::Render();
        engine.render();

		context->isRenderingFrame = false;
    }

	//--------------------------------------------------------------
    void Gui::draw()
	{
		if(!context) return;

        // Just for clarity, we require draw() not to be used in autoDraw mode.
		if( context->autoDraw ){
#ifdef OFXIMGUI_DEBUG
            ofLogWarning("ofxImGui::Gui::draw()") << "Please don't call draw() in autoDraw mode.";
#endif
            return;
        }
		else if(context->isRenderingFrame==true){
            render();
		}
	}


    void Gui::afterDraw( ofEventArgs& ){

        // This function is registered after ofApp::draw() to honor autodraw in shared context mode.
		if(context && context->isRenderingFrame ){
            // Autodraw renders here if sharedMode is on
			if( (context->autoDraw && context->isShared==true) ) render();
            // Render if manual render was forgotten (do we want this to happen??)
			else if( context->isShared==false ) render();
        };
    }

    // Initialise statics
	LinkedList<ofAppBaseWindow, ofxImGuiContext> Gui::imguiContexts = {};
}

