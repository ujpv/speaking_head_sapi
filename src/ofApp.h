#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "abstactmorpher.h"
#include "abstractspeaker.h"
#include "ini_loader.h"
#include "bot_client.h"

class ofApp : public ofBaseApp {

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
    void exit() override;

	~ofApp();

	ofxPanel gui;
	ofParameter<float> w1;
	ofParameter<float> w2;
	ofParameter<int> v1;
	ofParameter<int> v2;

private:
	AbstactMorpher * morpher;
	AbstractSpeaker *speaker;
	ofEasyCam cam;
	ofLight light;

	ofTexture head_texture;
	ofMesh    head;

    INILoader m_ini_file;
    const char const * INI_FILE_NAME = "options.ini";

    botClient client;
};
