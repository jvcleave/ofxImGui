#include "EngineOpenFrameworks.h"

// Custom backend include, only for getting keycodes, improving keyboard input
#if defined (TARGET_GLFW_WINDOW)
#include "ofAppGLFWWindow.h"
//#include "backends/imgui_impl_glfw.h"
#include "GLFW/glfw3.h" // needed by rpi
#endif

// Renderer includes
#include "ofGLProgrammableRenderer.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_opengl2.h"

#include "imgui.h"


namespace ofxImGui
{

	// Global storage for the latest clipboard string, ensures static storage
	std::string EngineOpenFrameworks::g_ClipboardText = "";

	//--------------------------------------------------------------
	void EngineOpenFrameworks::setup(ofAppBaseWindow* _window, ImGuiContext* _imguiContext, bool autoDraw)
	{
		// Store a reference to the current imgui context for event handling
		//imguiContext = ImGui::GetCurrentContext();

		ImGuiIO& io = ImGui::GetIO();

		// Todo: this feature could be enabled and mapped to OF. Seems rarely used.
		// io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos; // Not yet supported

		// Can we change mouse cursors ? Nope, not supported out of the box by OF...
		// io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

		// Store a reference to the current imgui context for event handling
		imguiContext = _imguiContext;

		//io.AddKeyEvent();
		io.DisplaySize = ImVec2( _window->getWindowSize() );//ofGetWindowSize() );
		io.DeltaTime = 1.0f / 60.0f; // start with non-null time
		io.WantCaptureMouse = true;

		// Clipboard management
		io.SetClipboardTextFn = &EngineOpenFrameworks::setClipboardString;
		io.GetClipboardTextFn = &EngineOpenFrameworks::getClipboardString;

		// Backend name
		io.BackendPlatformName = "imgui_impl_ofximgui_native_of";
		io.BackendRendererName = "imgui_impl_opengl3";

	   // OpenGL setup
		if (ofIsGLProgrammableRenderer()){
			// Todo: Maybe better to replace with const char* glsl_version = getGLVersionFromOF(); ?
			// To be tested on multiple platforms.
#if defined( TARGET_OPENGLES )
			ImGui_ImplOpenGL3_Init( "#version 150" ); // Todo: we might need to pass another or no version string here
#else
			ImGui_ImplOpenGL3_Init();
#endif
		} else {
#if defined( TARGET_OPENGLES )
			ImGui_ImplOpenGL2_Init(); // Todo: we might need to pass a version string here
#else
			ImGui_ImplOpenGL2_Init(); // Todo: we might need to pass a version string here
#endif
		}
		// Create initial fonts texture
		updateFontsTexture();

		// Event listeners
		registerListeners();

		// We're done :)
		isSetup = true;
	} 

