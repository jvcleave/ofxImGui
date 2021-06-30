#include "ofMain.h"
#include "ofApp.h"

int main(){
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
    settings.setSize(1280, 720);
    ofCreateWindow(settings);

    ofRunApp( new ofApp());

}
