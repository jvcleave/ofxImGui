#pragma once

#include "ofMain.h"

#include "ofxImGui.h"

class privateAddonClass : public ofBaseApp
{
public:
    virtual void setup() override;
    virtual void draw() override;

	void drawImGui();
private:
    ofxImGui::Gui gui;

    float sizef = 1;
    float speedf = 0.1f;
    ImVec4 color = {1,1,1,1};
    bool showGuiWindow = true;
    bool enableDrawing = true;
};
