#include "myAddonClass.h"

//--------------------------------------------------------------
void myAddonClass::setup()
{
    // Ensure the gui is setup.
    if(gui==nullptr){
        gui = new ofxImGui::Gui();
        gui->setup();
    }

    // Rest of setup code
    // ...
}

void myAddonClass::setup(ofxImGui::Gui& _parentGui)
{
    // Use parent's shared gui
    if( _parentGui.isInSharedMode() ){
        gui = &_parentGui;
    }
    else {
        ofLogError("myAddonClass::setup") << "The provided Gui is not running in shared mode, or has not been setup yet. Setting up a new Gui instead.";
    }

    // Continue with regular setup
    this->setup();
}

//--------------------------------------------------------------
void myAddonClass::drawImGui()
{
	auto mainSettings = ofxImGui::Settings();

    gui->begin();
	{
        if( showGuiWindow ){
            // 1st window
            if( ImGui::Begin("myAddonClass 1st Window", &showGuiWindow, ImGuiWindowFlags_None) ){
                ImGui::Text("Rectangle settings");
                ImGui::Checkbox( "Draw Rectangles", &enableDrawing );
                ImGui::ColorEdit4("Color", (float*)&color);
                ImGui::SliderFloat("Speed", &speedf, 0.f, 2.f);
                ImGui::SliderFloat("Size", &sizef, 0.f, 2.f);
            }
            ImGui::End();

            // 2nd window
            // Using ofParameters
            if (ofxImGui::BeginWindow("myAddonClass 2nd Window", mainSettings, ImGuiWindowFlags_None, &showGuiWindow)){
                ImGui::Text("These controls are bound to ofParameters");
                ofxImGui::AddGroup(colors, mainSettings);
                ofxImGui::AddSlider(rotationOffset);
                ofxImGui::AddSlider(minSize);
            }
            ofxImGui::EndWindow(mainSettings);
        }
	}

    // Extend application menu
    ImGui::BeginMainMenuBar();
    if(ImGui::BeginMenu("myAddonMenu")){
        //ImGui::MenuItem( "Show Gui" );
        ImGui::Checkbox("Show Controls", &showGuiWindow);
        ImGui::EndMenu();
    }

    // You can even append your menu items to existing menus
    if(ImGui::BeginMenu("Main")){
        ImGui::Checkbox( "Draw Rectangles", &enableDrawing );
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    gui->end();
}

//--------------------------------------------------------------
void myAddonClass::draw()
{
    if( enableDrawing ){
        float staticAnimationPos = ((cos(ofGetElapsedTimef()*TWO_PI*speedf)));
        int rectSize = minSize + abs( (((ofGetHeight()-minSize)*sizef))*(staticAnimationPos));
        ofPushMatrix();
        ofTranslate(ofGetWidth()*.5f, ofGetHeight()*.5f);
        ofRotateDeg((-rotationOffset*(staticAnimationPos)));
        ofSetColor(background->r*255, background->g*255, background->b*255, background->a*255);
        ofDrawRectangle( -rectSize*.5f, -rectSize*.5f, rectSize, rectSize );
        ofRotateDeg(rotationOffset*(staticAnimationPos));
        ofSetColor(color.x*255, color.y*255, color.z*255, color.w*255);
        ofDrawRectangle( -rectSize*.5f, -rectSize*.5f, rectSize, rectSize );
        ofRotateDeg(rotationOffset*(staticAnimationPos));
        ofSetColor(foreground->r*255, foreground->g*255, foreground->b*255, foreground->a*255);
        ofDrawRectangle( -rectSize*.5f, -rectSize*.5f, rectSize, rectSize );
        ofPopMatrix();
    }
}
