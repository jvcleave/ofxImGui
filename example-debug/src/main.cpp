#include "ofMain.h"
#include "ofApp.h"
//#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	ofSetLogLevel(OF_LOG_VERBOSE);

    int windowWidth = 900;
    int windowHeight = 900;
	
#ifndef TARGET_OPENGLES
    ofGLESWindowSettings settings;
    settings.setSize(windowWidth, windowHeight);
    settings.setGLESVersion(2);
#else
    //ofGLFWWindowSettings settings;
    ofGLWindowSettings settings;
    settings.setGLVersion(3,2);
	//ofSetupOpenGL(windowWidth, windowHeight, OF_WINDOW);

    //ofGLESWindowSettings settings;
    //settings.setGLESVersion(2);
#endif


	settings.title="ofxImGui Example Debug";
	settings.setSize(windowWidth, windowHeight);

	auto window1 = ofCreateWindow(settings);
	auto app1 = std::make_shared<ofApp>();

    ofRunApp(window1, app1);
	ofRunMainLoop();

	//ofSetupOpenGL(800,600,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	//ofRunApp(new ofApp());

    //ofRunApp(window1, app1);
	//ofRunMainLoop();
}
