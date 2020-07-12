#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
	inputPath = ofFilePath::getAbsolutePath("input");
	ofStringReplace(inputPath, "/", "\\");
	
	//gui.addFont("fonts\\Verdana.ttf");
	gui.setup();
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255);

	if (imgMain.isAllocated()) {
		imgMain.draw(0, 0);
	}
		
	gui.begin();
		drawGui(400, 20, 600, 600);
	gui.end();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::drawGui(int x, int y, int w, int h) {
	
	ofFill(); // It needs to be if previously was called ofNoFill
	ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);
	ImGui::Begin("img viewer");
	tempStrStream.str(std::string());
	tempStrStream << "input path: " << inputPath;

	if (ImGui::Selectable(tempStrStream.str().c_str())) {
		ofFileDialogResult result = ofSystemLoadDialog("select input folder", true);
		if (result.bSuccess) {
			inputPath = result.getPath();

			ofDirectory loadDir;
			loadDir.open(inputPath);
			loadDir.allowExt("jpg");
			loadDir.allowExt("png");

			imgNamesForListBox.clear();
			for (auto im : loadDir) {
				imgNamesForListBox.emplace_back(im.getFileName());
			}
		}
	}

	if (imgNamesForListBox.size() > 0) {
		ImGui::Text(("Nr of files: " + ofToString(imgNamesForListBox.size())).c_str());
	}
	
	ImGui::PushItemWidth(200);
	ImGui::ListBox("##imgfiles", &indexImgFile, imgNamesForListBox, 10);
	if (indexImgFile >= 0) {
		if (indexImgFile != prevIndexImgFile) {
			imgMain.loadImage(inputPath + "/" + imgNamesForListBox[indexImgFile]);
			prevIndexImgFile = indexImgFile;

			stringstream ss;
			ss << "name:" << imgNamesForListBox[indexImgFile] << endl;
			ss << "size:" << imgMain.getWidth() << "," << imgMain.getHeight() << endl;
			imgDescr = ss.str();
		}
	}
	ImGui::PopItemWidth();

	ImGui::SameLine(); ImGui::Text(imgDescr.c_str());
	ImGui::End();
}