#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_NOTICE);
	ofDisableArbTex();
	ofSetBackgroundAuto(false);
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	stepper = 0.0f;

	//use keys 1 / 2 to enable one gui. can't use both at the same time on same class (ofApp)
	guiVisible = false;
	guiVisible2 = true;

	// Gui
	gui.setup();

	// Gui2
	gui2.setup();
	params.setName("myOfParameterGroup");
	params.add(shapeType.set("shape", 1, 1, 2));
	params.add(numShapes.set("num squares", 1, 1, 24));
	params.add(separation.set("separation", 5, 1, 100));
	params.add(shapeSide.set("square side", 50, 5, 200));
	paramsNested.setName("style");
	paramsNested.add(fill.set("fill", false));
	paramsNested.add(color.set("color", ofColor(0, 0), ofColor(0, 0), ofColor(1, 1)));
	paramsNested.add(lineWidth.set("lineWidth", 1, 0.1, 10));
	params.add(paramsNested);
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

	drawScene();

	//-

	// Gui
	mouseOverGui = false;
	if (guiVisible)
	{
		mouseOverGui = imGui();
	}

	//-

	// Gui2
	mouseOverGui2 = false;
	if (guiVisible2)
	{
		mouseOverGui2 = imGui2();
	}

	//-

	//// mouseOverGui
	//if (mouseOverGui || mouseOverGui2)
	//{
	//	camera.disableMouseInput();
	//}
	//else
	//{
	//	camera.enableMouseInput();
	//}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == '1') guiVisible = !guiVisible;
	if (key == '2') guiVisible2 = !guiVisible2;
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
	auto mainSettings = ofxImGui::Settings();

	gui.begin();
	{
		if (ofxImGui::BeginWindow("Helpers", mainSettings, false))
		{
			ImGui::Text("%.1f FPS (%.3f ms/frame)", ofGetFrameRate(), 1000.0f / ImGui::GetIO().Framerate);

			if (ofxImGui::BeginTree(this->colors, mainSettings))
			{
				ofxImGui::AddParameter(this->background, false);
				ofxImGui::AddParameter(this->foreground);

				ofxImGui::EndTree(mainSettings);
			}

			if (ofxImGui::BeginTree(this->mesh, mainSettings))
			{
				ofxImGui::AddParameter(this->enabled);
				ofxImGui::AddRange("Size Range", this->sizeMin, this->sizeMax, 1.0f);
				ofxImGui::AddParameter(this->speed);
				ImGui::Text("Size: %.2f", this->cubeSize);

				ofxImGui::EndTree(mainSettings);
			}

			if (ofxImGui::BeginTree(this->render, mainSettings))
			{
				if (ImGui::Button("Load Image..."))
				{
					auto dialogResult = ofSystemLoadDialog("Load Image", false, ofToDataPath(""));
					if (dialogResult.bSuccess)
					{
						this->loadImage(dialogResult.filePath);
					}
				}

				static const std::vector<std::string> labels = { "None", "Color", "Texture" };

				ofxImGui::AddRadio(this->fillMode, labels, 3);
				ofxImGui::AddRadio(this->strokeMode, labels, 3);

				if (this->texture.isAllocated())
				{
					ofxImGui::AddParameter(this->preview);
				}

				ofxImGui::EndTree(mainSettings);
			}
		}
		ofxImGui::EndWindow(mainSettings);

		if (this->preview)
		{
			static const float kPreviewSize = 256.0f;
			auto previewSettings = ofxImGui::Settings();
			previewSettings.windowPos = ofVec2f(ofGetWidth() - kPreviewSize - kImGuiMargin * 3, kImGuiMargin);
			previewSettings.windowSize = ofVec2f(kPreviewSize, kPreviewSize);

			if (ofxImGui::BeginWindow(this->preview, previewSettings, false))
			{
				ofxImGui::AddImage(this->texture, previewSettings.windowSize);
			}
			ofxImGui::EndWindow(previewSettings);
		}
	}
	gui.end();

	return mainSettings.mouseOverGui;
}

//--------------------------------------------------------------
bool ofApp::imGui2() {
	
	gui2.begin();
	{
		//raw ImGui code
		if (ImGui::Button("Load Image..."))
		{
			auto dialogResult = ofSystemLoadDialog("Load Image", false, ofToDataPath(""));
			if (dialogResult.bSuccess)
			{
				this->loadImage(dialogResult.filePath);
			}
		}

		//ofxImGui helpers not working with instances on same class (ofApp)
		//ofVec2f pos(100, 100);
		//auto mainSettings = ofxImGui::Settings();
		//mainSettings.windowPos = pos;
		//ImGui::SetNextWindowPos(ofVec2f(pos.x, pos.y));
		//if (ofxImGui::BeginTree(this->mesh, mainSettings))
		//{
		//	ofxImGui::AddParameter(fill);
		//	ofxImGui::AddParameter(numShapes);
		//	ofxImGui::EndTree(mainSettings);
		//}
		//ofxImGui::AddGroup(params, mainSettings);
	}
	gui2.end();
	
	//return mainSettings.mouseOverGui;
	return false;
}


//--------------------------------------------------------------
void ofApp::drawScene()
{
	// Scene
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
}