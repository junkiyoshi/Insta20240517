#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);

	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

	int base_radius = (ofGetFrameNum() * 2) % 420 + 60;

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		if (glm::length(this->log_list[i].back()) > 720) {

			this->log_list.erase(this->log_list.begin() + i);
			this->seed_list.erase(this->seed_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);

			continue;
		}

		auto feature = this->velocity_list[i] * 120;
		auto deg = ofMap(ofNoise(this->seed_list[i].z, ofGetFrameNum() * 0.005), 0, 1, 0, 360);
		feature += glm::vec2(30 * cos(deg * DEG_TO_RAD), 30 * sin(deg * DEG_TO_RAD));
		auto next = this->log_list[i].back() + glm::normalize(feature) * 12;

		this->log_list[i].push_back(next);

		while (this->log_list[i].size() > 20) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}

	for (int i = 0; i < 3; i++) {

		int deg = ofGetFrameNum() * 1.44 + (i % 3) * 120;
		int next_deg = deg + 1;
		float radius = ofRandom(85, 95);

		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		auto next = glm::vec2(radius * cos(next_deg * DEG_TO_RAD), radius * sin(next_deg * DEG_TO_RAD));

		vector<glm::vec2> log;
		log.push_back(location);
		this->log_list.push_back(log);

		this->seed_list.push_back(glm::vec3(ofRandom(1000), ofRandom(1000), ofMap(ofNoise(location * 0.01), 0, 1, 0, 1000)));

		this->velocity_list.push_back(glm::normalize(location - next));
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	ofNoFill();
	for (int i = 0; i < this->log_list.size(); i++) {

		ofPushMatrix();
		ofTranslate(this->log_list[i].back());

		ofRotateY(ofMap(ofNoise(this->seed_list[i].y, ofGetFrameNum() * 0.025), 0, 1, 0, 360));
		ofRotateX(ofMap(ofNoise(this->seed_list[i].x, ofGetFrameNum() * 0.025), 0, 1, 0, 360));

		ofFill();
		ofSetColor(255);
		ofDrawRectangle(glm::vec3(), 40, 30);

		ofNoFill();
		ofSetColor(0);
		ofDrawRectangle(glm::vec3(), 40, 30);

		ofFill();
		ofSetColor(180, 0, 45);
		ofDrawCircle(glm::vec3(0, 0, 1), 6);
		ofDrawCircle(glm::vec3(0, 0, -1), 6);

		ofPopMatrix();
	}

	/*
	int start = 150;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}