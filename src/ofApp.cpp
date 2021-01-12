#include "ofApp.h"

int IMGX =1000;
int IMGY =1000;
int dither_type = 0;

int x_r = 2;
int y_r = 2;

int frame = 0;

string filename = "images/face.png";

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);

    res.allocate(IMGX, IMGY, OF_IMAGE_COLOR);

    res.load(filename);
    res.update();

    IMGX = res.getWidth();
    IMGY = res.getHeight();

    res.getPixelsRef().resize(IMGX/x_r,IMGY/y_r, OF_INTERPOLATE_NEAREST_NEIGHBOR);
    res.getPixelsRef().resize(IMGX/x_r,IMGY/y_r, OF_INTERPOLATE_NEAREST_NEIGHBOR);
    res.update();

    dither.dither_ordered(res, res, 8);

    res.getPixelsRef().resize(IMGX,IMGY,OF_INTERPOLATE_NEAREST_NEIGHBOR);

    res.update();

}


//--------------------------------------------------------------
void ofApp::update(){

    ofPixels & pixels = res.getPixels();

    int w = res.getWidth();
    int h = res.getHeight();

    for(int y=h-1; y>0; y--) {
        for(int x=0; x<w; x++) {
            if (frame > 8 * int(30 * x/w))
            {
                int i = y * w + x;
                int m = 1;
                if (frame % 2)
                {
                    m = 1+(7*y/h + 2+2*sin(TWO_PI * x/(w/20)));
                }
                int j = (y-m) * w + x;
                if (j>0)
                {
                    pixels[i] = pixels[j];
                }
            }
        }
    }

    for(int x=0; x<w; x++) {
        pixels[x] = 0;
        pixels[x+w] = 0;
    }

    res.update();

    frame++;

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
