#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // First call to setup() for your main application menu (master).
    // Any next call to setup() will setup ofxImGui as a slave.
    ofAppGui.setup(nullptr, false, ImGuiConfigFlags_DockingEnable, true);
    //ofAppGui.setup(nullptr, true); // Note: it's also possible to use autodraw

    // There's 2 ways of achieving multi context.
    // By default, it's automatically enabled when a 2nd ofxImGui instance is created in the same ofApp.
    // You can also force the shared mode, needed when the shared code cannot be modified

    // This all is needed for privateAddonClass, which "say" we cannot modify, and hasn't specially been build for use in shared contexts
    ofAppGui.setSharedMode(true); // Force shared context

    // Setup addon guis in shared mode
    myAddonObject.setup( ofAppGui ); // Preferred method : share the menu handle
    privateAddonObject.setup();
}

//--------------------------------------------------------------
void ofApp::drawImGui() 
{
    ofAppGui.begin(); // <-- First call = Normal behaviour in sharedMode
	{
        //ImGui::DockSpaceOverViewport();

        // Position the Gui only the firt time, after which it's reset to the last used layout.
        ImGui::SetNextWindowPos(ImVec2(0,20), ImGuiCond_FirstUseEver );
        ImGui::SetNextWindowSize(ImVec2(260,ofGetHeight()-20), ImGuiCond_FirstUseEver );
        ImGui::Begin("ofApp gui");
        ImGui::Separator();
        ImGui::Text("Background color :");
        ImGui::ColorPicker4("##picker", (float*)&ofAppColor);
		ImGui::End();

        ImGui::BeginMainMenuBar();
        if(ImGui::BeginMenu("Main")){
            // Add default menu entries
            ImGui::MenuItem( "Open" );
            ImGui::MenuItem( "Save" );
            ImGui::MenuItem( "Quit" );
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
	}
    ofAppGui.end(); // <-- Does nothing with sharedMode on
}

//--------------------------------------------------------------
void ofApp::draw() 
{
    // oF drawing
    ofPushStyle();
    ofClear(ofAppColor);
    privateAddonObject.draw();
    myAddonObject.draw();
    ofPopStyle();

    // GUI drawing

	// instance 1
    this->drawImGui();

    // instance 2 - Plugin code that was made for sharing context
    ImGui::PushID("##SomeUniqueSandboxHash"); // <-- if you want to ensure a sandbox between both (fails with sharedMode off)
	myAddonObject.drawImGui();
    ImGui::PopID(); // <-- If you want to ensure a sandbox between both

    // Instance 3 - Plugin code that was not mode for sharing context
    privateAddonObject.drawImGui();

    // Manually render once all GUI items have been submitted
    ofAppGui.draw(); //    <-- Also calls ofAppGui.end()
}
