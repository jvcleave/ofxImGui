#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofLogVerbose() << ofGetWidth() << " x " << ofGetHeight();
    //required call
    gui.setup();
    
    ImGui::GetIO().MouseDrawCursor = false;
    //backgroundColor is stored as an ImVec4 type but can handle ofColor
    backgroundColor = ofColor(114, 144, 154);
    show_test_window = true;
    show_another_window = false;
    floatValue = 0.0f;
    //load your own ofImage
    imageButtonSource.load("of.png");
    imageButtonID = gui.loadImage(imageButtonSource);
    
    //or have the loading done for you if you don't need the ofImage reference
    //imageButtonID = gui.loadImage("of.png");
    
    //can also use ofPixels in same manner
    ofLoadImage(pixelsButtonSource, "of_upside_down.png");
    pixelsButtonID = gui.loadPixels(pixelsButtonSource);
    
    //and alt method
    //pixelsButtonID = gui.loadPixels("of_upside_down.png");
    
    //pass in your own texture reference if you want to keep it
    textureSourceID = gui.loadTexture(textureSource, "of_upside_down.png");
    
    //or just pass a path
    //textureSourceID = gui.loadTexture("of_upside_down.png");
    
    ofLogVerbose() << "textureSourceID: " << textureSourceID;
    
    inputTextButton.setup("Editable Text Button");
}

//--------------------------------------------------------------
void ofApp::update()
{
    
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    //backgroundColor is stored as an ImVec4 type but is converted to ofColor automatically
    
    ofSetBackgroundColor(backgroundColor);
    
    //required to call this at beginning
    gui.begin();
    
    //In between gui.begin() and gui.end() you can use ImGui as you would anywhere else
    
    // 1. Show a simple window
    {
        inputTextButton.draw();
        
        ImGui::Text(inputTextButton.text.c_str());
        ImGui::SliderFloat("Float", &floatValue, 0.0f, 1.0f);
        
        //this will change the app background color
        ImGui::ColorEdit3("Background Color", (float*)&backgroundColor);
        if(ImGui::Button("Test Window"))
        {
            show_test_window = !show_test_window;
        }
        
        if (ImGui::Button("Another Window"))
        {
            //bitwise OR
            show_another_window ^= 1;
            
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    // 2. Show another window, this time using an explicit ImGui::Begin and ImGui::End
    if (show_another_window)
    {
        //note: ofVec2f and ImVec2f are interchangeable
        ImGui::SetNextWindowSize(ofVec2f(200,100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello");
        ImGui::End();
    }
    
    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (show_test_window)
    {
        ImGui::SetNextWindowPos(ofVec2f(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&show_test_window);
    }
    
    bool pressed = ImGui::ImageButton((ImTextureID)(uintptr_t)imageButtonID, ImVec2(200, 200));
    pressed = ImGui::ImageButton((ImTextureID)(uintptr_t)pixelsButtonID, ImVec2(200, 200));
    pressed = ImGui::ImageButton((ImTextureID)(uintptr_t)textureSourceID, ImVec2(200, 200));
    
    

    
    //required to call this at end
    gui.end();
    
    if(textureSource.isAllocated())
    {
        //textureSource.draw(ofRandom(200), ofRandom(200));
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}
