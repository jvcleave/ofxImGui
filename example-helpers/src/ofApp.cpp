#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_NOTICE);
	ofDisableArbTex();
	ofSetBackgroundAuto(false);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	this->stepper = 0.0f;

	// Gui
	this->gui.setup();
	this->guiVisible = true;
}

//--------------------------------------------------------------
void ofApp::update()
{
	this->stepper += this->speed;
	cubeSize = ofMap(sinf(this->stepper), -1.0f, 1.0f, this->sizeMin, this->sizeMax);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// We have to use ofParameter::get() since this we are using an ofFloatColor.
	ofClear(this->background.get());
	
	this->camera.begin();
	{
		ofEnableDepthTest();

		ofSetColor(ofColor::white);

		if (this->enabled)
		{
			ofPushStyle();
			{
				// We have to use ofParameter::get() since this we are using an ofFloatColor.
				ofSetColor(this->foreground.get());

				auto fillRender = static_cast<RenderMode>(this->fillMode.get());
				if (fillRender != RenderMode::None)
				{
					ofFill();
					if (fillRender == RenderMode::Texture && this->texture.isAllocated())
					{
						this->texture.bind();
					}
					ofDrawBox(this->cubeSize);
					if (fillRender == RenderMode::Texture && this->texture.isAllocated())
					{
						this->texture.unbind();
					}
				}

				auto strokeRender = static_cast<RenderMode>(this->strokeMode.get());
				if (strokeRender != RenderMode::None)
				{
					ofNoFill();
					if (strokeRender == RenderMode::Texture && this->texture.isAllocated())
					{
						this->texture.bind();
					}
					ofDrawBox(this->cubeSize);
					if (strokeRender == RenderMode::Texture && this->texture.isAllocated())
					{
						this->texture.unbind();
					}
				}
			}
			ofPopStyle();
		}

		ofDisableDepthTest();
	}
	this->camera.end();

	// Gui
	this->mouseOverGui = false;
	if (this->guiVisible)
	{
		this->mouseOverGui = this->imGui();
	}
	if (this->mouseOverGui)
	{
		this->camera.disableMouseInput();
	}
	else
	{
		this->camera.enableMouseInput();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

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

//--------------------------------------------------------------
bool ofApp::loadImage(const string & filePath)
{
	ofImage image;
	image.setUseTexture(false);
	if (!image.load(filePath)) 
	{
		ofLogError(__FUNCTION__) << "No image found at " << filePath;
		return false;
	}

	ofTextureData texData;
	texData.width = image.getWidth();
	texData.height = image.getHeight();
	texData.textureTarget = GL_TEXTURE_2D;
	texData.bFlipTexture = true;
	this->texture.allocate(texData);
	this->texture.loadData(image.getPixels());

	this->imagePath = ofFilePath::makeRelative(ofToDataPath(""), filePath);
	return true;
}

//--------------------------------------------------------------
bool ofApp::imGui()
{
	auto mainSettings = ofxImGuiExt::Settings();

	this->gui.begin();
	{
		if (ofxImGuiExt::BeginWindow("Helpers", mainSettings, false))
		{
			ImGui::Text("%.1f FPS (%.3f ms/frame)", ofGetFrameRate(), 1000.0f / ImGui::GetIO().Framerate);

			if (ofxImGuiExt::BeginTree(this->colors, mainSettings))
			{
				ofxImGuiExt::AddParameter(this->background, false);
				ofxImGuiExt::AddParameter(this->foreground);

				ofxImGuiExt::EndTree(mainSettings);
			}

			if (ofxImGuiExt::BeginTree(this->mesh, mainSettings))
			{
				ofxImGuiExt::AddParameter(this->enabled);
				ofxImGuiExt::AddRange("Size Range", this->sizeMin, this->sizeMax, 1.0f);
				ofxImGuiExt::AddParameter(this->speed);
				ImGui::Text("Size: %.2f", this->cubeSize);

				ofxImGuiExt::EndTree(mainSettings);
			}

			if (ofxImGuiExt::BeginTree(this->render, mainSettings))
			{
				if (ImGui::Button("Load Image..."))
				{
					auto dialogResult = ofSystemLoadDialog("Load Image", false, ofToDataPath(""));
					if (dialogResult.bSuccess)
					{
						this->loadImage(dialogResult.filePath);
					}
				}

				static const vector<string> labels = { "None", "Color", "Texture" };

				ofxImGuiExt::AddRadio(this->fillMode, labels, 3);
				ofxImGuiExt::AddRadio(this->strokeMode, labels, 3);

				if (this->texture.isAllocated())
				{
					ofxImGuiExt::AddParameter(this->preview);
				}

				ofxImGuiExt::EndTree(mainSettings);
			}
		}
		ofxImGuiExt::EndWindow(mainSettings);

		if (this->preview)
		{
			static const float kPreviewSize = 256.0f;
			auto previewSettings = ofxImGuiExt::Settings();
			previewSettings.windowPos = glm::vec2(ofGetWidth() - kPreviewSize - kGuiMargin * 3, kGuiMargin);
			previewSettings.windowSize = glm::vec2(kPreviewSize, kPreviewSize);

			if (ofxImGuiExt::BeginWindow(this->preview, previewSettings, false))
			{
				ofxImGuiExt::AddImage(this->texture, previewSettings.windowSize);
			}
			ofxImGuiExt::EndWindow(previewSettings);
		}
	}
	this->gui.end();

	return mainSettings.mouseOverGui;
}
