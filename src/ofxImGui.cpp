#include "ofxImGui.h"

ofxImGui::ofxImGui()
{
    last_time = 0.0f;
    engine = NULL;
    io = NULL;
}

void ofxImGui::setup()
{
    io = &ImGui::GetIO();

    io->DisplaySize = ImVec2((float)ofGetWidth(), (float)ofGetHeight());
    io->MouseDrawCursor = false;

#if defined(TARGET_OPENGLES)
    engine = new EngineOpenGLES();
#else  
    engine = new EngineGLFW();
#endif

    engine->setup(io);
    cinderStyle.setup();

}

GLuint ofxImGui::loadImage(string imagePath)
{
    if(!engine) return -1;

    ofImage image;
    image.load(imagePath);
    return engine->loadTextureImage2D(image);
}

void ofxImGui::begin()
{
    if(!io || !engine)
    {
        ofLogError() << "setup call required - calling it for you";
        setup();
    }
    float current_time = ofGetElapsedTimef();
    io->DeltaTime = last_time > 0.f ? current_time - last_time : 1.f / 60.f;
    last_time = current_time;

    io->MousePos = ImVec2((float)ofGetMouseX(), (float)ofGetMouseY());
    ImGui::NewFrame();
}

void ofxImGui::end()
{
    ImGui::Render();
}


void ofxImGui::close()
{
    if(engine)
    {
        delete engine;
        engine = NULL;
    }
    if(io)
    {
        io->Fonts->TexID = 0;
        io = nullptr;
    }
    ImGui::Shutdown();
}

ofxImGui::~ofxImGui()
{
    close();
}
