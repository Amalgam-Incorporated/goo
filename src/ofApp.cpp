#include "ofApp.h"

int IMGX =1000;
int IMGY =1000;

int bands = 20;

int framerate = 60;

int dither_type = 0;

int x_r = 2;
int y_r = 2;

int state = 0;
int frame = 0;

string qr_filename = "images/qr.png";
string face_filename = "images/face.png";

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(framerate);

    res.allocate(IMGX, IMGY, OF_IMAGE_COLOR);
}


//--------------------------------------------------------------
//

bool ofApp::wait_for_qr(){
    if (qr.load(qr_filename))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool ofApp::show_qr(){
    return 1;
}

bool ofApp::wait_for_face(){

    if (res.load(face_filename))
    {
        res.update();

        res.getPixelsRef().resize(IMGX/x_r,IMGY/y_r, OF_INTERPOLATE_NEAREST_NEIGHBOR);
        res.getPixelsRef().resize(IMGX/x_r,IMGY/y_r, OF_INTERPOLATE_NEAREST_NEIGHBOR);
        res.update();

        dither.dither_ordered(res, res, 8);

        res.getPixelsRef().resize(IMGX,IMGY,OF_INTERPOLATE_NEAREST_NEIGHBOR);

        res.update();

        frame=0;

        return 1;
    }
    else
    {
        return 0;
    }
}



bool ofApp::show_face_melting(){
    // show the face, start the melt, turn on the gravel
    // check for end of face, turn off the gavel

    ofPixels & pixels = res.getPixels();

    int band = min(bands-1, int(bands * frame*5/IMGX ));

    if (frame*5== int(band * IMGX/bands)) {
        printf("band %i on\n", band);
    }

    for(int x=0; x<(min((band+1)*(IMGX/bands),IMGX)); x++) {

        int m1 = 1+ int( 4* sin( PI * (x % int(IMGX/bands))/int(IMGX/bands)));

        for(int y=IMGY-1; y>0; y--) {

            int i = y * IMGX + x;
            int m2 = 1;
            if (frame % 2)
            {
                m2 = 1+(7*y/IMGY + m1);
            }
            int j = (y-m2) * IMGX + x;
            if (j>0)
            {
                pixels[i] = pixels[j];
            }
        }
    }

    for(int x=0; x<IMGX; x++) {
        pixels[x] = 180;
        pixels[x+IMGX] = 180;
    }

    res.update();

    frame++;

    return 0;

}

void ofApp::update(){


    switch (state){

        case 0:
        // wait for qr code image
        if (wait_for_qr()){
            state=1;
        }
        break;

        case 1:
        // show qr code image
        if (show_qr()){
            state=2;
        }
        break;

        case 2:
        // wait for face image
        if (wait_for_face()){
            state=3;
        }
        break;

        case 3:
        // show and melt face image, on/off gravel
        if (show_face_melting()){
            state=0;
        }
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
