#include "ofApp.h"
#include "IconsFontAwesome5.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //required call
    gui.setup(nullptr, ImGuiConfigFlags_DockingEnable);
    
    // Add polish characters
    static const ImWchar polishCharRanges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0100, 0x01FF, // Polish characters
        0,
    };
    auto normalCharRanges = ImGui::GetIO().Fonts->GetGlyphRangesDefault();

    customFont = gui.addFont("Roboto-Medium.ttf",16.f, nullptr, true?polishCharRanges:normalCharRanges);

    // Add fontawesome fonts by merging new glyphs
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    gui.addFont("fa-regular-400.ttf", 18.f, &config, icon_ranges);

    // For more advanced font loading examples, please refer to
    // https://github.com/ocornut/imgui/blob/master/docs/FONTS.md
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    // Start imgui
    gui.begin();

    ImGui::SetNextWindowPos(ImVec2(5,5), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(600-10,600-10), ImGuiCond_Once);
    ImGui::Begin("Font Examples");
    
    ImGui::Spacing();
    ImGui::CollapsingHeader("Default font", ImGuiTreeNodeFlags_Leaf);
    // Uses the default font
    ImGui::Text("Hello, world!");
    ImGui::Text(u8"Witaj świecie !");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(255,255,255,0.5), "<-- one character is not loaded in this font !");
    ImGui::Spacing();

    // Use 2ndary font
    ImGui::CollapsingHeader("Special characters", ImGuiTreeNodeFlags_Leaf);
    ImGui::PushFont(customFont);
    // u8 ensures text is encoded as utf8 from the cpp source code
    ImGui::Text(u8"Witaj świecie !");
    ImGui::Text(u8"Some polish characters: ć, ń, ó, ś, ź, ż, ą, ę, ł.");
    ImGui::PopFont();
    ImGui::Dummy(ImVec2(0,10));

    // Fontawesome
    ImGui::CollapsingHeader("Fontawesome icons", ImGuiTreeNodeFlags_Leaf);
    ImGui::PushFont(customFont);
    ImGui::Text("FontAweome is loaded too ! %s", ICON_FA_THUMBS_UP);
    ImGui::Button( ICON_FA_TRASH_ALT " Trash it !");
    ImGui::SameLine();
    ImGui::Button( ICON_FA_ANGRY " Mad button !");
    ImGui::SameLine();
    ImGui::Button( ICON_FA_BELL " Ring it !");
    ImGui::PopFont();
    ImGui::Dummy(ImVec2(0,10));

    // More
    ImGui::TextWrapped("For more advanced font loading examples, please refer to : ");
    ImGui::Text("https://github.com/ocornut/imgui/blob/master/docs/FONTS.md");
    ImGui::Dummy(ImVec2(0,10));

    // Show imgui font viewer / debugger
    ImGui::Dummy(ImVec2(0,10));
    ImGui::CollapsingHeader("ImGui Font Debugger", ImGuiTreeNodeFlags_Leaf);
    //ImGui::ShowFontSelector("Default font");
    ImGui::ShowStyleEditor();

    ImGui::End(); // close window
    
    // End imgui
    gui.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}


void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY)
{

}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
