#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

// Comment below to use manual drawing
#define USE_AUTODRAW

class ofApp : public ofBaseApp{

	public:
        ofApp() {}

		void setup() {
            #ifdef USE_AUTODRAW
            gui.setup(nullptr, true, ImGuiConfigFlags_None, true );
            #else
            gui.setup(nullptr, false);
            #endif
		}

		void draw() {
            ofSetBackgroundColor(backGroundColor[0]*100, backGroundColor[1]*100, backGroundColor[2]*100);

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
            ImGui::Text("GLFW version : %i%i%i0", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION );

            // Close the main window
            ImGui::End();

            // The GUI hasn't been rendered yet : we can still draw below it
            if(drawLines){
                auto halfWidth = ofGetWidth()*.5f;
                auto halfHeight = ofGetHeight()*.5f;
                ofDrawLine( halfWidth+ofRandomf()*halfWidth, halfHeight+ofRandomf()*halfHeight, halfWidth+ofRandomf()*halfWidth, halfHeight+ofRandomf()*halfHeight );
            }
            ofDrawBitmapStringHighlight( "Float value used in oF: " +ofToString(v1), 10, 20);

            // End our ImGui Frame.
            gui.end();

            #ifdef USE_AUTODRAW
            ofDrawBitmapStringHighlight( "I'm below the Gui !", 10, 40); // <-- This text will be over below the gui, except in shared mode
            #else
            gui.draw(); // <-- In manual mode, you can choose to render imgui at a given moment in your rendering pipeline
            ofDrawBitmapStringHighlight( "I'm over the Gui thanks to manual draw !", 10, 40); // <-- This text will be drawn over the gui
            #endif

            // If shared mode is on together with autodraw, rendering will happen after this scope, using the ofApp::draw callback.
		}

        void update(){
            // Gui variables are also accessible outside of the draw() loop.
            int v1copy = v1;
            v1copy*=1;// silence "unused variable" warning !
        }

    private:
        ofxImGui::Gui gui;

        // Variables exposed to ImGui
        float v1 = 0;
        float backGroundColor[3] = {1,1,1};
        bool drawLines = false;
};
