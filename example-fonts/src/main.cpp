#include "ofMain.h"
#include "ofApp.h"

int main()
{

#if defined(TARGET_OPENGLES) || defined(TARGET_EMSCRIPTEN) || defined(__EMSCRIPTEN__)
    ofGLESWindowSettings settings;
    settings.setGLESVersion(2);
#else
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2);
#endif
    settings.setPosition(ofVec2f(0, 0));
    settings.setSize(600, 600);
    settings.title="ofxImGui font usage example";
    ofCreateWindow(settings);

    ofRunApp( new ofApp());

}
