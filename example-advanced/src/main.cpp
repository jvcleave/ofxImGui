#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.title="ofxImGui Example Advanced";

	auto window1 = ofCreateWindow(settings);
    auto app1 = std::make_shared<ofApp>();

    ofRunApp(window1, app1);
	ofRunMainLoop();
}