	//--------------------------------------------------------------
	void EngineOpenFrameworks::exit()
	{
		if (!isSetup) return;

		// Event listeners
		unregisterListeners();

		// Clear GPU data
		if (ofIsGLProgrammableRenderer()){
			ImGui_ImplOpenGL3_Shutdown();
		} else {
			ImGui_ImplOpenGL2_Shutdown();
		}

		//ImGui::DestroyContext();
		ImGuiIO& io = ImGui::GetIO();
		io.BackendPlatformName = nullptr;
		imguiContext = nullptr;
		isSetup = false;
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::newFrame()
	{
		// Need to set context here too ?

		// Set delta time
		ImGuiIO& io = ImGui::GetIO();
		float elapsed = ofGetLastFrameTime();
		io.DeltaTime = elapsed > 0.0 ? elapsed : (1.f/60.f); // Note: never be 0 !

		// Prepare graphics
		if (ofIsGLProgrammableRenderer()){
			ImGui_ImplOpenGL3_NewFrame();
		} else {
			ImGui_ImplOpenGL2_NewFrame();
		}
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::render()
	{
		// Need to set context here too ?

		// Flush imgui pipeline
		ImGui::Render();

		// Draw !
		if (ofIsGLProgrammableRenderer()){
			ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
		} else {
			ImGui_ImplOpenGL2_RenderDrawData( ImGui::GetDrawData() );
		}

	}

	//--------------------------------------------------------------
	bool EngineOpenFrameworks::updateFontsTexture(){
		// Set context ?

		// The renderer does it for us :)
		if (ofIsGLProgrammableRenderer()) {
			return ImGui_ImplOpenGL3_CreateFontsTexture();
		}
		else {
			return ImGui_ImplOpenGL2_CreateFontsTexture();
		}
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::onMouseMoved(ofMouseEventArgs& event)
	{
		// Set context
		if(!setImGuiContext()) return;

		// Update imgui mouse pos
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent(static_cast<float>(event.x), static_cast<float>(event.y));

		restoreImGuiContext();
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::onMouseDragged(ofMouseEventArgs& event)
	{
		// Set context
		if(!setImGuiContext()) return;

		// Update imgui mouse pos
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent(static_cast<float>(event.x), static_cast<float>(event.y));

		restoreImGuiContext();
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::onMouseScrolled(ofMouseEventArgs& event)
	{
		// Set context
		if(!setImGuiContext()) return;

		// Update imgui mouse pos
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent( static_cast<float>(event.scrollX), static_cast<float>(event.scrollY) );

		restoreImGuiContext();
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::onMouseButton(ofMouseEventArgs& event)
	{
		// Set context
		if(!setImGuiContext()) return;

		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(event.button, event.type == ofMouseEventArgs::Pressed);

		restoreImGuiContext();
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::onKeyEvent(ofKeyEventArgs& event)
	{
		// Ignore repeats
		if(event.isRepeat) return;

		// Set context
		if(!setImGuiContext()) return;

		int key = event.keycode; // Todo: this seems to be window specific ?
		ImGuiIO& io = ImGui::GetIO();

		// Set key modifiers (ensures they are set on time, fixes kb shortcuts)
		io.AddKeyEvent(ImGuiMod_Ctrl,  event.hasModifier(OF_KEY_CONTROL));
		io.AddKeyEvent(ImGuiMod_Shift, event.hasModifier(OF_KEY_SHIFT));
		io.AddKeyEvent(ImGuiMod_Alt,   event.hasModifier(OF_KEY_ALT));
		io.AddKeyEvent(ImGuiMod_Super, event.hasModifier(OF_KEY_SUPER));

		// Since 1.87 : Key events
		ImGuiKey imKey = oFKeyToImGuiKey(event.key);
		//ImGuiKey imgui_key = ImGui_ImplGlfw_KeyToImGuiKey(keycode); // Previous code

		// Fallback by guessing the imguikey from the typed character
		// Note: could create weird behaviour on some special keyboards ?
		// If so: It could be disabled, it doesn't prevent from using ImGui
		// This helps registering key up/down state, which is rarely used in imgui widgets.
		if(imKey == ImGuiKey_None){
			// Note: codepoint corresponds to the typed character
			imKey = keyCodeToImGuiKey( event.codepoint );
		}

		io.AddKeyEvent(imKey, event.type == ofKeyEventArgs::Pressed );

		// Note: this brings support for pre-1.87 user code using very specific API code.
		// It causes an assert/crash in imgui v1.89.3 when releasing ALT+CMD simultanously (osx+backend_of_native), when IMGUI_DISABLE_OBSOLETE_KEYIO is not defined.
		// As we disabled it, we can support old user code again.
		io.SetKeyEventNativeData(imKey, key, event.scancode); // To support legacy indexing (<1.87 user code)

		// Note: Not anymore, we do this in the charInput callback now.
		// io.AddInputCharacter((unsigned short)event.codepoint);

		restoreImGuiContext();
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::onCharInput(uint32_t& _char)
	{
		// Set context
		if(!setImGuiContext()) return;

		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter((unsigned short)_char);

		restoreImGuiContext();
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::onWindowResized(ofResizeEventArgs& window)
	{
		// Set context
		if(!setImGuiContext()) return;

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)window.width, (float)window.height);

		restoreImGuiContext();
	}

	// Constexpr ascii table math, which will be optimised by the compiler. Small overhead on debug.
//	inline constexpr char constexprCharToLower(const char c) {
//		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
//	}
//	inline constexpr char constexprCharToUpper(const char c) {
//		//return c + (c - 'a' + 'A');
//		return (c >= 'a' && c <= 'z') ? c + (c - 'a' + 'A') : c;
//	}

	//--------------------------------------------------------------
	// Utility to guess the key from the keycode, which is more universal amongst platforms
	ImGuiKey EngineOpenFrameworks::keyCodeToImGuiKey(int keyCode)
	{
#define DOUBLE_QUOTE(x) #x
#define SINGLE_QUOTE(x) DOUBLE_QUOTE(x)[0]
//#define LOWERTOUPPER(x) ((SINGLE_QUOTE(x) - 'a') + 'A')
#define UPPERTOLOWER(x) (SINGLE_QUOTE(x) + ('a' - 'A'))
#define MAKE_VARNAME(before, var) before##var
#define MAP_LETTER(upper) case SINGLE_QUOTE(upper): case UPPERTOLOWER(upper): return MAKE_VARNAME(ImGuiKey_,upper);
#define MAP_NUMBER(number) case (char)(number): return MAKE_VARNAME(ImGuiKey_,number);
#define MAP_SIGN(sign, signname) case sign: return MAKE_VARNAME(ImGuiKey_,signname);

//#define MAP_KEY(lower, upper) case lower: case upper: return ImGuiKey_##upper;
		switch (keyCode)
		{
			// a-z and A-Z
			MAP_LETTER(A)
			MAP_LETTER(B)
			MAP_LETTER(C)
			MAP_LETTER(D)
			MAP_LETTER(E)
			MAP_LETTER(F)
			MAP_LETTER(G)
			MAP_LETTER(H)
			MAP_LETTER(I)
			MAP_LETTER(J)
			MAP_LETTER(K)
			MAP_LETTER(L)
			MAP_LETTER(M)
			MAP_LETTER(N)
			MAP_LETTER(O)
			MAP_LETTER(P)
			MAP_LETTER(Q)
			MAP_LETTER(R)
			MAP_LETTER(S)
			MAP_LETTER(T)
			MAP_LETTER(U)
			MAP_LETTER(V)
			MAP_LETTER(W)
			MAP_LETTER(X)
			MAP_LETTER(Y)
			MAP_LETTER(Z)
			// 0-9
			MAP_NUMBER(0)
			MAP_NUMBER(1)
			MAP_NUMBER(2)
			MAP_NUMBER(3)
			MAP_NUMBER(4)
			MAP_NUMBER(5)
			MAP_NUMBER(6)
			MAP_NUMBER(7)
			MAP_NUMBER(8)
			MAP_NUMBER(9)
			// Some signs (todo: add more?)
			MAP_SIGN('.', Period)
			MAP_SIGN(',', Comma)
			MAP_SIGN('-', Minus)
			MAP_SIGN('\'',Apostrophe)
			MAP_SIGN('/', Slash)
			MAP_SIGN(';', Semicolon)
			MAP_SIGN('=', Equal)
			MAP_SIGN('[', LeftBracket)
			MAP_SIGN('\\',Backslash)
			MAP_SIGN(']', RightBracket)
			MAP_SIGN(' ', Space)
		}
		return ImGuiKey_None;
	}

	// Maps OF keys to ImGui keys
	// Openframeworks keys are platform-specific
	ImGuiKey EngineOpenFrameworks::oFKeyToImGuiKey(int key)
	{
		switch (key)
		{
			// Openframeworks keycodes are platform and windowing system dependant.
			// But there are some universal ones
			case OF_KEY_TAB:       return ImGuiKey_Tab;
			case OF_KEY_RETURN:    return ImGuiKey_Enter;
			case OF_KEY_ESC:       return ImGuiKey_Escape;
			case OF_KEY_BACKSPACE: return ImGuiKey_Backspace;
			case OF_KEY_DEL:       return ImGuiKey_Delete;
			case OF_KEY_F1:        return ImGuiKey_F1;
			case OF_KEY_F2:        return ImGuiKey_F2;
			case OF_KEY_F3:        return ImGuiKey_F3;
			case OF_KEY_F4:        return ImGuiKey_F4;
			case OF_KEY_F5:        return ImGuiKey_F5;
			case OF_KEY_F6:        return ImGuiKey_F6;
			case OF_KEY_F7:        return ImGuiKey_F7;
			case OF_KEY_F8:        return ImGuiKey_F8;
			case OF_KEY_F9:        return ImGuiKey_F9;
			case OF_KEY_F10:       return ImGuiKey_F10;
			case OF_KEY_F11:       return ImGuiKey_F11;
			case OF_KEY_F12:       return ImGuiKey_F12;
			case OF_KEY_LEFT:      return ImGuiKey_LeftArrow;
			case OF_KEY_UP:        return ImGuiKey_UpArrow;
			case OF_KEY_RIGHT:     return ImGuiKey_RightArrow;
			case OF_KEY_DOWN:      return ImGuiKey_DownArrow;
			case OF_KEY_PAGE_UP:   return ImGuiKey_PageUp;
			case OF_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
			case OF_KEY_HOME:      return ImGuiKey_Home;
			case OF_KEY_END:       return ImGuiKey_End;
			case OF_KEY_INSERT:    return ImGuiKey_Insert;

#if defined (TARGET_GLFW_WINDOW)
			case GLFW_KEY_TAB: return ImGuiKey_Tab;
			case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
			case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
			case GLFW_KEY_UP: return ImGuiKey_UpArrow;
			case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
			case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
			case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
			case GLFW_KEY_HOME: return ImGuiKey_Home;
			case GLFW_KEY_END: return ImGuiKey_End;
			case GLFW_KEY_INSERT: return ImGuiKey_Insert;
			case GLFW_KEY_DELETE: return ImGuiKey_Delete;
			case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
			case GLFW_KEY_SPACE: return ImGuiKey_Space;
			case GLFW_KEY_ENTER: return ImGuiKey_Enter;
			case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
			case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
			case GLFW_KEY_COMMA: return ImGuiKey_Comma;
			case GLFW_KEY_MINUS: return ImGuiKey_Minus;
			case GLFW_KEY_PERIOD: return ImGuiKey_Period;
			case GLFW_KEY_SLASH: return ImGuiKey_Slash;
			case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
			case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
			case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
			case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
			case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
			case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
			case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
			case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
			case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
			case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
			case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
			case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
			case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
			case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
			case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
			case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
			case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
			case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
			case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
			case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
			case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
			case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
			case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
			case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
			case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
			case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
			case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
			case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
			case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
			case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
			case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
			case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
			case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
			case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
			case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
			case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
			case GLFW_KEY_MENU: return ImGuiKey_Menu;
			case GLFW_KEY_0: return ImGuiKey_0;
			case GLFW_KEY_1: return ImGuiKey_1;
			case GLFW_KEY_2: return ImGuiKey_2;
			case GLFW_KEY_3: return ImGuiKey_3;
			case GLFW_KEY_4: return ImGuiKey_4;
			case GLFW_KEY_5: return ImGuiKey_5;
			case GLFW_KEY_6: return ImGuiKey_6;
			case GLFW_KEY_7: return ImGuiKey_7;
			case GLFW_KEY_8: return ImGuiKey_8;
			case GLFW_KEY_9: return ImGuiKey_9;
			case GLFW_KEY_A: return ImGuiKey_A;
			case GLFW_KEY_B: return ImGuiKey_B;
			case GLFW_KEY_C: return ImGuiKey_C;
			case GLFW_KEY_D: return ImGuiKey_D;
			case GLFW_KEY_E: return ImGuiKey_E;
			case GLFW_KEY_F: return ImGuiKey_F;
			case GLFW_KEY_G: return ImGuiKey_G;
			case GLFW_KEY_H: return ImGuiKey_H;
			case GLFW_KEY_I: return ImGuiKey_I;
			case GLFW_KEY_J: return ImGuiKey_J;
			case GLFW_KEY_K: return ImGuiKey_K;
			case GLFW_KEY_L: return ImGuiKey_L;
			case GLFW_KEY_M: return ImGuiKey_M;
			case GLFW_KEY_N: return ImGuiKey_N;
			case GLFW_KEY_O: return ImGuiKey_O;
			case GLFW_KEY_P: return ImGuiKey_P;
			case GLFW_KEY_Q: return ImGuiKey_Q;
			case GLFW_KEY_R: return ImGuiKey_R;
			case GLFW_KEY_S: return ImGuiKey_S;
			case GLFW_KEY_T: return ImGuiKey_T;
			case GLFW_KEY_U: return ImGuiKey_U;
			case GLFW_KEY_V: return ImGuiKey_V;
			case GLFW_KEY_W: return ImGuiKey_W;
			case GLFW_KEY_X: return ImGuiKey_X;
			case GLFW_KEY_Y: return ImGuiKey_Y;
			case GLFW_KEY_Z: return ImGuiKey_Z;
			case GLFW_KEY_F1: return ImGuiKey_F1;
			case GLFW_KEY_F2: return ImGuiKey_F2;
			case GLFW_KEY_F3: return ImGuiKey_F3;
			case GLFW_KEY_F4: return ImGuiKey_F4;
			case GLFW_KEY_F5: return ImGuiKey_F5;
			case GLFW_KEY_F6: return ImGuiKey_F6;
			case GLFW_KEY_F7: return ImGuiKey_F7;
			case GLFW_KEY_F8: return ImGuiKey_F8;
			case GLFW_KEY_F9: return ImGuiKey_F9;
			case GLFW_KEY_F10: return ImGuiKey_F10;
			case GLFW_KEY_F11: return ImGuiKey_F11;
			case GLFW_KEY_F12: return ImGuiKey_F12;
#endif
			default: return ImGuiKey_None;
		}

		return ImGuiKey_None;
	}

	//--------------------------------------------------------------
	const char* EngineOpenFrameworks::getClipboardString(void * userData)
	{
		g_ClipboardText = ofGetWindowPtr()->getClipboardString();
		return g_ClipboardText.c_str();
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::setClipboardString(void * userData, const char * text)
	{
		g_ClipboardText = ofToString(text);
		ofGetWindowPtr()->setClipboardString(g_ClipboardText);
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::registerListeners(){
		// Mouse events
		ofAddListener(ofEvents().mouseMoved,    this, &EngineOpenFrameworks::onMouseMoved   );
		ofAddListener(ofEvents().mouseDragged,  this, &EngineOpenFrameworks::onMouseDragged );
		ofAddListener(ofEvents().mousePressed,  this, &EngineOpenFrameworks::onMouseButton  );
		ofAddListener(ofEvents().mouseReleased, this, &EngineOpenFrameworks::onMouseButton  );
		ofAddListener(ofEvents().mouseScrolled, this, &EngineOpenFrameworks::onMouseScrolled);
		// Todo: Bind touch events too ? (touchDown, touchUp, touchMoved, touchCancelled, touchDoubleTap)

		// Keyboard avents
		ofAddListener(ofEvents().keyReleased,   this, &EngineOpenFrameworks::onKeyEvent );
		ofAddListener(ofEvents().keyPressed,    this, &EngineOpenFrameworks::onKeyEvent );
		ofAddListener(ofEvents().charEvent,     this, &EngineOpenFrameworks::onCharInput);

		// Window Listeners
		ofAddListener(ofEvents().windowResized, this, &EngineOpenFrameworks::onWindowResized);
		//ofAddListener(ofEvents().windowMoved, this, &EngineOpenFrameworks::onWindowResized); // Seems unnecssary

		// Additional mouse data
		ofAddListener(ofEvents().mouseEntered, this, &EngineOpenFrameworks::onMouseMoved);
		ofAddListener(ofEvents().mouseExited , this, &EngineOpenFrameworks::onMouseMoved);

		// ImGui also has io.AddFocusEvent but OF hasn't got them.
	}

	//--------------------------------------------------------------
	void EngineOpenFrameworks::unregisterListeners(){
		// Mouse events
		ofRemoveListener(ofEvents().mouseMoved,     this, &EngineOpenFrameworks::onMouseMoved   );
		ofRemoveListener(ofEvents().mouseDragged,   this, &EngineOpenFrameworks::onMouseDragged );
		ofRemoveListener(ofEvents().mousePressed,   this, &EngineOpenFrameworks::onMouseButton  );
		ofRemoveListener(ofEvents().mouseReleased,  this, &EngineOpenFrameworks::onMouseButton  );
		ofRemoveListener(ofEvents().mouseScrolled,  this, &EngineOpenFrameworks::onMouseScrolled);

		// Keyboard avents
		ofRemoveListener(ofEvents().keyReleased,    this, &EngineOpenFrameworks::onKeyEvent );
		ofRemoveListener(ofEvents().keyPressed,     this, &EngineOpenFrameworks::onKeyEvent );
		ofRemoveListener(ofEvents().charEvent,      this, &EngineOpenFrameworks::onCharInput);

		// Window Listeners
		ofRemoveListener(ofEvents().windowResized,  this, &EngineOpenFrameworks::onWindowResized);

		// Additional mouse data
		ofRemoveListener(ofEvents().mouseEntered, this, &EngineOpenFrameworks::onMouseMoved);
		ofRemoveListener(ofEvents().mouseExited , this, &EngineOpenFrameworks::onMouseMoved);
	}
}

