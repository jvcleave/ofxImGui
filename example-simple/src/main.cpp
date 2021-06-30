#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    int windowWidth = 800;
    int windowHeight = 600;

#if defined( TARGET_OPENGLES ) || defined ( FORCE_GLES )
    ofGLESWindowSettings settings;
    #if defined(TARGET_RASPBERRY_PI)
    settings.setGLESVersion(2);
    #else
    settings.setGLESVersion(3);
    #endif

#else
    ofGLWindowSettings settings;
    #if defined(TARGET_OSX)
    settings.setGLVersion(3,2);
    #else
    settings.setGLVersion(4,1);
    #endif
#endif
	settings.title="ofxImGui Example Debug";
	settings.setSize(windowWidth, windowHeight);

	auto window1 = ofCreateWindow(settings);
	auto app1 = std::make_shared<ofApp>();

	ofRunApp(window1, app1);
	ofRunMainLoop();
}
