#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

class ofApp 
	: public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void drawScene();
	void keyPressed(int key);

protected:
	enum class RenderMode
	{
		None,
		Color,
		Texture
	};

	// Camera
	ofEasyCam camera;

	// Mesh
	float stepper;
	float cubeSize;

	// Render
	bool loadImage(const string & filePath);

	ofTexture texture;


	// Gui1
	bool imGui();
	bool mouseOverGui;
	bool guiVisible;
	ofxImGui::Gui gui;
	ofParameter<ofFloatColor> background{ "Background", ofFloatColor::black };
	ofParameter<ofFloatColor> foreground{ "Foreground", ofFloatColor::crimson };
	ofParameterGroup colors{ "Colors", background, foreground };
	ofParameter<bool> enabled{ "Enabled", true };
	ofParameter<float> sizeMin{ "Size Min", 10.0f, 0.0f, 1000.0f };
	ofParameter<float> sizeMax{ "Size Max", 200.0f, 0.0f, 1000.0f };
	ofParameter<float> speed{ "Speed", 0.1f, 0.0f, 1.0f };
	ofParameterGroup mesh{ "Mesh", enabled, sizeMin, sizeMax, speed };
	ofParameter<int> fillMode{ "Fill Mode", static_cast<int>(RenderMode::Texture) };
	ofParameter<int> strokeMode{ "Stroke Mode", static_cast<int>(RenderMode::None) };
	ofParameter<bool> preview{ "Preview", false };
	ofParameter<string> imagePath{ "Image Path", "texture.jpg" };
	ofParameterGroup render{ "Render", fillMode, strokeMode, preview, imagePath };
	
	// Gui2
	bool imGui2();
	bool guiVisible2;
	bool mouseOverGui2;
	ofxImGui::Gui gui2;
	ofParameterGroup params;
	ofParameterGroup paramsNested;
	ofParameter<bool> fill;
	ofParameter<float> lineWidth;
	ofParameter<ofFloatColor> color;
	ofParameter<int> shapeType;
	ofParameter<int> numShapes;
	ofParameter<int> separation;
	ofParameter<int> shapeSide;
};
