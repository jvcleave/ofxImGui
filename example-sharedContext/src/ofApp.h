#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

// Uncomment to use a slightly different method
//#define ALTERNATIVE_IMPLEMENTATION

class ofApp : public ofBaseApp{

	public:

		void setup() {
			v2 = 0;
			v1 = 0;

            gui1.setup(nullptr, false);
            //gui1.setup(nullptr, true); // Note: it's also possible to use autodraw

            #ifdef ALTERNATIVE_IMPLEMENTATION
            gui1.forceSharedMode(true); // Force share context
            #endif

            { // scope = called plugin code that you can't change

                #ifndef ALTERNATIVE_IMPLEMENTATION
                gui2.setup();

                #else
                gui2 = &gui1;
                #endif
            }
		}

		void draw() {
			// [...] some code
            gui1.begin(); //   <-- First call = Normal behaviour
			ImGui::SliderFloat("slider1", &v1, 0.f, 10.f); //....
			ImGui::SliderFloat("slider2", &v2, 0.f, 10.f); //....
            gui1.end(); //     <-- Does nothing with sharedMode on

            ImGui::PushID("###Sandbox"); // <-- if you want to ensure a sandbox between both

            { // scope = called plugin code that you can't change

                #ifndef ALTERNATIVE_IMPLEMENTATION
                gui2.begin(); // <-- Does nothing with sharedMode on
                ImGui::SliderFloat("slider1", &v3, 0.f, 10.f); //....
                ImGui::SliderFloat("slider2", &v4, 0.f, 10.f); //....
                gui2.end(); //   <-- Does nothing with sharedMode on
                #else

                gui2->begin(); // <-- Does nothing with sharedMode on
                ImGui::SliderFloat("slider1", &v3, 0.f, 10.f); //....
                ImGui::SliderFloat("slider2", &v4, 0.f, 10.f); //....
                gui2->end(); //   <-- Does nothing with sharedMode on
                #endif
			}
            ImGui::PopID(); // <-- If you want to ensure a sandbox between both

            gui1.draw(); //    <-- Also calls gui.end()
		}


	private:
        ofxImGui::Gui gui1;
#ifdef ALTERNATIVE_IMPLEMENTATION
        ofxImGui::Gui* gui2;
#else
        ofxImGui::Gui gui2;
#endif
        float v1, v2, v3, v4;
};
