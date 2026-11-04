#include "Gui.h"

#include "ofAppRunner.h"

#include "ofxImGuiConstants.h"
#include "imgui_internal.h" // <-- advanced docking features from imgui internals...

// Includes required for debug builds
#ifdef OFXIMGUI_DEBUG

// KeyPress input debug helper
void appendCharToPressedChars(const char theChar, char (&lastPressedChars)[OFXIMGUI_DEBUG_INPUT_CHARS_LEN]){
    if(theChar < ' ' || theChar > '~') return; // ignore invisible chars
    unsigned int i = 0;
    for(char& c : lastPressedChars){
        // leave last (ending) char alone, it's a delimiter
        if(i==OFXIMGUI_DEBUG_INPUT_CHARS_LEN-1) break;
        if(i+2<sizeof(lastPressedChars)) c = lastPressedChars[i+1];
        else c = theChar;
        i++;
    }
}
#endif // OFXIMGUI_DEBUG


// Todo: Texture/Pixel loading functions: Stop using ofDisableArbTex and use appropriate textureSettings instead.

namespace ofxImGui
{
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

        // Fixme: This should be set false between CreateContext() and setup() !
        // Disable tooltips until setup() is called, they may crash the app otherwise.
        // ImGuiIO& io = ImGui::GetIO();
        // io.ConfigErrorRecoveryEnableTooltip = false;
    }

	//--------------------------------------------------------------
	Gui::~Gui()
	{
		exit();
	}

	// Retro-compatibility with older setup functions
	SetupState Gui::setup(BaseTheme* theme_, bool autoDraw_, ImGuiConfigFlags customFlags_, bool _restoreGuiState, bool _showImGuiMouseCursor )
	{
		auto window = ofGetCurrentWindow();
		IM_ASSERT( window && window != nullptr && "You must call setup() from within an active ofAppBaseWindow, or provide a pointer to it." );
		return setup( window, theme_, autoDraw_, customFlags_, _restoreGuiState, _showImGuiMouseCursor);
	}

	//--------------------------------------------------------------
    // Must be called from the oF window that's gonna display the gui.
    // Creates 1 imgui context per oF window (glfw context).
    // Links the context handle to the correct window's imguicontext.
    // Todo: an optional ofAppBaseWindow* to specify a manual context instead of using the current active ofApp window.
	// Returns a state indcating if the setup happened as slave or master
	SetupState Gui::setup(std::shared_ptr<ofAppBaseWindow>& _ofWindow, BaseTheme* theme_, bool autoDraw_, ImGuiConfigFlags customFlags_, bool _restoreGuiState, bool _showImGuiMouseCursor )
	{
#ifdef OFXIMGUI_DEBUG
		ofLogVerbose("Gui::setup()") << "Setting up ofxImGui [" << this << "] in window " << _ofWindow.get();
#endif
        // Instance already setup ?
		if( context!=nullptr ){
#ifdef OFXIMGUI_DEBUG
            ofLogWarning("Gui::setup()") << "This Gui instance is already setup, you are calling it twice ! (ignoring this 2nd call)";
#endif
			return isContextOwned ? SetupState::Master : SetupState::Slave;
        }

        // Curwindow cannot be null
		if(_ofWindow==nullptr){
			ofLogError("Gui::setup()") << "The provided ofAppBaseWindow pointer is null, cannot continue setup !";
			return SetupState::Error;
		}

        // Initial docking viewport
        dockingViewport = ofRectangle(0, 0, _ofWindow->getWidth(), _ofWindow->getHeight());

		// Grab existing ImGui Context
		//ImGuiContext* existingImGuiContext = ImGui::GetCurrentContext(); // Null on first call ever

		// Does this ofBaseWindow already have an ofxImGuiContext ?
		ofxImGuiContext* existingOfWindowContext = nullptr;
		auto foundWindowContext = imguiContexts.find( _ofWindow.get() );
		if( foundWindowContext != imguiContexts.end() ) existingOfWindowContext = &foundWindowContext->second ;

		if( existingOfWindowContext != nullptr ){
			existingOfWindowContext->slaveCount++; // tells master that the context became shared

			// Keep reference of master context
			context = existingOfWindowContext;
			isContextOwned = false;

#ifdef OFXIMGUI_DEBUG
			ofLogNotice("Gui::setup()") << "Context " << context << "/" << context->imguiContext << " already exists in window " << _ofWindow.get() << ", using the existing context as a shared one.";
			if(autoDraw_) ofLogWarning("Gui::setup()") << "You requested to enable Autodraw, but this is a Slave instance. Not enabling autoDraw.";
#endif
        }
        // Create a unique context for this window
        else {
			//context = new ofxImGuiContext(_ofWindow);
//			std::pair<ofAppBaseWindow*, ofxImGuiContext> myPair(std::piecewise_construct, std::forward_as_tuple(_ofWindow.get()), std::forward_as_tuple(_ofWindow));
			//std::pair<ofAppBaseWindow*, ofxImGuiContext> myPair(std::piecewise_construct, std::forward_as_tuple(_ofWindow.get()), std::forward_as_tuple(_ofWindow));

			// Register
			//imguiContexts.emplace(_ofWindow.get(), _ofWindow);
			//imguiContexts[_ofWindow.get()] = ofxImGuiContext(_ofWindow);
			auto createdContext = imguiContexts.emplace(std::piecewise_construct, std::forward_as_tuple(_ofWindow.get()), std::forward_as_tuple(_ofWindow));
			if(!createdContext.second){ // Failed creating ?
				ofLogError("Gui::setup()") << "Couldn't create a context for this window !";
				return SetupState::Error;
			}
			context = &createdContext.first->second;
			context->slaveCount ++; // Master counts as a slave too ;)
			isContextOwned = true;

			// Enable autodraw
			if( autoDraw_ && _ofWindow!=nullptr ){
				context->autoDraw = true;
				autoDrawListener = _ofWindow->events().draw.newListener( this, &Gui::autoDraw, OF_EVENT_ORDER_AFTER_APP );
            }

#ifdef OFXIMGUI_DEBUG
			ofLogVerbose("Gui::setup()") << "Created context "<< context << " in window " << _ofWindow.get() << " ["<< ofGetWindowPtr()->getWindowSize() <<"]";
#endif
        }

		// Set context and grab IO
		ImGui::SetCurrentContext(context->imguiContext);
        ImGuiIO& io = ImGui::GetIO();

		// Dummy call that will crash if the io is invalid --> ie for easier debugging
#ifdef OFXIMGUI_DEBUG
		(void)io;
#endif

        // Note : In chaining mode, additional flags can still be set.
        io.ConfigFlags |= customFlags_;

        // Inject touch flags by default on touch-only platforms
#if defined(OFXIMGUI_TOUCH_EVENTS) && !defined(OFXIMGUI_TOUCH_EVENTS_AUTO_CONFIG)
        io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
#endif

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
		if(_restoreGuiState == false)
			io.IniFilename = nullptr;
		else
			io.IniFilename = iniFileName.c_str();

		// Start engines
		this->context->engine.setup( _ofWindow.get(), context->imguiContext, context->autoDraw);

		// Load a default font
		// Fixme: make this optional ? (= less GPU memory).
		// For now we ensure a default font is always available, for the ease of use.
		ImFont* defaultFont = nullptr;
		if(io.Fonts->Fonts.size()==0){
#ifndef IMGUI_DISABLE_DEFAULT_FONT
			//defaultFont = io.Fonts->AddFontDefault();
			ImFontConfig cfg;
			cfg.SizePixels = 13.0f;
			defaultFont = io.Fonts->AddFontDefault(&cfg);

            // Set as default
            if(defaultFont){
#if IMGUI_VERSION_NUM < 1920
                rebuildFontsTexture(); // Fixme: could be an optional call to improve loading speeds (only needs to be called once after loading all fonts).
#endif

                // Ensure the default font is marked default (can be user-overridden later)
                if(!io.FontDefault){
                    //io.FontDefault = defaultFont;
                    setDefaultFont(defaultFont);
                }
            }
#else
            IM_ASSERT("No font is not set and the default ImGui font has been disabled !");
#endif
        }

		// Load a theme
		if (theme_)
		{
			setTheme(theme_);
		}
		else
		{
			DefaultTheme* defaultTheme = new DefaultTheme();
			setTheme((BaseTheme*)defaultTheme);
		}

#ifdef OFXIMGUI_DEBUG
		// Start keypress input debug helper
		if(!bDebugWindowBoundToOF){
			ofAddListener(ofEvents().keyPressed, this, &Gui::recordOfKeyPresses, OF_EVENT_ORDER_BEFORE_APP);
			bDebugWindowBoundToOF = true;
		}
#endif

		return SetupState::Master;
	}

	//--------------------------------------------------------------
	void Gui::setAutoContentScale(bool enabled)
	{
		autoContentScale = enabled;
	}

	//--------------------------------------------------------------
	void Gui::exit()
	{
        // Exit engine
		if(isContextOwned){

			// Unregister the afterDraw() callback (if used)
			autoDrawListener.unsubscribe();

			ImGui::SetCurrentContext(context->imguiContext);
			this->context->engine.exit();

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
                loadedTextures[i] = nullptr;
            }
		}
		loadedTextures.clear();

        // Destroy context
		if(isContextOwned){
			// What if slaves exit() after the master ?
			// Todo: maybe leave it alive ? Transfer ownership to slave ?
			if(context!=nullptr){

				// Unregister
//				ofAppBaseWindow* myOfWindow = imguiContexts.find(context);
//				if(myOfWindow!=nullptr){
//					imguiContexts.remove(myOfWindow);
//				}
//				else {
//					ofLogWarning("Gui::exit()") << "Master could not unregister the context !";
//				}

#ifdef OFXIMGUI_DEBUG
//			ofLogNotice("Gui::exit()") << "Destroyed master context " << context << " that was bound to window " << myOfWindow << " (together with ImGuiContext" << context->imguiContext << ").";
#endif

				// Destroy
				ImGui::DestroyContext(context->imguiContext);

				// Set context to null so that slaves can know the context is gone.
				context->imguiContext = nullptr;

				isContextOwned = false;
				//delete context;
			}
        }
		else {
#ifdef OFXIMGUI_DEBUG
			ofLogNotice("Gui::exit()") << "Destroyed slave context " << context << ".";
#endif

			// Slaves set their context to nullptr without destroying anything
			context = nullptr;
		}

		// Unbind debug event listeners
