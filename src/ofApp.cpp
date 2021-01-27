#include "ofApp.h"

int IMGX =1000;
int IMGY =1000;

int bands = 20;

int framerate = 60;

int dither_type = 0;

int x_r = 2;
int y_r = 2;

enum {
    PAUSE,
    REQUEST_TICKET,
    WAIT_FOR_TICKET,
    WAIT_FOR_FACE,
    MELT_FACE } state = REQUEST_TICKET;

int frame = 0;

string qr_filename = "images/qr.png";
string face_filename = "images/face.png";

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(framerate);

    res.allocate(IMGX, IMGY, OF_IMAGE_COLOR);

    verdana14.load("verdana.ttf", 14, true, true);

    ofRegisterURLNotification(this);
}

void ofApp::urlResponse(ofHttpResponse &httpResponse){
    printf("httpResponse.status: %i\n", httpResponse.status);
    printf("httpResponse.data: %s\n", httpResponse.data);
    if (state == WAIT_FOR_TICKET) {
        state = WAIT_FOR_FACE;
    }
  // if(httpResponse.request.getID() == loadXmlId && httpResponse.status==200 ){  // i.e is it ok
  //}
}


//--------------------------------------------------------------
//

bool ofApp::request_ticket(){

    ofHttpRequest request;
    request.method = ofHttpRequest::GET;
    request.url = "http://localhost:8087/api/ticket/next";
    string payload = "{ "
        "}";

    request.body = payload;

    ofURLFileLoader http;
    auto response = http.handleRequest(request);
    ofLogNotice() << response.status << std::endl;
    ofLogNotice() << response.data.getText() << std::endl;

    return true;

}

bool ofApp::wait_for_qr(){

    if (res.load(qr_filename))
    {
        return true;
    }
    return false;
}

bool ofApp::show_qr(){


    res.update();
    return true;
}

bool ofApp::wait_for_face(){

    ofImageLoadSettings settings;
    settings.exifRotate=true;

    if (res2.load(face_filename, settings))
    {
        res = res2;
        res.update();

        res.getPixelsRef().resize(IMGX/x_r,IMGY/y_r, OF_INTERPOLATE_NEAREST_NEIGHBOR);
        res.getPixelsRef().resize(IMGX/x_r,IMGY/y_r, OF_INTERPOLATE_NEAREST_NEIGHBOR);
        res.update();

        dither.dither_ordered(res, res, 8);

        res.getPixelsRef().resize(IMGX,IMGY,OF_INTERPOLATE_NEAREST_NEIGHBOR);

        res.update();

        frame=0;

        return true;
    }
    else
    {
        return false;
    }
}


void ofApp::band_control(int band, const char* msg){

    printf("band %i %s\n", band, msg);

    ofHttpRequest request;

    request.method = ofHttpRequest::POST;
    request.url = "http://localhost:8087/api/rock/";
    string payload = "{ "
        "\"band\": " + to_string(band) + ", "
        "\"msg\": \""+msg+"\""
        "}";

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
            return true;
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

    return false;

}

void ofApp::update(){

    switch (state){

        case REQUEST_TICKET:
        // wait for qr code image
        if (request_ticket()){
            state=WAIT_FOR_TICKET;
        }
        break;

        case WAIT_FOR_TICKET:
        // show qr code image
        if (show_qr()){
            state=WAIT_FOR_FACE;
        }
        break;

        case WAIT_FOR_FACE:
        // wait for face image
        if (wait_for_face()){
            state=MELT_FACE;
        }
        break;

        case MELT_FACE:
        // show and melt face image, on/off gravel
        if (show_face_melting()){
            state=REQUEST_TICKET;
        }
        break;

    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    if (state == WAIT_FOR_TICKET) {
        res.draw(200, 50);
        verdana14.drawString("Your banner ad here.", 300, 600);
    }
    // res.draw(0, 0);
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
                state=PAUSE;
            }
            else {
                state =MELT_FACE;
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
