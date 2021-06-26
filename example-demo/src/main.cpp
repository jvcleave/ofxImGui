#include "ofMain.h"
#include "ofApp.h"

int main(){

#if defined(TARGET_OPENGLES)
    ofGLESWindowSettings settings;
    #if defined(TARGET_RASPBERRY_PI)
    settings.setGLESVersion(2);
    #else
    settings.setGLESVersion(3);
    #endif

#else
    ofGLESWindowSettings settings;
    settings.setGLVersion(3,2);
#endif
    settings.setSize(1280, 720);
    ofCreateWindow(settings);

    ofRunApp( new ofApp());

}
