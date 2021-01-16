#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "imgui_internal.h" // <-- example uses some imgui internals...

class ofApp : public ofBaseApp{

	public:
        ofApp() {}

		void setup() {
            // Setup imgui with the appropriate config flags
            gui.setup(nullptr, false, ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable, true);

            // Uncomment below to perform docking with SHIFT key
            // Gives a better user experience, matter of opinion.
            //ImGui::GetIO().ConfigDockingWithShift=true;

            ofSetBackgroundAuto(false);
            ofEnableAlphaBlending();
        }

		void draw() {
            // erase the bg progressively to reveal which ImGui window are within the OF window
            ofPushStyle();
            ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
            ofFill();
            ofSetColor(200, 200, 200, 2);
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofDrawRectangle(ofGetWidth()*.5f, ofGetHeight()*.5f, ofGetWidth()+20, ofGetHeight()+20);
            ofSetRectMode(OF_RECTMODE_CORNER);
            ofPopStyle();

            ofPushStyle();
            // Example info text
            ofDrawBitmapStringHighlight(
                        "Docking windows lets you easily re-arrange your window space.\n"\
                        "Start dragging a window and drop it on a dockable zone !\n\n"\

                        "Viewports : When you drag a window out of the oF window,\n"\
                        "it seamlessly leaves the oF window context to become standalone.\n"\
                        "if you see onion skins while dragging the window, it's in the same window as oF.",
                        100, 40
                        );
            ofPopStyle();


            // Start drawing to ImGui (newFrame)
			gui.begin();

            // Define the ofWindow as a docking space
            ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0,0,0,0)); // Fixes imgui to expected behaviour. Otherwise add in ImGui::DockSpace() [±line 14505] : if (flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;
            ImGuiID dockNodeID = ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);
            ImGui::PopStyleColor();

            ImGuiDockNode* dockNode = ImGui::DockBuilderGetNode(dockNodeID);
            if(dockNode){
                ImGuiDockNode* centralNode = ImGui::DockBuilderGetCentralNode(dockNodeID);
                // Verifies if the central node is empty (visible empty space for oF)
                if( centralNode && centralNode->IsEmpty() ){
                    ImRect availableSpace = centralNode->Rect();
                    //availableSpace.Max = availableSpace.Min + ImGui::GetContentRegionAvail();
                    ImGui::GetForegroundDrawList()->AddRect(availableSpace.GetTL()+ImVec2(8,8), availableSpace.GetBR()-ImVec2(8,8), IM_COL32(255,50,50,255));

                    ImVec2 viewCenter = availableSpace.GetCenter();
                    // Depending on the viewports flag, the XY is either absolute or relative to the oF window.
                    if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable ) viewCenter = viewCenter - ImVec2(ofGetWindowPositionX(),ofGetWindowPositionY());

                    ofPushStyle();
                    ofSetRectMode(OF_RECTMODE_CENTER);
                    ofSetColor(255,0,0,2);
                    ofNoFill();

                    ofDrawRectangle(
                                viewCenter.x,
                                viewCenter.y,
                                availableSpace.GetSize().x-6,
                                availableSpace.GetSize().y-6
                    );
                    ofNoFill();
                    ofSetColor(255,255,255,30);
                    ofDrawRectangle(
                                (viewCenter.x),
                                (viewCenter.y),
                                (availableSpace.GetSize().x-20)*fmodf(abs(sin(ofGetElapsedTimef())),1.f),
                                (availableSpace.GetSize().y-20)*fmodf(abs(sin(ofGetElapsedTimef())),1.f)
                    );
                    ofSetRectMode(OF_RECTMODE_CORNER);
                    ofPopStyle();
                }
            }

            // Draw a few windows
            // ImGuiWindowFlags_DockNodeHost => prevents hosting other windows, but still can be docked into another one.
            drawWindow("Dockable Window",   50,     100, ImGuiWindowFlags_None          );
            drawWindow("Window 1",          300,    100, ImGuiWindowFlags_DockNodeHost  );
            drawWindow("Window 2",          50,     350, ImGuiWindowFlags_DockNodeHost  );
            drawWindow("Window 3",          300,    350, ImGuiWindowFlags_DockNodeHost  );
            drawWindow("Window 4",          550,    100, ImGuiWindowFlags_DockNodeHost  );
            drawWindow("Window 5",          550,    350, ImGuiWindowFlags_DockNodeHost  );

            // End our ImGui Frame. Also Renders in autoDraw mode.
            gui.end();

            gui.draw(); // <-- In manual mode, you can choose to render imgui at a given moment in your pipeline

            // Show FPS
            ofDrawBitmapStringHighlight( ofToString(ofGetFrameRate() ), 10, 50, ofColor(0,60), ofColor(255,128)); // <-- This text will be drawn over the layout

            if(!dockNode || !ImGui::DockBuilderGetCentralNode(dockNodeID) || !ImGui::DockBuilderGetCentralNode(dockNodeID)->IsEmpty()){
                static int posX=1, posY=1, velX=1, velY=1;
                ofDrawBitmapStringHighlight("No empty dock space, nowhere to draw for oF !", posX, posY);
                posX+=velX*3;
                posY+=velY*3;
                ofBitmapFont f;
                ofRectangle strSize = f.getBoundingBox("No empty dock space, nowhere to draw for oF !",posX,posY);
                if(posX <= 0) velX=1;
                if(posY <= strSize.height) velY=1;
                if(posX >= ofGetWidth()-strSize.width) velX=-1;
                if(posY >= ofGetHeight()) velY=-1;
            }
		}

    private:

        void drawWindow(const char* _title, int _x=300, int _y=300, ImGuiWindowFlags _flags=ImGuiWindowFlags_None ){
            ImGui::SetNextWindowSize(ImVec2(200,200), ImGuiCond_Once);
            ImGui::SetNextWindowPos(ImVec2(_x+ofGetWindowPositionX(),_y+ofGetWindowPositionY()), ImGuiCond_Once);
            ImGui::Begin(_title, NULL, _flags );
            static int v = 0;
            ImGui::InputInt("InputInt", &v);
            ImGui::SliderInt("SliderInt", &v, 0, 10);
            ImGui::DragInt("DragInt", &v);
            ImGui::End();
        }

        ofxImGui::Gui gui;
};
