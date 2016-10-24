#pragma once

#include "ofxiOS.h"
#include "ofxImGui.h"
#include "InputTextButton.h"

class ofApp : public ofxiOSApp {
	
public:
    void setup();
    void update();
    void draw();
    void exit();

    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);

    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    
    ofxImGui::Gui gui;
    ImVec4 backgroundColor;
    bool show_test_window;
    bool show_another_window;
    
    float floatValue;
    ofImage imageButtonSource;
    GLuint imageButtonID;
    
    ofPixels pixelsButtonSource;
    GLuint pixelsButtonID;
    
    ofTexture textureSource;
    GLuint textureSourceID;
    
    InputTextButton inputTextButton;

};


