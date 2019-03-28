#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.setSize(300, 300);
	settings.setPosition(ofVec2f(0, 100));
	auto window1 = ofCreateWindow(settings);

	settings.setPosition(ofVec2f(300, 100));
	auto window2 = ofCreateWindow(settings);

	auto app1 = std::make_shared<ofApp>();
	auto app2 = std::make_shared<ofApp>();

	ofRunApp(window1, app1);
	ofRunApp(window2, app2);

	ofRunMainLoop();
}
