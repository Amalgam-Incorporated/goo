#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	// ofSetupOpenGL(1000,1000,OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(2160,2160,OF_GAME_MODE);			// <-------- setup the GL context
	ofRunApp(new ofApp());

}
