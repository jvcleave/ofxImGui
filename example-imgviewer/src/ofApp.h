#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ImGuiExtension.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
	void drawGui(int x, int y, int w, int h);

	void mousePressed(int x, int y, int button);
	void keyReleased(int key);

private:
	ofImage imgMain;
	ofxImGui::Gui gui;
	string inputPath;
	string imgDescr = "";
	vector<string> imgNamesForListBox;
	stringstream tempStrStream;
	int indexImgFile = -1;
	int prevIndexImgFile = -1;
};
