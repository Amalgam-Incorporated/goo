#pragma once

#include "ofMain.h"

#include "algorithm"
#include "ofxDither.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    ofImage res;
    ofImage qr;
    ofxDither dither;

private:
    bool wait_for_qr();
    bool show_qr();
    bool wait_for_face();

    void band_control(int band, const char* msg);

    bool show_face_melting();

};
