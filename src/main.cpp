#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    if (0)
        ofSetupOpenGL(1000,1000, OF_WINDOW);			// <-------- setup the GL context
    else
        ofSetupOpenGL(1680,1050, OF_GAME_MODE);			// <-------- setup the GL context

	ofRunApp(new ofApp());

}
