#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    
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
    
    ofDirectory dataDirectory(ofToDataPath("", true));
    
    files = dataDirectory.getFiles();
    for(size_t i=0; i<files.size(); i++)
    {
        fileNames.push_back(files[i].getFileName());
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    
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
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("Float", &floatValue, 0.0f, 1.0f);
        
        //this will change the app background color
        ImGui::ColorEdit3("Background Color", (float*)&backgroundColor);
        if(ImGui::Button("Demo Window"))
        {
            show_test_window = !show_test_window;
        }
        
        if (ImGui::Button("Another Window"))
        {
            //bitwise OR
            show_another_window ^= 1;
            
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        
        if (ImGui::Button("CUSTOM THEME"))
        {
            gui.setTheme(new MyTheme());
            
        }ImGui::SameLine();
        
        if (ImGui::Button("DEFAULT THEME"))
        {
            gui.setTheme(new ofxImGui::DefaultTheme());
            
        }
        
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
    
    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowDemoWindow()
    if (show_test_window)
    {
        ImGui::SetNextWindowPos(ofVec2f(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowDemoWindow(&show_test_window);
    }
    
    if(!fileNames.empty())
    {
        
        //ofxImGui::VectorListBox allows for the use of a vector<string> as a data source
        static int currentListBoxIndex = 0;
        if(ofxImGui::VectorListBox("VectorListBox", &currentListBoxIndex, fileNames))
        {
            ofLog() << " VectorListBox FILE PATH: "  << files[currentListBoxIndex].getAbsolutePath();
        }
        
        //ofxImGui::VectorCombo allows for the use of a vector<string> as a data source
        static int currentFileIndex = 0;
        if(ofxImGui::VectorCombo("VectorCombo", &currentFileIndex, fileNames))
        {
            ofLog() << "VectorCombo FILE PATH: "  << files[currentFileIndex].getAbsolutePath();
        }
    }

    
    
    //GetImTextureID is a static function define in Helpers.h that accepts ofTexture, ofImage, or GLuint
    if(ImGui::ImageButton(GetImTextureID(imageButtonID), ImVec2(200, 200)))
    {
           ofLog() << "PRESSED";
    }
    
    //or do it manually
    ImGui::Image((ImTextureID)(uintptr_t)textureSourceID, ImVec2(200, 200));

    ImGui::Image(GetImTextureID(pixelsButtonID), ImVec2(200, 200));

   
    
    

    
    //required to call this at end
    gui.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    ofLogVerbose(__FUNCTION__) << key;
    switch (key)
    {

        case 's':
        {
            break;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    ofLogVerbose(__FUNCTION__) << key;
    
}


void ofApp::mouseScrolled(float x, float y)
{
    ofLogVerbose(__FUNCTION__) << "x: " << x << " y: " << y;
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
