#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "MyTheme.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void mouseScrolled(float x, float y);
    
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
    
    std::vector<std::string> fileNames;
    std::vector<ofFile> files;
};
