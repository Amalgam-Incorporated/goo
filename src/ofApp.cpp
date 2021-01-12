#include "ofApp.h"

int IMGX =500;
int IMGY =500;
int dither_type = 0;

int x_r = 4;
int y_r = 4;

string filename = "images/face.png";

//--------------------------------------------------------------
void ofApp::setup(){

    res.allocate(IMGX, IMGY, OF_IMAGE_COLOR);

    res.load(filename);
    res.update();

    ofPixels pix;
    // ofLoadImage(pix, filename);

    ofPixelsRef pixels = res.getPixels();

    IMGX = res.getWidth();
    IMGY = res.getHeight();

}


//--------------------------------------------------------------
void ofApp::update(){

    res.getPixelsRef().resize(IMGX/x_r,IMGY/y_r, OF_INTERPOLATE_NEAREST_NEIGHBOR);
    res.update();

    dither.dither_ordered(res, res, 8);
    res.getPixelsRef().resize(IMGX,IMGY,OF_INTERPOLATE_NEAREST_NEIGHBOR);

    /*
    for( int y=0; y<IMGY; y++ )
        {
         for( int x=0; x<IMGX; x++ )
         {
			int p = y * w + x;

			int oldPx = imageOutPixels[ p ];
			int newPx = oldPx < 127 ? 0 : 255;
			imageOutPixels[ p ] = newPx;


         }
         }

    */

    res.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    res.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == '0') {
        res.load(filename);
        res.update();
        dither_type = 0;

    } else if (key == '1') {
        res.load(filename);
        res.update();
        dither_type = 1;

    } else if (key == 's') {
        res.save("result_" + ofGetTimestampString() + ".png");

    }

    res.load(filename);

    if(key == OF_KEY_UP) {
        y_r++;
    } else if (key == OF_KEY_DOWN) {
        y_r--;
    } else if (key == OF_KEY_LEFT) {
        x_r--;
    } else if (key == OF_KEY_RIGHT) {
        x_r++;
    }

    x_r = ofClamp(x_r, 1, 100);
    y_r = ofClamp(y_r, 1, 100);
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