#ifdef OFXIMGUI_DEBUG
		if(bDebugWindowBoundToOF){
			ofRemoveListener(ofEvents().keyPressed, this, &Gui::recordOfKeyPresses, OF_EVENT_ORDER_BEFORE_APP);
		}
#endif
	}

    //--------------------------------------------------------------
	// In some rare cases you might wish to enable this manually. (in most cases it's automatic)
//    void Gui::setSharedMode(bool _sharedMode) {
//		if(!_sharedMode || !context || !context->imguiContext) return;
//		context->slaveCount++;
//    }
	// Returns true if the context is setup in shared mode
	// (when multiple Gui instances share the same context).
    bool Gui::isInSharedMode() const {
		if( !context ) return false;
		return context->isShared();
    }

    bool Gui::isMaster() const {
        return isContextOwned;
    }

    //--------------------------------------------------------------
    bool Gui::setDefaultFont(int indexAtlasFont) {
        if(context==nullptr){
            ofLogWarning("Gui::setDefaultFont()") << "You must load fonts after gui.setup() ! (ignoring this call)";
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
            ofLogWarning("Gui::setDefaultFont") << "You must load fonts after gui.setup() ! (ignoring this call)";
            return false;
        }

		ImGui::SetCurrentContext(context->imguiContext);
        ImGuiIO& io = ImGui::GetIO();

		// Don't override default font with nullptr
		if( _atlasFont != nullptr ){
			// Loop existing fonts to ensure setting an available font
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
			ofLogWarning("Gui::addFont()") << "You must load fonts after gui.setup() ! (ignoring this call)";
			return nullptr;
		}

		// ImFontConfig structure allows you to configure oversampling.
		// By default OversampleH = 3 and OversampleV = 1 which will make your font texture data 3 times larger
		// than necessary, so you may reduce that to 1.

		ImGui::SetCurrentContext(context->imguiContext);
		ImGuiIO& io = ImGui::GetIO();

		std::string absDataPath = ofFilePath::getAbsolutePath(fontPath, true);
		// Within data folder (=original ofxImGui AddFont() behaviour, for compatibility)
		const bool absDataPathExists = ofFile(absDataPath).exists();
		// We still want to sanitize the path
		// (as imgui can be difficult with some path notations such as `~/documents/myfont.ttf`)
		// This addon is used in different versions of OF, non-data paths should remain possible.
		// Especially the raspberry seems to have different path logics
		std::string filePath = absDataPathExists?absDataPath:ofFilePath::getAbsolutePath(fontPath, false);

        ImFont* font = io.Fonts->AddFontFromFileTTF(filePath.c_str(), fontSize, _fontConfig, _glyphRanges);

		if (font != nullptr){
#if IMGUI_VERSION_NUM < 1920
			// Fixme: could be an optional call to improve loading speeds (only needs to be called once after loading all fonts).
			rebuildFontsTexture();
#endif

			if(_setAsDefaultFont) setDefaultFont(font);

			return font;
		}

		// delete font; // Nope, handled by ImGui !

		return nullptr;
	}
	//--------------------------------------------------------------
	ImFont* Gui::addFontFromMemory(void* fontData, int fontDataSize, float fontSize, const ImFontConfig* _fontConfig, const ImWchar* _glyphRanges, bool _setAsDefaultFont ) {

		if(context==nullptr){
		  ofLogWarning("Gui::addFontFromMemory()") << "You must load fonts after gui.setup() ! (ignoring this call)";
		  return nullptr;
		}

		// ImFontConfig structure allows you to configure oversampling.
		// By default OversampleH = 3 and OversampleV = 1 which will make your font texture data 3 times larger
		// than necessary, so you may reduce that to 1.

		ImGui::SetCurrentContext(context->imguiContext);
		ImGuiIO& io = ImGui::GetIO();


		ImFont* font = io.Fonts->AddFontFromMemoryTTF( fontData, fontDataSize, fontSize, _fontConfig, _glyphRanges);

		if (font != nullptr){
#if IMGUI_VERSION_NUM < 1920
			// Fixme: could be an optional call to improve loading speeds (only needs to be called once after loading all fonts).
			rebuildFontsTexture();
#endif
			if(_setAsDefaultFont) setDefaultFont(font);
			return font;
		}
		else {
			return nullptr;
		}
	}

	//--------------------------------------------------------------
	bool Gui::rebuildFontsTexture(){
#if IMGUI_VERSION_NUM >= 19200
		// Note: could be forced with :
		// ImFontAtlasBuildClear(ImGui::GetIO().Fonts); // Rebuild fonts
		return true; // not needed anymore = done !
#else
		if(context==nullptr){
		  ofLogWarning("Gui::rebuildFontsTexture()") << "You must build fonts after gui.setup() ! (ignoring this call)";
		  return false;
		}

		ImGui::SetCurrentContext(context->imguiContext);
		ImGuiIO& io = ImGui::GetIO();

		if (io.Fonts->Fonts.size() > 0) {
			io.Fonts->Build();
			return context->engine.updateFontsTexture();
		}
		return false;
#endif
	}

	//--------------------------------------------------------------
	void Gui::setTheme(BaseTheme* theme_)
	{
		// Handle ownership of the previous theme (we do take ownership)
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
		return context->engine.loadTextureImage2D(pixels.getData(), pixels.getWidth(), pixels.getHeight());
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
		// Note: illegal : loads pixels to texture, and texture is never kept ?!
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
		const bool isUsingArb = ofGetUsingArbTex();
		if (isUsingArb)
		{
			ofDisableArbTex();
		}
		ofTexture* texture = new ofTexture();
		ofLoadImage(*texture, imagePath);
		if (isUsingArb)
		{
			ofEnableArbTex();
		}
		loadedTextures.push_back(texture);
		return texture->getTextureData().textureID;
	}

	//--------------------------------------------------------------
	GLuint Gui::loadTexture(ofTexture& texture, const std::string& imagePath)
	{
		const bool isUsingArb = ofGetUsingArbTex();
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
			if(!context->isShared()){
				// Notify misuse if shared context is off
				static bool userWasWarned = false;
				if(!userWasWarned){
					ofLogWarning("Gui::begin()") << "The frame already began, and the context is not shared, you're probably not orchestrating begin/end calls correctly, or you forgot to call gui.draw() in manual draw mode !";
					userWasWarned = true;
				}
			}
            return;
        }

		ImGui::SetCurrentContext(context->imguiContext);

