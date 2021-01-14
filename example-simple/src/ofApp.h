#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp{

	public:
        ofApp() {}

		void setup() {
            gui.setup(nullptr, false);
		}

		void draw() {
            ofSetBackgroundColor(backGroundColor[0]*255, backGroundColor[1]*255, backGroundColor[2]*255);

            // Start drawing to ImGui (newFrame)
			gui.begin();

            // Create a new window
            ImGui::Begin("ofxImGui example-simple");

            // Method 1 - Passing variables to ImGui
            // In ImGui code, you'll often find static (stack) variables, as below.
            // They are created once in a stack and remain accessible there, unlike non-static variables which get destructed once the stack closes.
            // Many arguments you pass to ImGui functions need to remain valid between frames. (they are passed by reference [&])
            static bool staticBool = false;
            ImGui::Checkbox("Checkbox", &staticBool);

            // You can hide and show Gui parts on demand
            if(staticBool){
                ImGui::Text("The checkbox above is checked.");
            }

            ImGui::Dummy(ImVec2(10,10));
            ImGui::Separator();

            // Method 2 - Passing variables to ImGui
            // This method is closer to OF practises, defining it as a member variable of your ofApp, and passing it to ImGui as a reference.
            ImGui::Checkbox("Draw lines", &drawLines);
            ImGui::ColorEdit3("Background color", &backGroundColor[0]);
            ImGui::SliderFloat("Float Slider", &v1, -10.f, 10.f);

            // Close the main window
            ImGui::End();

            // End our ImGui Frame. Also Renders in autoDraw mode.
            gui.end();

            // The GUI hasn't been rendered yet : we can still draw below it
            if(drawLines){
                auto halfWidth = ofGetWidth()*.5f;
                auto halfHeight = ofGetHeight()*.5f;
                ofDrawLine( halfWidth+ofRandomf()*halfWidth, halfHeight+ofRandomf()*halfHeight, halfWidth+ofRandomf()*halfWidth, halfHeight+ofRandomf()*halfHeight );
            }

            gui.draw(); // <-- In manual mode, you can choose to render imgui at a given moment in your pipeline

            ofDrawBitmapStringHighlight( ofToString(v1), 10, 20); // <-- This text will be drawn over the layout
		}

        void update(){
            // Gui variables are also accessible outside of the draw() loop.
            int v1copy = v1;
        }

    private:
        ofxImGui::Gui gui;

        // Variables exposed to ImGui
        float v1 = 0;
        float backGroundColor[3] = {1,1,1};
        bool drawLines = false;
};
