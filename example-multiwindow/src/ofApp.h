#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:

		void setup() {
			v2 = 0;
			v1 = 0;

			gui.setup(false, true); // No autodraw, Allow chaining
		}

		void draw() {
			// [...] some code
			gui.begin(); //   <-- First call = Normal behaviour
			ImGui::SliderFloat("slider1", &v1, 0.f, 10.f); //....
			ImGui::SliderFloat("slider2", &v2, 0.f, 10.f); //....
			gui.end(); //     <-- Does nothing with chaining on
			// [...] more code
			{ // scope = called plugin code that you can't change
				gui.begin(); // <-- Does nothing with chaining on
				ImGui::SliderFloat("slider1", &v1, 0.f, 10.f); //....
				ImGui::SliderFloat("slider2", &v2, 0.f, 10.f); //....
				gui.end(); //   <-- Does nothing with chaining on
			}
			gui.draw(); //    <-- Also calls gui.end()
		}

	private:
		ofxImGui::Gui gui;
		float v1;
		float v2;
};