#if IMGUI_VERSION_NUM < 19190
        // Help people loading fonts incorrectly
        ImGuiIO& io = ImGui::GetIO();
        IM_ASSERT( io.Fonts->IsBuilt() ); // Fonts incorrectly setup
        //IM_ASSERT( io.Fonts->Fonts.Size > 0 && io.Fonts->TexIsBuilt); // Fonts incorrectly setup
#endif

        //std::cout << "New Frame in context " << context << " in window " << ofGetWindowPtr() << " (" << ofGetWindowPtr()->getWindowSize().x << ")" << std::endl;
		context->engine.newFrame();
        ImGui::NewFrame();

        // Sync IO debug chars
#ifdef OFXIMGUI_DEBUG
        if(ImGui::GetIO().InputQueueCharacters.Size > 0) for(ImWchar c : ImGui::GetIO().InputQueueCharacters){
            appendCharToPressedChars(static_cast<char>(c), lastPressedCharsIM);
        }
#endif

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

		// Let context open in shared mode. (automatically called in the native render function)
		if( context->isShared()==true ){
#ifdef OFXIMGUI_DEBUG
			if( !context->isRenderingFrame ){
				ofLogWarning("Gui::end()") << "The Gui already rendered, or you forgot to call Gui::Begin() ! Please ensure you render the gui after other instances have rendered.";
            }
#endif
            // Todo: if user doesn't gui.draw() (autodraw=0), ImGui::EndFrame() is not called, causing a "forgot to BeginFrame()" assert
            return;
        }

        // Update cached variables
        updateDockingVp();
        // Update height
        ImGuiWindow* menuWin = ImGui::FindWindowByID(ImGui::GetIDWithSeed("##MainMenuBar", nullptr, 0));
        menuHeight = (menuWin && !menuWin->Hidden && menuWin->Active) ? ImGui::GetFrameHeight() : 0;

        // Only render in autodraw mode.
		if(context->autoDraw){
            // Protection : don't endFrame() twice.
			if( !context->isRenderingFrame ){
                return;
            }

            // End submitting to ImGui
			//context->engine.endFrame();
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

			//context->engine.endFrame(); // (Does nothing...)
            ImGui::EndFrame();
        }
    }

    //--------------------------------------------------------------
    // Note: render() is private, never called by the end user.
    void Gui::render(){
        if( context==nullptr ) return;

        ofEventArgs a;
        beforeDraw.notify(a);

		ImGui::SetCurrentContext(context->imguiContext);
        ImGui::Render();
		context->engine.render();
		context->isRenderingFrame = false;

        afterDraw.notify(a);
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

	//--------------------------------------------------------------
	void Gui::autoDraw( ofEventArgs& ){

        // This function is registered after ofApp::draw() to honor autodraw in shared context mode.
		if(context && context->isRenderingFrame ){
            // Autodraw renders here if sharedMode is on
			if( context->autoDraw && context->isShared()) render();
            // Render if manual render was forgotten (do we want this to happen??)
			else if( !context->isShared() ) render();
        };
    }

	//--------------------------------------------------------------
	void Gui::updateDockingVp(){
		static ofRectangle dockingVpCached = ofGetWindowRect();

        // Only when docking is enabled
        if(bool(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)){
            // Note: same as defalut ID returned by ImGui::DockSpaceOverViewport(0, NULL, dockingFlags);
            // A bit complicated to reverse, we have to grab the hardcoded IDs

            // Like ImGui::DockSpaceOverViewport
            char label[32];
            ImFormatString(label, IM_ARRAYSIZE(label), "WindowOverViewport_%08X", ImGui::GetMainViewport()->ID);

            // Get docking host window
            ImGuiWindow* window = ImGui::FindWindowByName(label);
            if(window){
                // Like ImGui::DockSpaceOverViewport, but within a non-current window
                const char dockSpaceId[] = "DockSpace"; // ImGui version : ImGui::GetID("DockSpace");
                ImGuiID dockNodeID = ImGui::GetIDWithSeed(dockSpaceId, nullptr, window->ID);

                ImGuiDockNode* dockNode = ImGui::DockBuilderGetNode(dockNodeID);

                // Only use dockspace if currently visible
                if(dockNode && dockNode->IsVisible){
                    ImGuiDockNode* centralNode = ImGui::DockBuilderGetCentralNode(dockNodeID);

                    // Verifies if the central node is empty (visible empty space for oF)
                    if( centralNode && centralNode->IsEmpty() ){
                        ImRect availableSpace = centralNode->Rect();

                        // Detect change ?
                        if(
                            dockingVpCached.x != availableSpace.Min.x ||
                            dockingVpCached.y != availableSpace.Min.y ||
                            dockingVpCached.width != availableSpace.GetWidth() ||
                            dockingVpCached.height != availableSpace.GetHeight()
                        ){
                            // Update viewport
                            dockingVpCached.x = availableSpace.Min.x;
                            dockingVpCached.y = availableSpace.Min.y;
                            dockingVpCached.width = availableSpace.GetWidth();
                            dockingVpCached.height = availableSpace.GetHeight();
                        }

                        dockingViewport = dockingVpCached;

                        // Normalise data according to the different ImGui coordinate spaces
                        // Depending on the viewports flag, the XY is either absolute or relative to the oF window.
                        if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable ){
                            dockingViewport.position.x -= ofGetWindowPositionX();
                            dockingViewport.position.y -= ofGetWindowPositionY();
                        }

                        // Success (don't set default below)
                        return;
                    }
                }
            }
        }

		// Set dockingVP to ofWindow when docking unactive
		dockingViewport = ofGetWindowRect();
	}


	ofRectangle Gui::getMainWindowViewportRect(bool returnScreenCoords, bool removeMenuBar, bool removeDockingAreas) const {

		// Handle menubar
		ofRectangle rect = ofGetWindowRect();

		// Subtract menubar
		if(removeMenuBar){
			rect.y += menuHeight;
			rect.height -= menuHeight;
		}

		// When docking is active, it already excludes the menubar
		if(removeDockingAreas && ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable){
			rect = dockingViewport;
		}

		// Make them absolute ?
		if(returnScreenCoords){
			rect.position.x += ofGetWindowPositionX();
			rect.position.y += ofGetWindowPositionY();
		}

		return rect;
	};

	int Gui::getMenuHeight() const {
		return menuHeight;
	}

	ofRectangle Gui::getDockingViewport() const {
		return dockingViewport;
	}

	bool Gui::isAutoDrawEnabled() const {
		return context->autoDraw;
	}

	bool Gui::wantsCaptureMouse() const {
		return ImGui::GetIO().WantCaptureMouse;
	}

	bool Gui::wantsCaptureKeyboard() const {
		return ImGui::GetIO().WantCaptureKeyboard;
	}

    // Initialise statics
	//LinkedList<ofAppBaseWindow, ofxImGuiContext> Gui::imguiContexts = {};
	std::unordered_map<ofAppBaseWindow*, ofxImGuiContext> Gui::imguiContexts = {};

#ifdef OFXIMGUI_DEBUG
	void Gui::recordOfKeyPresses(ofKeyEventArgs &args){
		if(bInputDebugIgnoreWhenImGuiActive && ImGui::GetIO().WantCaptureKeyboard) return;
		if(!args.isRepeat) appendCharToPressedChars(args.key, lastPressedCharsOF);
	}
	bool Gui::bInputDebugIgnoreWhenImGuiActive = true;
#endif
}

