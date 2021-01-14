#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    //settings.setSize(300, 300);
    //settings.setPosition(ofVec2f(0, 100));
    //settings.setGLVersion(3,2);
    settings.title="ofxImGui Shared Context Demo";
    auto window1 = ofCreateWindow(settings);

    auto app1 = std::make_shared<ofApp>();

    ofRunApp(window1, app1);

	ofRunMainLoop();
}
