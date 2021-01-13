#include "ofApp.h"

int IMGX =1000;
int IMGY =1000;
int dither_type = 0;

int x_r = 2;
int y_r = 2;

int state = 3;
int frame = 0;

string qr_filename = "images/face.png";
string face_filename = "images/face.png";

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);

    res.allocate(IMGX, IMGY, OF_IMAGE_COLOR);

    res.load(face_filename);
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
//

void ofApp::wait_for_qr(){
}

void ofApp::show_qr(){
}

void ofApp::wait_for_face(){
}

void ofApp::show_face_melting(){

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
        pixels[x] = 180;
        pixels[x+w] = 180;
    }

    res.update();

    frame++;


}

void ofApp::update(){


    switch (state){

        case 0:
        // wait for qr code image
        wait_for_qr();
        state=1;
        break;

        case 1:
        // show qr code image
        show_qr();
        state=2;
        break;

        case 2:
        // wait for face image
        wait_for_face();
        state=3;
        break;

        case 3:
        // show and melt face image, on/off gravel
        show_face_melting();
        state=0;
        break;

    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    res.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == '0') {
        res.load(face_filename);
        res.update();
        dither_type = 0;

    } else if (key == '1') {
        res.load(face_filename);
        res.update();
        dither_type = 1;

    } else if (key == 's') {
        res.save("result_" + ofGetTimestampString() + ".png");

    }

    res.load(face_filename);

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
