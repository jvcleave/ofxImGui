#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:
		ofApp() : v(0) {}

		void setup() {
            gui.setup();
		}
		void draw() {
            // Warning message
            ofDrawBitmapStringHighlight("Known bugs :", 0, ofGetHeight()-45);
            ofDrawBitmapStringHighlight("Both windows have to be the same height, otherwise there's a mouse offset !", 0, ofGetHeight()-25);
            ofDrawBitmapStringHighlight("Also, clipping occurs when their sizes differ.", 0, ofGetHeight()-10);

            // Draw to ImGui (after OF related stuff, to make it overlay)
			gui.begin();

            ImGui::Begin("MultiWindow");
			ImGui::SliderFloat("slider", &v, 0.f, 10.f);
            ImGui::End();

            ImGui::ShowMetricsWindow();
            //ImGui::ShowDemoWindow();

			gui.end();
            gui.draw();
		}

	private:
		ofxImGui::Gui gui;
		float v;
};
