#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

void FxTestBed(bool*); // Forward declared

// It is easy to extend ImGui making simple functions that use the ImGui API.
// If you need access to inner ImGui variables (eg: more complex tweaks), it's better to write an ImGui plugin and load that. (see imdrawlist_party.cpp)
// Code from ImGuiDemo.cpp
static void HelpMarker(const char* desc){
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

class ofApp : public ofBaseApp{

	public:
		ofApp() : v(0) {}

        void setup() {
            // Enable viewports, enable state saving
            gui.setup(nullptr, false, ImGuiConfigFlags_ViewportsEnable | ImGuiConfigFlags_DockingEnable, true);
		}

		void draw() {
            // Precalcs
            #define plotSize 200
            static float realFPS[plotSize]={0};
            static float avgFPS=0;
            avgFPS=0;
            for(int i=0; i<plotSize-1; ++i){
                avgFPS+=realFPS[i];
                realFPS[i]=realFPS[i+1];
            }
            realFPS[plotSize-1]=ofGetFrameRate();
            avgFPS=(avgFPS+realFPS[plotSize-1])/plotSize;

            // Start drawing to ImGui (newFrame)
			gui.begin();

            // Draw a menu bar
            ImGui::BeginMainMenuBar();

            if(ImGui::BeginMenu( "Options")){

                ImGui::Checkbox("Show ImGui Metrics", &showMetrics);
                ImGui::SameLine(); HelpMarker("The ImGui metrics is a gui window allowing you to inspect what is going on and how imgui is intagrated and interfaced with oF.");

                ImGui::Separator();
                ImGui::Checkbox("Show ImGui Demo", &showDemo);
                ImGui::SameLine(); HelpMarker("The ImGui Demo Window demonstrates many gui possibilities and serves as a reference for available gui widgets, together with its source code.");

                ImGui::Separator();
                ImGui::Checkbox("Show ImGui FX", &showFX);
                ImGui::SameLine(); HelpMarker("Demoscene contest !\nDemonstrates :\n - how to make an ImGui plugin\n - How to use the imgui drawlist api");

                ImGui::Separator();
                ImGui::Checkbox("Draw lines", &drawLines);

                // Submenu
                ImGui::Separator();
                if(ImGui::BeginMenu( "More..." )){
                    ImGui::MenuItem( "Something" );
                    ImGui::MenuItem( "Something else" );
                    ImGui::MenuItem( "Something different" );
                    ImGui::EndMenu();
                }

                // Exit
                ImGui::Separator();
                if(ImGui::MenuItem( "Quit" )){
                    ofExit();
                }

                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu( "Runtime")){

                // Full Screen
                static bool fullScreen = false;
                if(ImGui::Checkbox("Full screen", &fullScreen)){
                    ofSetFullscreen(fullScreen);
                }

                // Vertical Sync
                static bool vSync = false;
                if(ImGui::Checkbox("Vertical Sync", &vSync)){
                    ofSetVerticalSync(vSync);
                }

                ImGui::Separator();

                // Resolution changer
                static int resolution[2];
                resolution[0]=ofGetWidth();
                resolution[1]=ofGetHeight();
                std::string resString = ofToString(resolution[0]).append(" x ").append(ofToString(resolution[1]));
                if(ImGui::BeginCombo("Resolution", resString.c_str())){
                    if(ImGui::Selectable("800 × 600")){
                        ofSetWindowShape(800,600);
                    }
                    if(ImGui::Selectable("1024 × 768")){
                        ofSetWindowShape(1024, 768);
                    }
                    if(ImGui::Selectable("1366 × 768")){
                        ofSetWindowShape(1366, 768);
                    }
                    if(ImGui::InputInt2("Custom", resolution)){
                        ofSetWindowShape(resolution[0],resolution[1]);
                    }
                    ImGui::EndCombo();
                }

                // FPS
                static int appFPS=30;//ofGetTargetFrameRate();
                if(ImGui::DragInt("FPS Target", &appFPS, 1.f, 10)){ // Minimum set to 10 because double click is not detected below it
                    ofSetFrameRate(appFPS);
                    appFPS=ofGetTargetFrameRate(); // re-sync value with oF
                }
                if(ImGui::IsItemHovered()){
                    ImGui::BeginTooltip();
                    ImGui::Text("(double-click to edit)");
                    ImGui::EndTooltip();
                }
                static char avgFPSString[28]={0};
                sprintf(avgFPSString, "FPS : %.2f average", avgFPS);
                ImGui::PlotHistogram("FPS Histogram", realFPS, plotSize, 0, avgFPSString, 0, 60);

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();

            // Line drawing logic
            glm::vec2 windowPos;
            // With viewports enabled, coordinates are absolute
            if(ImGui::GetIO().ConfigFlags &= ImGuiConfigFlags_ViewportsEnable)  windowPos -= glm::vec2(ofGetWindowPositionX(), ofGetWindowPositionY());
            glm::vec2 windowSize;
            static bool isWindowDragging = false;
            static bool isWindowFocused = false;

            // Create a new window
            // ImGuiCond_Once => Set on launch, then let user use their size
            ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->Pos + ImVec2(((ofRandomf()+1.f)*.25f)*ofGetWidth(), ((ofRandomf()+1.f)*.25f)*ofGetHeight()), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(320,240), ImGuiCond_Once);

            if(ImGui::Begin("Main Window\n")){
                ImGui::Text("Example-Advanced");
                ImGui::TextWrapped(
                    "All windows except this one should restore their size and positions when re-opening this ofApp.\n\n"\
                    "The graphics around this window are pimped by oF. Use the options menu to draw lines instead.\n\n"
                );
                ImGui::Text("There are some help indicators around.");
                ImGui::SameLine(); HelpMarker("Bravo, you got it !");
            }
            // Query ImGui window state
            // Note: Helpers help convert ImVec2 to glm::Vec2
            windowPos         = ImGui::GetWindowPos();
            if(ImGui::GetIO().ConfigFlags &= ImGuiConfigFlags_ViewportsEnable)  windowPos -= glm::vec2(ofGetWindowPositionX(), ofGetWindowPositionY()); // with viewports enabled, coordinates change
            windowSize        = ImGui::GetWindowSize();
            isWindowDragging  = ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left);
            isWindowFocused   = ImGui::IsWindowFocused();

            ImGui::End(); // end Main Window

            static float speed1 = 0.072f;
            static float speed2 = 0.103f;
            static float variation1 = 0.0147f;
            static float variation2 = 0.032f;
            static float offset1 = 3.4f;
            static float offset2 = 5.1f;
            static ImVec4 color = ImVec4(1.f, 1.f, 1.f, 1.f);

            // Line Settings window
            if( drawLines ){
                ImGui::Begin("Line Settings", &drawLines );

                if( ImGui::CollapsingHeader("Line settings", ImGuiTreeNodeFlags_DefaultOpen) ){

                    ImGui::TreePush();

                    ImGui::Dummy(ImVec2(20,5));

                    ImGui::SliderInt("Lines per swarm", &linesPerSide, 1, 1000);
                    ImGui::DragFloat("Line Spacing", &linesSpacing, 0.0001f, 0.001f, 0.5f, "%.4f");
                    ImGui::DragFloat("Line Thickness", &lineThickness, 0.01f, 0.01f, 2.f, "%.2f");
                    ImGui::ColorEdit4("Color", (float*)&color);

                    ImGui::Dummy(ImVec2(20,10));

                    // It's also easy to add extra Gui interactions to fit your needs.
                    ImGui::SliderFloat("Speed 1", &speed1, 0.001f, 1.5f);
                    if(ImGui::IsItemActive() && ofGetKeyPressed(OF_KEY_SHIFT))
                        speed2 = speed1;
                    ImGui::SameLine(); HelpMarker("Hold shift to sync both speeds");
                    ImGui::SliderFloat("Speed 2", &speed2, 0.001f, 1.5f);
                    if(ImGui::IsItemActive() && ofGetKeyPressed(OF_KEY_SHIFT))
                        speed1 = speed2;
                    ImGui::SameLine(); HelpMarker("Hold shift to sync both speeds");

                    ImGui::Separator();
                    ImGui::SliderFloat("Speed Variation 1", &variation1, 0.001f, 1.5f);
                    ImGui::SliderFloat("Speed Variation 2", &variation2, 0.001f, 1.5f);

                    ImGui::Separator();
                    ImGui::SliderFloat("Offset 1", &offset1, 0.001f, 10.f);
                    ImGui::SliderFloat("Offset 2", &offset2, 0.001f, 10.f);

                    ImGui::TreePop();
                }
                // Close the line settings window
                ImGui::End();
            }

            if(showMetrics){
                ImGui::SetNextWindowPos(ImVec2(ofGetWindowPositionX()+ofGetWidth()-350,ofGetWindowPositionY()+30), ImGuiCond_FirstUseEver );
                ImGui::ShowMetricsWindow( &showMetrics );
            }
            if(showDemo) ImGui::ShowDemoWindow(&showDemo);
            if(showFX){
                // Window position is only set on the first launch
                ImGui::SetNextWindowPos(ImVec2(ofGetWindowPositionX()+ofGetWidth()-350,ofGetWindowPositionY()+ofGetHeight()-230), ImGuiCond_FirstUseEver );
                FxTestBed(&showFX); // <-- ImGui extension from im_drawlist_party.cpp
            }

            gui.end();

            // Line drawing variables
            glm::vec2 windowTopLeft     = windowPos;
            glm::vec2 windowTopRight    = windowTopLeft   + glm::vec2(windowSize.x,0);
            glm::vec2 windowBottomLeft  = windowTopLeft   + glm::vec2(0, windowSize.y);
            glm::vec2 windowBottomRight = windowTopLeft   + windowSize;

            glm::vec2 ofWindowTopLeft     = glm::vec2(0                 ,0);
            glm::vec2 ofWindowTopRight    = glm::vec2(ofGetWindowWidth(),0);
            glm::vec2 ofWindowBottomLeft  = glm::vec2(0                 ,ofGetWindowHeight());
            glm::vec2 ofWindowBottomRight = glm::vec2(ofGetWindowWidth(),ofGetWindowHeight());

            if(drawLines){
                ofSetLineWidth(lineThickness);
                ofSetColor(color.x*255.f, color.y*255.f, color.z*255.f, color.w*255.f);
                const static int numSides = 4;
                for(int i=0; i < linesPerSide*numSides; i++){
                    glm::vec2 fromA  = (i<linesPerSide)? ofWindowTopLeft  :((i<linesPerSide*2)? ofWindowTopRight    :((i<linesPerSide*3)? ofWindowBottomRight : ofWindowBottomLeft));
                    glm::vec2 toA    = (i<linesPerSide)? windowTopLeft    :((i<linesPerSide*2)? windowTopRight      :((i<linesPerSide*3)? windowBottomRight   : windowBottomLeft  ));
                    glm::vec2 fromB  = (i<linesPerSide)? ofWindowTopRight :((i<linesPerSide*2)? ofWindowBottomRight :((i<linesPerSide*3)? ofWindowBottomLeft  : ofWindowTopLeft   ));
                    glm::vec2 toB    = (i<linesPerSide)? windowTopRight   :((i<linesPerSide*2)? windowBottomRight   :((i<linesPerSide*3)? windowBottomLeft    : windowTopLeft     ));
                    unsigned int sideNum= (i<linesPerSide)? 0             :((i<linesPerSide*2)? 1                   :((i<linesPerSide*3)? 2                   : 3                 ));
                    int lineNum      = i % linesPerSide;
                    int variantA     = (sideNum+0)%numSides;
                    int variantB     = (sideNum+1)%numSides;
                    ofDrawLine(
                      //                                                     |Speed  | A/B variation                    |       Spacing        | Differentiation
                      ofInterpolateCosine( fromA , toA , ofGetElapsedTimef()*(speed1 + variation1 * variantA * 0.2f )   + lineNum*linesSpacing + ((variantA)%2)*5.3f*offset1),
                      ofInterpolateCosine( fromB,  toB,  ofGetElapsedTimef()*(speed1 + variation1 * variantB * 0.2f )   + lineNum*linesSpacing + ((variantB)%2)*5.3f*offset1)
                    );
                    ofDrawLine(
                      ofInterpolateCosine( fromA , toA , ofGetElapsedTimef()*(speed2 + variation2* variantA * 0.2f )   + lineNum*linesSpacing + ((variantB)%2)*5.3f*offset2),
                      ofInterpolateCosine( fromB,  toB,  ofGetElapsedTimef()*(speed2 + variation2* variantB * 0.2f )   + lineNum*linesSpacing + ((variantA)%2)*5.3f*offset2)
                    );
                }
                ofSetLineWidth(1.f);
            }
            else {
                // Connect Window corners with OF corners
                ofDrawLine(ofWindowTopLeft    , windowTopLeft    );
                ofDrawLine(ofWindowTopRight   , windowTopRight   );
                ofDrawLine(ofWindowBottomRight, windowBottomRight);
                ofDrawLine(ofWindowBottomLeft , windowBottomLeft );

                // Draw a rectangle behind the window
                ofFill();
                if(isWindowDragging) ofSetColor(255,0,0,126);
                else ofSetColor(255,isWindowFocused?80:40);
                ofDrawRectRounded(windowTopLeft-glm::vec2(10), windowSize.x+20, windowSize.y+20, 10);

                // Draw Circles on corners
                ofSetColor(255,255);
                ofNoFill();
                ofDrawCircle(windowTopLeft    , glm::mod( ofGetElapsedTimef()*10.f     , 10.f) );
                ofDrawCircle(windowTopRight   , glm::mod( ofGetElapsedTimef()*10.f+5.0f, 10.f) );
                ofDrawCircle(windowBottomLeft , glm::mod( ofGetElapsedTimef()*10.f+2.5f, 10.f) );
                ofDrawCircle(windowBottomRight, glm::mod( ofGetElapsedTimef()*10.f+7.5f, 10.f) );
            }

            gui.draw();

        }

	private:
        ofxImGui::Gui gui;

        // Variables exposed to ImGui
        bool showMetrics = true;
        bool showDemo = false;
        bool showFX = true;

        float v = 0;

        int   linesPerSide  = 15;
        float linesSpacing  = 0.02f;
        float lineThickness = 0.8f;
        bool  drawLines     = false;
};
