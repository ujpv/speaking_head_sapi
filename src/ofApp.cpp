#include "string"

#include "ofApp.h"

#include "cpu_morpher.h"
//#include "simplespeaker.h"
#include "simplespeaker_sapi.h"
#include <memory>

//--------------------------------------------------------------
void ofApp::setup() {
	std::unique_ptr<AbstactMorpher> tmp(new CPU_Morpher("data\\"));
//    speaker = new SimpleSpeakerSapi();
	//speaker = new SimpleSpeaker();
	morpher = tmp.release();

	gui.setup("par_group");
	gui.add(w1.set("w1", 0, 0, 1.0));
	gui.add(w2.set("w2", 0, 0, 1.0));
	gui.add(v1.set("v1", 0, 0, 22));
	gui.add(v2.set("v2", 0, 0, 22));

	ofLoadImage(head_texture, "data\\pers_head_color_03.jpg");

	cam.setDistance(3);
	cam.setNearClip(1);
}

//--------------------------------------------------------------
void ofApp::update() {
		speaker->update();
		v1.set(speaker->get_previous());
		v2.set(speaker->get_current());
		w1.set(speaker->get_previous_w());
		w2.set(speaker->get_curren_w());
	morpher->set_current(v1, w1 * .7, v2, w2 * .7);
	morpher->update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	cam.begin();
	ofEnableDepthTest();
	ofEnableLighting();
	light.enable();
	light.setPosition(cam.getPosition());

	head_texture.bind();
	morpher->draw();
	head_texture.unbind();

	cam.end();
	ofDisableDepthTest();

	gui.draw();

	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(), 2), 5, 15);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	speaker->speak(L"");
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

ofApp::~ofApp()
{
	delete morpher;
	delete speaker;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
