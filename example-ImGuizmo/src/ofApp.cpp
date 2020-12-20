#include "ofApp.h"
#include <glm/gtx/matrix_decompose.hpp>


//--------------------------------------------------------------
void ofApp::setup(){
	gui_.setup();
	cam_.setPosition({0,0,100});
	cam_.lookAt({0,0,0});
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	cam_.begin();
	node_.draw();
	cam_.end();
	gui_.begin();
	ImGuizmo::BeginFrame();
	auto mat = node_.getGlobalTransformMatrix();
	if(ImGuizmo::Manipulate(cam_, mat, op_, mode_)) {
		glm::mat4 transformation;
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(mat, scale, rotation, translation, skew, perspective);
		node_.setPosition(translation);
		node_.setScale(scale);
		node_.setOrientation(rotation);
	}
	gui_.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case 'w': op_ = ImGuizmo::TRANSLATE; break;
		case 'e': op_ = ImGuizmo::SCALE; break;
		case 'r': op_ = ImGuizmo::ROTATE; break;
		case ' ': mode_ = mode_==ImGuizmo::LOCAL?ImGuizmo::WORLD:ImGuizmo::LOCAL; break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
