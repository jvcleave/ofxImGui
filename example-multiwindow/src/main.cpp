#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

#pragma message "Warning, multiwindow is in testing phase. Proceed with caution !"

//========================================================================
int main( ){
    ofSetLogLevel( OF_LOG_VERBOSE );

    ofGLFWWindowSettings settings; // <-- GLES or GL depending on environment
#if defined( TARGET_OPENGLES )
    #if defined(TARGET_RASPBERRY_PI)
    settings.setGLESVersion(2);
    #else
    settings.setGLESVersion(3);
    #endif
#else
    #if defined(TARGET_OSX)
    settings.setGLVersion(3,2);
    #else
    settings.setGLVersion(4,1);
    #endif
#endif

    settings.setSize(300, 400);
	settings.setPosition(ofVec2f(0, 100));

    settings.title="Window 1";
	auto window1 = ofCreateWindow(settings);

	settings.setPosition(ofVec2f(300, 100));
    settings.setSize(500, 600);
    settings.title="Window 2";
#if !defined( TARGET_OPENGLES )
    settings.shareContextWith = window1;
#else
#pragma message "Warning, cannot share context with GLES windows. This example might not work correctly."
#endif

    // WARNING : settings.setGLVersion(2, 1);
    // For now, both GL versions must be the same for ImGui to work in a multiwindow context.
    auto window2 = ofCreateWindow(settings);
    std::cout << "Window 1 = " << window1.get() << " // " << (ofAppGLFWWindow*) window1.get() << " // " << ((ofAppGLFWWindow*) window1.get())->getGLFWWindow() << std::endl;
    std::cout << "Window 2 = " << window2.get() << " // " << (ofAppGLFWWindow*) window2.get() << " // " << ((ofAppGLFWWindow*) window2.get())->getGLFWWindow() << std::endl;
    auto app1 = std::make_shared<ofApp>();
    auto app2 = std::make_shared<ofApp>();

    ofRunApp(window1, app1);
    ofRunApp(window2, app2);

	ofRunMainLoop();
}
