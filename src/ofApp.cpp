#include "ofApp.h"

int IMGX =2160;
int IMGY =2160;

int qr_x=1100;
int qr_y=250;

int bands = 20;
int framerate = 60;
int melt_time = 10;

int total_frames = framerate*melt_time;

int dither_type = 0;

int x_r = 2;
int y_r = 2;

enum {
    PAUSE,
    REQUEST_TICKET,
    WAIT_FOR_TICKET,
    WAIT_FOR_QR,
    SHOW_QR,
    WAIT_FOR_FACE,
    MELT_FACE } state = REQUEST_TICKET;

int frame = 0;

string qr_filename = "images/qr.png";
string bg_filename = "images/bg.png";

string face_filename = "images/selfie";

string ticket_no;

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(framerate);

    res.allocate(IMGX, IMGY, OF_IMAGE_COLOR);

    verdana14.load("verdana.ttf", 14, true, true);

    bg.load(qr_filename);

}

//--------------------------------------------------------------
//

bool ofApp::request_ticket(){

    ofHttpRequest request;
    request.method = ofHttpRequest::POST;
    request.url = "http://localhost/of/nextTicket";
    string payload = "{ "
        "}";

    request.body = payload;

    ofURLFileLoader http;
    auto response = http.handleRequest(request);

    ofLogNotice() << response.status << std::endl;
    ofLogNotice() << response.data.getText() << std::endl;

    ticket_no = response.data.getText();

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
    // Each servo has an endpoint of the format: [root]/hardware/servos/[servoNumber]/[command]

    printf("band %i %s\n", band, msg);

    ofHttpRequest request;

    request.method = ofHttpRequest::POST;
    request.url = "http://localhost/hardware/servos/" +
        to_string(band) + "/" + msg;
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

    // band in motion.  goes from 0 to 40 (2x bands count)
    // capped at bands count-1, so 0 to 19
    // int band = min(bands-1, int(bands * frame*5/IMGX ));
    // 60fps x 10s = 600 frames.
    // 600 frames / 20 bands = 30 frames per band
    int band = int(frame/(total_frames/bands));
    if  (frame*bands==band*total_frames) {
        // If we have transitioned from one band to another
        if (( 0 <= band ) && (band <= bands-1)) {
            // if the band number is in the open range
            band_control(band, "open");
        }
        if (( 0 <= band-8) && (band-8 <= bands-1)) {
            // if the band number is in the close range
            band_control(band-8, "close");
        }
    }

    for(int x=0; x<(min((band+1)*(IMGX/bands),IMGX)); x++) {

        int m1 = 1+ int( 8* sin( PI * (x % int(IMGX/bands))/int(IMGX/bands)));

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
        pixels[x] = 255;
        pixels[x+IMGX] = 255;
    }

    res.update();

    frame++;

    return false;

}

void ofApp::update(){

    switch (state){

        case REQUEST_TICKET:
        //
        if (request_ticket()){
            // state=WAIT_FOR_TICKET;
            state=WAIT_FOR_QR;
        }
        break;

        case WAIT_FOR_QR:
        // show qr code image
        if (wait_for_qr()){
            state=SHOW_QR;
        }
        break;

        case SHOW_QR:
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

    switch (state){

        case SHOW_QR:
        case WAIT_FOR_FACE:
            // res.draw(1200, 250);
            res.draw(qr_x, qr_y);
            // verdana14.drawString("Your banner ad here.", 300, 600);
            verdana14.drawString(ticket_no, 1000, 1650);
        break;

        case MELT_FACE:
            res.draw(0, 0);
        break;
    }
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
            if (state==MELT_FACE){
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

		case 'i':
            qr_x--;
            printf("%i",qr_x);
            break;
		case 'j':
            qr_y--;
            break;
		case 'k':
            qr_y++;
            break;
		case 'm':
            qr_x++;
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
