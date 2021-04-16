#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "myAddonClass.h"
#include "privateAddonClass.h"

class ofApp : public ofBaseApp{

	public:
        void setup() override;
        void draw() override;

	private:

        // instance 1 (ofApp)
        ofxImGui::Gui ofAppGui;
        ImVec4 ofAppColor = ofColor(114, 144, 154);
        void drawImGui();

        // instance 2 (class/addon build for shared contexts)
        myAddonClass myAddonObject;

        // Instance 3 (class/addon that you can not modify)
        privateAddonClass privateAddonObject;
};
