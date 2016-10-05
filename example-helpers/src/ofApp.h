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

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

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

	// Gui
	bool imGui();

	ofxImGui::Gui gui;
	bool guiVisible;
	bool mouseOverGui;

	// Parameters
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
};
