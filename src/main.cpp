#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	// ofSetupOpenGL(1000,1000,OF_WINDOW);			// <-------- setup the GL context
    // ofSetupOpenGL(3840,2160,OF_GAME_MODE);			// <-------- setup the GL context
	ofSetupOpenGL(1680,1050,OF_GAME_MODE);			// <-------- setup the GL context
    // 1680x1050
	ofRunApp(new ofApp());

}
