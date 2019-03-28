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
			gui.begin();
			ImGui::SliderFloat("slider", &v, 0.f, 10.f);
			gui.end();
		}

	private:
		ofxImGui::Gui gui;
		float v;
};
