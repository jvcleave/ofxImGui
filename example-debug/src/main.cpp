#include "ofMain.h"
#include "ofApp.h"
//#include "ofAppGLFWWindow.h"

// Uncomment to force GLES for testing
//#define FORCE_GLES

// Uncomment to force FIXED pipeline for testing
#define FORCE_FIXED_GL

//========================================================================
int main( ){
	ofSetLogLevel(OF_LOG_VERBOSE);

    int windowWidth = 900;
    int windowHeight = 900;
	
#if defined( TARGET_OPENGLES ) || defined ( FORCE_GLES )
    ofGLESWindowSettings settings;
    #ifdef FORCE_FIXED_GL
        settings.setGLESVersion(1);
    #else
        #if defined(TARGET_RASPBERRY_PI)
        settings.setGLESVersion(2);
        #else
        settings.setGLESVersion(3);
        #endif
    #endif
#else
    ofGLWindowSettings settings;
    #ifdef FORCE_FIXED_GL
        settings.setGLVersion(2,1);
    #else
        #if defined(TARGET_OSX)
        settings.setGLVersion(3,2);
        #else
        settings.setGLVersion(4,1);
        #endif
    #endif
#endif

	settings.title="ofxImGui Example Debug";
	settings.setSize(windowWidth, windowHeight);

	auto window1 = ofCreateWindow(settings);
	auto app1 = std::make_shared<ofApp>();

    ofRunApp(window1, app1);
	ofRunMainLoop();
}
