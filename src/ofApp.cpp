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

    ofRegisterURLNotification(this);
}

void ofApp::urlResponse(ofHttpResponse &httpResponse){
    printf("httpResponse.status: %i\n", httpResponse.status);
  // if(httpResponse.request.getID() == loadXmlId && httpResponse.status==200 ){  // i.e is it ok
  //}
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


void ofApp::band_control(int band, const char* msg){

    printf("band %i %s\n", band, msg);

    ofHttpRequest request;
    request.method = ofHttpRequest::POST;
    request.url = "http://localhost:8082/api/";
    string payload = "{ "
        "\"band\": " + to_string(band) + ", "
        "\"msg\": \""+msg+"\""
        "}";

    printf("payload: %s\n", payload.c_str());

    request.url = "http://localhost:8082/api/";
    request.body = payload;

    ofURLFileLoader http;
    auto response = http.handleRequest(request);
    ofLogNotice() << response.status << std::endl;
    ofLogNotice() << response.data.getText() << std::endl;

}

bool ofApp::show_face_melting(){
    // show the face, start the melt, turn on the gravel
    // check for end of face, turn off the gavel

    ofPixels & pixels = res.getPixels();

    int band = min(bands-1, int(bands * frame*5/IMGX ));
    if (frame*5== int(band * IMGX/bands)) {
        band_control(band, "on");
    }

    int band_off = min(bands-1, int(bands * (frame-framerate*5)*5/IMGX ));
    if (band_off>=0 and (frame-framerate*5)*5 == int(band_off * IMGX/bands)) {
        band_control(band_off, "off");
        if (band_off == 19){
            band_control(-1, "unlock");
            return 1;
        }
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

	switch (key) {

		case ' ':
			break;

		case 'f':
			ofToggleFullscreen();
			break;

		case 'p':
            if (state==3){
                state=4;
            }
            else {
                state =3;
            }
            return;
			break;

		case '0':
            res.load(face_filename);
            res.update();
            dither_type = 0;
			break;
		case '1':
            res.load(face_filename);
            res.update();
            dither_type = 1;
			break;

		case 's':
            res.save("result_" + ofGetTimestampString() + ".png");
            break;

		case OF_KEY_UP:
            y_r++;
            break;
		case OF_KEY_DOWN:
            y_r--;
            break;
		case OF_KEY_LEFT:
            x_r--;
            break;
		case OF_KEY_RIGHT:
            x_r++;
            break;
    }

    res.load(face_filename);

    x_r = ofClamp(x_r, 1, 100);
    y_r = ofClamp(y_r, 1, 100);

    res.getPixelsRef().resize(IMGX/x_r,IMGY/y_r, OF_INTERPOLATE_NEAREST_NEIGHBOR);
    res.getPixelsRef().resize(IMGX/x_r,IMGY/y_r, OF_INTERPOLATE_NEAREST_NEIGHBOR);
    res.update();

    dither.dither_ordered(res, res, 8);

    res.getPixelsRef().resize(IMGX,IMGY,OF_INTERPOLATE_NEAREST_NEIGHBOR);

    res.update();

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
