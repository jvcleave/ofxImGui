#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:
		ofApp() : v(0) {}

        void setup() {
            gui.setup(nullptr, false, ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable);
		}
		void draw() {
            // Warning message
            #define FONT_HEIGHT 18
            #define FONT_MARGIN 8
            int height = ofGetHeight()-FONT_MARGIN-(FONT_HEIGHT*4);
            ofDrawBitmapStringHighlight("Known bugs :", FONT_MARGIN, height); height+=FONT_HEIGHT;
            //ofDrawBitmapStringHighlight("Both windows have to be the same height, otherwise there's a mouse offset !", 0, height+=15);
            //ofDrawBitmapStringHighlight("Clipping occurs when their sizes differ.", 0, height+=15);
            ofDrawBitmapStringHighlight(" - Depending on the focused window, framerates differ.", FONT_MARGIN, height); height+=FONT_HEIGHT;
            ofDrawBitmapStringHighlight(" - Viewports and docking features work, but only within the same oF window.\n   (they are isolated, better enable viewports and pop standalone windows over your 2nd of window)", FONT_MARGIN, height); height+=FONT_HEIGHT*2;

            // Draw to ImGui (after OF related stuff, to make it overlay)
			gui.begin();

            ImGui::Begin("MultiWindow");
			ImGui::SliderFloat("slider", &v, 0.f, 10.f);
            ImGui::End();

            ImGui::ShowMetricsWindow();
            //ImGui::ShowDemoWindow();

            static ofBitmapFont f;
            static char ofString[100];
            static char imString[100];
            sprintf(ofString, "oF   \t%ix%i\tFPS: %04.f", ofGetWindowWidth(), ofGetWindowHeight(), ofGetFrameRate());
            sprintf(imString, "ImGui\t%.fx%.f\tFPS: %04.f", ImGui::GetMainViewport()->Size.x,ImGui::GetMainViewport()->Size.y, ImGui::GetIO().Framerate);

            gui.end();

            ofDrawBitmapString(ofString, ofGetWidth() - 5 - f.getBoundingBox(ofString, 0, 0).width, 15);
            ofDrawBitmapString(imString, ofGetWidth() - 5 - f.getBoundingBox(imString, 0, 0).width, 30);

            gui.draw();
		}

	private:
		ofxImGui::Gui gui;
		float v;
};
