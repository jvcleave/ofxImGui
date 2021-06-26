#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    int windowWidth = 800;
    int windowHeight = 600;

#if defined ( TARGET_OPENGLES )
    ofGLESWindowSettings settings;
    settings.setGLESVersion(2);
#else
    ofGLWindowSettings settings;
    settings.setGLVersion(3,2);
#endif

	settings.title="ofxImGui Example Debug";
	settings.setSize(windowWidth, windowHeight);

	auto window1 = ofCreateWindow(settings);
	auto app1 = std::make_shared<ofApp>();

	ofRunApp(window1, app1);
	ofRunMainLoop();
}
