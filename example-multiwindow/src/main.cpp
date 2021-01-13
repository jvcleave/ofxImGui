#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

// Demo define
#define USE_GL3

//========================================================================
int main( ){

	ofGLFWWindowSettings settings;
    settings.setSize(300, 400);
	settings.setPosition(ofVec2f(0, 100));
#ifdef USE_GL3
    settings.setGLVersion(3, 2);
#else
    settings.setGLVersion(2, 1);
#endif
    settings.title="Window 1";
	auto window1 = ofCreateWindow(settings);

	settings.setPosition(ofVec2f(300, 100));
    settings.setSize(500, 600);
    settings.title="Window 2";
    settings.shareContextWith = window1;
    // WARNING : settings.setGLVersion(2, 1);
    // For now, both GL versions must be the same for ImGui to work in a multiwindow context.
    auto window2 = ofCreateWindow(settings);
    std::cout << "Window 1 = " << window1.get() << " // " << (ofAppGLFWWindow*) window1.get() << std::endl;
    std::cout << "Window 2 = " << window2.get() << " // " << (ofAppGLFWWindow*) window2.get() << std::endl;
    auto app1 = std::make_shared<ofApp>();
    auto app2 = std::make_shared<ofApp>();

    ofRunApp(window1, app1);
    ofRunApp(window2, app2);

	ofRunMainLoop();
}
