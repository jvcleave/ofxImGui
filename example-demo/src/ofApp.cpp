#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //required call
    gui.setup();
    //gui.setTheme(new ThemeTest());
    
    ImGui::GetIO().MouseDrawCursor = false;
    clear_color = ImColor(114, 144, 154);
    show_test_window = true;
    show_another_window = false;
    floatValue = 0.0f;
    
    tex_button = gui.loadImage("of.png");
    
}

bool doSetTheme = false;
//--------------------------------------------------------------
void ofApp::update(){
    
    if(doSetTheme)
    {
        doSetTheme = false;
        gui.setTheme(new ThemeTest());
        
    }
    
    
}
bool doThemeColorsWindow = false;
//--------------------------------------------------------------
void ofApp::draw(){
    
    //required to call this at beginning
    gui.begin();
    
    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    {
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &floatValue, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        if (ImGui::Button("Test Window")) show_test_window ^= 1;
        if (ImGui::Button("Another Window")) show_another_window ^= 1;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    } 
    // 2. Show another simple window, this time using an explicit Begin/End pair
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
    
    
    bool pressed = ImGui::ImageButton((ImTextureID)(uintptr_t)tex_button, ImVec2(200, 141));
    
    if(doThemeColorsWindow)
    {
        gui.openThemeColorWindow();
        
    }
    
    //required to call this at end
    gui.end();
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    ofLogVerbose(__FUNCTION__) << key;
    switch (key)
    {
        case 't' :
        {
            doThemeColorsWindow = !doThemeColorsWindow;
            break;
        }
        case 'c' :
        {
            doSetTheme = !doSetTheme;
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