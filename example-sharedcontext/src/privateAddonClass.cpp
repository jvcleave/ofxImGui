#include "privateAddonClass.h"

//--------------------------------------------------------------
void privateAddonClass::setup()
{
	gui.setup();
}

//--------------------------------------------------------------
void privateAddonClass::drawImGui()
{

	gui.begin();

    // 2nd window
    if(showGuiWindow){
        if(ImGui::Begin("privateAddon Gui Window", &showGuiWindow)){
            ImGui::Text("Circle settings");
            ImGui::Checkbox( "Draw Circle", &enableDrawing );
            ImGui::ColorEdit4("Color", (float*)&color);
            ImGui::SliderFloat("Speed", &speedf, 0.f, 2.f);
            ImGui::SliderFloat("Circle Size", &sizef, 0.f, 2.f);
        }
        ImGui::End();
    }

    // Extend application menu
    ImGui::BeginMainMenuBar();

    if( ImGui::BeginMenu("Circle") ){
        ImGui::Checkbox( "Show Controls", &showGuiWindow );
        ImGui::EndMenu();
    }

    // You can even append your menu items to existing menus
    if( ImGui::BeginMenu("Main") ){
        ImGui::Checkbox( "Draw Circle", &enableDrawing );
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

	gui.end();
}

//--------------------------------------------------------------
void privateAddonClass::draw()
{
    if( enableDrawing ){
        ofSetColor(color.x*255, color.y*255, color.z*255, color.w*255);
        ofDrawCircle( ofGetWidth()*.5f, ofGetHeight()*.5f, ofGetHeight()*.5f*sizef*((fmod(ofGetElapsedTimef()*speedf, 1.f)-0.5f)*2.f) );
    }
}
