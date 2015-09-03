#include "ofMain.h"
#include "ofApp.h"
#if (OF_VERSION_MINOR != 9) && defined(TARGET_OPENGLES)
#include "ofGLProgrammableRenderer.h"
#endif

#define FORCE_PROGRAMMMABLE 1

#ifdef FORCE_PROGRAMMMABLE
#include "ofGLProgrammableRenderer.h"
#endif
int main()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
#if defined(TARGET_OPENGLES)
    #if (OF_VERSION_MINOR == 9)
        ofGLESWindowSettings settings;
        settings.width = 1280;
        settings.height = 720;
        settings.setGLESVersion(2);
        ofCreateWindow(settings);
    #else
        ofSetLogLevel("ofThread", OF_LOG_ERROR);
        ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
        ofSetupOpenGL(1280, 720, OF_WINDOW);
    #endif
#else
#ifdef FORCE_PROGRAMMMABLE
    ofGLWindowSettings glWindowSettings;
    glWindowSettings.width = 1280;
    glWindowSettings.height = 720;
    glWindowSettings.setGLVersion(3, 2);
    ofCreateWindow(glWindowSettings);
#else
    ofSetLogLevel("ofThread", OF_LOG_ERROR);
    ofSetupOpenGL(1280, 720, OF_WINDOW);
#endif
    
#endif
    ofRunApp( new ofApp());
}
