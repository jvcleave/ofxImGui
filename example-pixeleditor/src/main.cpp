#include "ofMain.h"
#include "ofApp.h"

#ifndef __EMSCRIPTEN__
#include "ofAppGLFWWindow.h"
#endif

int main() {
#ifndef __EMSCRIPTEN__
    ofGLWindowSettings settings;
    settings.setGLVersion(3, 2);
    settings.setSize(820, 660);
    settings.title = "ofxImGui \xe2\x80\x94 example-pixeleditor";
    auto window = ofCreateWindow(settings);
    ofRunApp(window, std::make_shared<ofApp>());
    ofRunMainLoop();
#else
    ofSetupOpenGL(820, 660, OF_WINDOW);
    ofRunApp(new ofApp());
#endif
}
