#include "ofApp.h"

#define UID "wt001"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_NOTICE);
    ofDisableArbTex();
    ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    ofFile file("settings.xml");
    if(!file.exists()){
        saveToFile("settings.xml", servos_parameters);
    }else {
        loadFromFile("settings.xml", servos_parameters);
    }

    // Gui
    this->gui.setup();
    this->guiVisible = true;
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofClear(this->background.get());

    // Gui
    this->mouseOverGui = false;
    if (this->guiVisible) {
        this->mouseOverGui = this->imGui();
    }
    if (this->mouseOverGui) {

    }else {

    }
}

//--------------------------------------------------------------
void ofApp::exit()
{

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {    
    ofLogVerbose(__FUNCTION__) << key;
    switch (key)
    {
        case 'l' :
        {
            loadFromFile("settings.xml", servos_parameters);
            break;
        }
        case 's' :
        {
            saveToFile("settings.xml", servos_parameters);
            break;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

static void ShowHelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

//--------------------------------------------------------------
bool ofApp::imGui()
{
    static bool show_app_about = false;
    static bool show_app_servo_settings = true;
    auto mainSettings = ofxImGui::Settings();

    this->gui.begin();
    {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ShowAppMenuFile();
                ImGui::EndMenu();
            }
        if (ImGui::BeginMenu("Help")) {
            ImGui::MenuItem("About ofxImgui Parameters examples", NULL, &show_app_about);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
        }

        if (show_app_about) {
            ImGui::Begin("About ofxImgui Parameters examples", &show_app_about, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("ofxImgui Parameters examples, 0.01");
            ImGui::Separator();
            ImGui::Text("By ofxImgui.");
            ImGui::End();
        }

        if (show_app_servo_settings) {
            if (ofxImGui::BeginWindow("Servo Settings", mainSettings, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("UID: %2s", UID);
                ImGui::SameLine();
                ShowHelpMarker("Device serial number.");
                ImGui::SameLine();
                ImGui::Text("FW Version: 0.0.1");
                ImGui::SameLine();
                ImGui::Text("Timeouts: 0");
                ImGui::SameLine();
                if (ofxImGui::AddParameter(this->servo_statusled)){
                }
                ImGui::SameLine();
                if (ImGui::Button("Reset")) {
                }
                ImGui::Separator();
                //ImGui::AlignTextToFramePadding();
                ImGui::PushItemWidth(145);
                static const std::vector<std::string> servos_select = { "Servo 0", "Servo 1", "Servo 2", "Servo 3", "Servo 4", "Servo 5", "Servo 6", "All Servos" };
                ofxImGui::AddCombo(this->servoSelect, servos_select);
                ImGui::SameLine();
                if (this->servoSelect.get() == 0) {
                    if (ofxImGui::AddParameter(this->servo0_enabled)) {
                    }
                    ImGui::Text("Current Consumption: %s mA", "0" );
                    ImGui::Text("Pulse Width min/max (µs):");
                    ImGui::SameLine();
                    if (ofxImGui::AddStepper(this->servo0_Pulse_Width_min, 100, 100)) {
                    }
                    ImGui::SameLine();
                    ImGui::Text(" / ");
                    ImGui::SameLine();
                    if (ofxImGui::AddStepper(this->servo0_Pulse_Width_max, 100, 100)) {
                    }
                    ImGui::Text("Degree min/max (°/100):");
                    ImGui::SameLine();
                    if(ofxImGui::AddStepper(this->servo0_Degree_min, 500, 1000)) {
                    }
                    ImGui::SameLine();
                    ImGui::Text(" / ");
                    ImGui::SameLine();
                    if(ofxImGui::AddStepper(this->servo0_Degree_max, 500, 1000)) {
                    }
                    ImGui::Separator();
                    if(ofxImGui::AddParameter(this->servo0_Position)) {
                    }
                    if(ofxImGui::AddParameter(this->servo0_Velocity)) {
                    }
                    if(ofxImGui::AddParameter(this->servo0_Acceleration)) {
                    }
                    if(ofxImGui::AddParameter(this->servo0_Period)) {
                    }
                }else if (this->servoSelect.get() == 1) {
                    ofxImGui::AddParameter(this->servo1_enabled);
                }else if (this->servoSelect.get() == 2) {
                    ofxImGui::AddParameter(this->servo2_enabled);
                }else if (this->servoSelect.get() == 3) {
                    ofxImGui::AddParameter(this->servo3_enabled);
                }else if (this->servoSelect.get() == 4) {
                    ofxImGui::AddParameter(this->servo4_enabled);
                }else if (this->servoSelect.get() == 5) {
                    ofxImGui::AddParameter(this->servo5_enabled);
                }else if (this->servoSelect.get() == 6) {
                    ofxImGui::AddParameter(this->servo6_enabled);
                }else if (this->servoSelect.get() == 7) {
                    ofxImGui::AddParameter(this->allservos_enabled);
                }
                ImGui::Separator();
                ofxImGui::EndWindow(mainSettings);
            }
        }
    }

    this->gui.end();
    return mainSettings.mouseOverGui;
}

//--------------------------------------------------------------
void ofApp::ShowAppMainMenuBar() {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowAppMenuFile();
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

//--------------------------------------------------------------
void ofApp::ShowAppMenuFile()
{
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open Configuration", "l")) {loadFromFile("settings.xml", servos_parameters);}
    if (ImGui::MenuItem("Save Configuration", "s")) {saveToFile("settings.xml", servos_parameters);}
    if (ImGui::MenuItem("Save Configuration As..")) {}
    ImGui::Separator();
    ImGui::Separator();
    if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

//--------------------------------------------------------------
void ofApp::loadFromFile(const std::string& filename, ofAbstractParameter & parameter){
	ofXml xml;
	xml.load(filename);
	ofDeserialize(xml, parameter);
}

//--------------------------------------------------------------
void ofApp::saveToFile(const std::string& filename, ofAbstractParameter & parameter){
	ofXml xml;
	ofSerialize(xml, parameter);
	xml.save(filename);
}

//--------------------------------------------------------------
void ofApp::SaveAsSettings() {

}

