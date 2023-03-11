#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

namespace ImGuiEx {

    void ShowHelpMarker(const char* desc);
    void ShowWarningMarker(const char* desc);
}

class ofApp : public ofBaseApp{

	public:
		ofApp() : v(0) {

		}

        void setup() override {
			gui.setup(nullptr, false, ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable);
		}
		void draw() override {
            // Warning message
            #define FONT_HEIGHT 18
            #define FONT_MARGIN 8
            int height = ofGetHeight()-FONT_MARGIN-(FONT_HEIGHT*4);
            ofDrawBitmapStringHighlight("Known bugs :", FONT_MARGIN, height); height+=FONT_HEIGHT;
			ofDrawBitmapStringHighlight(" - Viewports and docking features work, but only within the same oF window.\n   (they are isolated)", FONT_MARGIN, height); height+=FONT_HEIGHT*2;

            // Debug of vs imgui events
            height = ofGetHeight()-FONT_MARGIN-(FONT_HEIGHT*5);
            if(letterToDraw.second > 0){
                ofDrawBitmapStringHighlight(ofToString(letterToDraw.first), FONT_MARGIN, height); height-=FONT_HEIGHT;
                --letterToDraw.second;
            }
            if(mouseButtonToDraw.second > 0){
                ofDrawBitmapStringHighlight(ofToString(mouseButtonToDraw.first), FONT_MARGIN, height); height-=FONT_HEIGHT;
                --mouseButtonToDraw.second;
            }
            ofDrawBitmapStringHighlight(latestKeysPressed, FONT_MARGIN, height); height-=FONT_HEIGHT;

            // Draw to ImGui (after OF related stuff, to make it overlay)
			gui.begin();

			gui.drawOfxImGuiDebugWindow();

			if(ImGui::IsMouseDown(ImGuiMouseButton_Left)){
				mouseIsPressedImgui = true;
			}
			if(ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
				mouseIsPressedImgui = false;
			}

            ImGui::Begin("MultiWindow");
			ImGui::TextWrapped("Some checks you can perform to ensure everything works correctly.");
			ImGui::TextWrapped("For each check, try in both ofAppWindows.");
			ImGui::TextWrapped("If viewports are enabled, try both merged in the host window and in a standalone window.");

			ImGui::Dummy({10,10});
			ImGui::SeparatorText("Input & Variables");
			ImGui::SliderFloat("slider", &v, 0.f, 10.f);
			ImGuiEx::ShowHelpMarker("A slider with the same ID in both windows : check if they can have different values.");

			ImGui::Dummy({10,10});
			ImGui::SeparatorText("Mouse Input");

            ImGui::Button("Hoverable");
            ImGuiEx::ShowHelpMarker("Check the hover behaviour in both windows. Checks if mouse positions are correctly received.");

			ImGui::Dummy({10,10});
			ImGui::SeparatorText("Interface");

			ImGui::Text("MousePressed");
			ImGui::SameLine();
			ImGui::Checkbox("oF", &mouseIsPressedOf);
			ImGui::SameLine();
			bool tmp = ofGetMousePressed();
			ImGui::Checkbox("oFbis", &tmp);
			ImGui::SameLine();
			ImGui::Checkbox("ImGui", &mouseIsPressedImgui);
			ImGuiEx::ShowHelpMarker("Check if events are fired on all in this window, and not in the other window.");

			ImGui::Dummy({10,10});
			bool windowIsFocused = ImGui::IsWindowFocused();
			ImGui::Checkbox("ImGui Window Focus", &windowIsFocused);
			ImGuiEx::ShowHelpMarker("The checkbox should be on when this window is focused.");
			bool windowIsHovered = ImGui::IsWindowHovered();
			ImGui::Checkbox("ImGui Window Hover", &windowIsHovered);
			ImGuiEx::ShowHelpMarker("Should be on when this window is hovered.");

			ImGui::Dummy({10,10});
			ImGui::SeparatorText("Keyboard input");

			ImGui::TextWrapped("Check if you can write and erase text here, both from within the ofWindow and as a standalone pop-out viewport window.");
			static char someText[30];
			ImGui::InputText("Type here", someText,  IM_ARRAYSIZE(someText));
			ImGuiEx::ShowHelpMarker("Note: It's normal that the text is the same in both windows.");

			ImGui::Dummy({10,10});
			ImGui::Text("Keydown test: ");
			ImGuiEx::ShowHelpMarker("Note: Pressing too much keys at the same time will have random results.");
			static ImGuiKey someKeys[] =          {
				ImGuiKey_Space,     ImGuiKey_Backspace,  ImGuiKey_Enter,     ImGuiKey_Tab,
				ImGuiKey_LeftArrow, ImGuiKey_RightArrow, ImGuiKey_UpArrow,   ImGuiKey_DownArrow,
				ImGuiKey_LeftAlt,   ImGuiKey_LeftCtrl,   ImGuiKey_LeftShift, ImGuiKey_LeftSuper,
			};
			static const char * someKeysNames[] = {
				"Space",            "Backspace",         "Enter",            "Tab",
				"Left",             "Right",             "Up",               "Down",
				"AltL",             "CtrlL",             "ShiftL",           "SuperL",
			};
			static bool someKeysState[IM_ARRAYSIZE(someKeys)] = {false};
			ImGui::Columns(4);
			for(int i=0; i<IM_ARRAYSIZE(someKeys); i++){
				someKeysState[i] = ImGui::IsKeyDown(someKeys[i]);
				ImGui::Checkbox(someKeysNames[i], &someKeysState[i] );
				ImGui::NextColumn();
			}
			ImGui::Columns(1);

// You need to make some private vars public manually if you enable this
#if defined(OFX_IMGUI_DEBUG) && DEBUG_POINTERS
			ImGui::Separator();

			ImGui::Text("Internal pointers");
			ImGuiEx::ShowHelpMarker("For debugging and testing this example.");
			ImGui::Dummy(ImVec2(10,0));

            ImGui::BeginTable("Pointers", 3);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("ofxImGui::Gui");
            ImGui::TableNextColumn();
            ImGui::Text("%p", &gui);
            ImGui::TableNextColumn();
            ImGui::Text("ofxImGui::Gui*");

            ImGuiContext* currentImGuiContext = ImGui::GetCurrentContext();
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("ImGui.Context");
            ImGui::TableNextColumn();
            ImGui::Text("%p", currentImGuiContext);
            ImGui::TableNextColumn();
            ImGui::Text("ImGuiContext*");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("ofxImGui.ImGuiContextOfOFBaseWindow");
            ImGui::TableNextColumn();
            auto theoreticalContextEntry = ofxImGui::Gui::imguiContexts.find(ofGetWindowPtr());
            ImGuiContext* theoreticalWindow = (theoreticalContextEntry!=ofxImGui::Gui::imguiContexts.end()) ? theoreticalContextEntry->second : nullptr;
            ImGui::Text("%p", theoreticalWindow);
            if(!theoreticalWindow || currentImGuiContext != theoreticalWindow) ImGuiEx::ShowWarningMarker("Not the same as the current ImGui.Context ! !");
            ImGui::TableNextColumn();
            ImGui::Text("ImGuiContext*");

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("ImGui.Context.IO");
			ImGui::TableNextColumn();
			ImGui::Text("%p", &ImGui::GetIO());
			ImGui::TableNextColumn();
			ImGui::Text("ImGuiIO*");

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("ImGui.Context.IO.Bd");
			ImGui::TableNextColumn();
			ImGui::Text("%p", ((ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData));
			ImGui::TableNextColumn();
			ImGui::Text("ImGui_ImplGlfw_Data*");

			void* backendWindowPtr = ((ImGui_ImplGlfw_Data*)ImGui::GetIO().BackendPlatformUserData)->Window;
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("ImGui.Context.IO.Bd.Window");
			ImGui::TableNextColumn();
			ImGui::Text("%p", backendWindowPtr);
			ImGui::TableNextColumn();
			ImGui::Text("GLFWwindow*");

			void* ofGlFwWindowPtr = (void*)ofGetCurrentWindow()->getWindowContext();
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("of.GLFWWindow");
			ImGui::TableNextColumn();
			ImGui::Text("%p", ofGlFwWindowPtr);
			if(ofGlFwWindowPtr != backendWindowPtr) ImGuiEx::ShowWarningMarker("Not the same as the backend window !");
			ImGui::TableNextColumn();
			ImGui::Text("GLFWWindow*");

			ofAppBaseWindow* ofWindowPtr = ofGetWindowPtr();
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("of.BaseWindow");
			ImGui::TableNextColumn();
			ImGui::Text("%p", ofWindowPtr);
			if(ofxImGui::Gui::imguiContexts.find(ofWindowPtr) == ofxImGui::Gui::imguiContexts.end()) ImGuiEx::ShowWarningMarker("Not known to ofxImGui !");
			ImGui::TableNextColumn();
			ImGui::Text("ofAppBaseWindow*");

			ImGuiViewport* imGuiMainViewport = ImGui::GetMainViewport();
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("ImGui.Context.MainViewport");
			ImGui::TableNextColumn();
			ImGui::Text("%p", imGuiMainViewport);
			//if(ofxImGui::Gui::imguiContexts.find(ofWindowPtr) == ofxImGui::Gui::imguiContexts.end()) ImGuiEx::ShowWarningMarker("Not known to ofxImGui !");
			ImGui::TableNextColumn();
			ImGui::Text("ImGuiViewport*");

			ImGui::EndTable();
			ImGui::Separator();
#endif

            ImGui::End();

            ImGui::ShowMetricsWindow();
            //ImGui::ShowDemoWindow();

            static ofBitmapFont f;
            static char ofString[100];
            static char imString[100];
            sprintf(ofString, "oF   \t%ix%i\tFPS: %04.f", ofGetWindowWidth(), ofGetWindowHeight(), ofGetFrameRate());
            sprintf(imString, "ImGui\t%.fx%.f\tFPS: %04.f", ImGui::GetMainViewport()->Size.x,ImGui::GetMainViewport()->Size.y, ImGui::GetIO().Framerate);

            gui.end();

            ofDrawBitmapString(ofString, ofGetWidth() - 5 - f.getBoundingBox(ofString, 0, 0).width, 15);
            ofDrawBitmapString(imString, ofGetWidth() - 5 - f.getBoundingBox(imString, 0, 0).width, 30);

            gui.draw();
		}

		std::pair<char, unsigned int> letterToDraw = {' ', 0};
		char latestKeysPressed[11] = "         \0";
		std::pair<const char*, unsigned int> mouseButtonToDraw = {" ", 0};
		bool mouseIsPressedOf = false;
		bool mouseIsPressedImgui = false;
		void keyPressed(ofKeyEventArgs &e) override {
			letterToDraw = { e.key, 100 };
			for(unsigned int i=10; i>0; --i){
				latestKeysPressed[i] = latestKeysPressed[i-1];
			}
			latestKeysPressed[0] = e.key;
		}
		void keyReleased(ofKeyEventArgs &e) override{
			letterToDraw = { e.key, 0 };
		}
		void mouseMoved(int x, int y ) override{}
		void mouseDragged(int x, int y, int button) override{}
		void mousePressed(int x, int y, int button) override{
			mouseButtonToDraw = { button==OF_MOUSE_BUTTON_1 ? "BTN1":"BTNX", 100 };
			mouseIsPressedOf = true;
		}
		void mouseReleased(int x, int y, int button) override{
			mouseButtonToDraw = { button==OF_MOUSE_BUTTON_1 ? "BTN1":"BTNX", 0 };
			mouseIsPressedOf = false;
		}
		void mouseEntered(int x, int y) override{}
		void mouseExited(int x, int y) override{}
	private:
		ofxImGui::Gui gui;
		float v;
};
