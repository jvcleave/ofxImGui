#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //required call
    gui.setup(nullptr, true, ImGuiConfigFlags_ViewportsEnable );

    //backgroundColor is stored as an ImVec4 type but can handle ofColor
    backgroundColor = ofColor(114, 144, 154);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    //required to call this at beginning
    gui.begin();
    
    // Show the ImGui test window. Most of the sample code is in ImGui::ShowDemoWindow()
    ImGui::SetNextWindowPos( ofVec2f( ofGetWindowPositionX(), ofGetWindowPositionY()), ImGuiCond_Once);
    ImGui::SetNextWindowSize( ofVec2f(ofGetWidth(), ofGetHeight()), ImGuiCond_Once);
    ImGui::ShowDemoWindow();

    //required to call this at end
    gui.end();
    
}


