#include "ofMain.h"
#include "ofApp.h"

#ifndef __EMSCRIPTEN__
#include "ofAppGLFWWindow.h"
#endif

int main() {
#ifndef __EMSCRIPTEN__
    // Desktop — OpenGL 3.2 core profile
    ofGLWindowSettings settings;
    settings.setGLVersion(3, 2);
    settings.setSize(1280, 800);
    settings.title = "ofxImGui \xe2\x80\x94 example-Conway";
    auto window = ofCreateWindow(settings);
    ofRunApp(window, std::make_shared<ofApp>());
    ofRunMainLoop();
#else
    // Emscripten — WebGL2 / GLES3 (ofxImGui auto-selects the correct backend)
    ofSetupOpenGL(1280, 800, OF_WINDOW);
    ofRunApp(new ofApp());
#endif
}
