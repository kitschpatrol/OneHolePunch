#include "testApp.h"

void testApp::setup(){
	ofSetFrameRate(60);
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);
	ofEnableSmoothing();
	ofSetCircleResolution(30);
	
	// set up video
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(320,240);
	
	colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	
	CvSeq* circles = new CvSeq;

	circID = 0;
}

void testApp::update(){	
	vidGrabber.grabFrame();
	
	if (vidGrabber.isFrameNew()) {
		colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
		grayImage = colorImg;
		houghCircles(grayImage);	
	}
}


void testApp::draw(){
	ofSetHexColor(0xffffff);
	grayImage.draw(0,0);

	// draw circles
	// cout << "total circles: " << circles->total << endl;

	for (int i = 0; i < circles->total; i++) {
		float* p = (float*)cvGetSeqElem( circles, i );
		ofPoint pos;
		pos.x = cvPoint(cvRound(p[0]),cvRound(p[1])).x;  
		pos.y = cvPoint(cvRound(p[0]),cvRound(p[1])).y;
		pos.x = (int)pos.x;
		pos.y = (int)pos.y;
		float radius = cvRound(p[2]);
		
		lerpPosX = ofLerp(lerpPosX, pos.x, 0.4);
		lerpPosY = ofLerp(lerpPosY, pos.y, 0.4);
		lerpRad = ofLerp(lerpRad, radius, 0.08);        
		
		ofSetColor( 255, 0, 0 );
		ofPushMatrix();
		ofTranslate(0, 0);
		ofCircle( pos.x, pos.y, lerpRad );
		ofPopMatrix();
	}

	// finally, a report:
	ofSetHexColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "threshold %i (press: +/-)\nnum circs found %i, fps: %f", threshold, circles->total, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);
	drawCircles();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

}



//--------------------------------------------------------------

void testApp::houghCircles( ofxCvGrayscaleImage sourceImg) {
	IplImage* gray = sourceImg.getCvImage();	
	cvSmooth( gray, gray, CV_GAUSSIAN, 5, 5 ); // smooth it, otherwise a lot of false circles may be detected
	CvMemStorage* storage = cvCreateMemStorage(0);	
	circles = cvHoughCircles(gray, storage, CV_HOUGH_GRADIENT, 2, gray->width/8, 300, 200 );
	
	// find positions of CV circles
	
	for (int i = 0; i < circles->total; i++) 
	{
		float* p = (float*)cvGetSeqElem( circles, i );
		ofPoint pos;
		pos.x = cvPoint( cvRound(p[0]),cvRound(p[1]) ).x;  
		pos.y = cvPoint( cvRound(p[0]),cvRound(p[1]) ).y;
		pos.x = (int)pos.x;
		pos.y = (int)pos.y;
		int radius = cvRound( p[2] );
		
		bool cFound = false;
		
		for (int circs = 0; circs < myCircles.size(); circs++) 
		{
			ofPoint& posCirc = myCircles[circs].pos;
			float dist = ofDistSquared(pos.x, pos.y, posCirc.x, posCirc.y);
			//cout << "distance is: " << dist << endl;
			
			// check to see if there is a circle near an existing tracked circle
			if ( dist < 1000 ) 
			{
				myCircles[circs].lastSeen = ofGetFrameNum();
				myCircles[circs].radius = radius;
				myCircles[circs].pos = pos;
				myCircles[circs].isAlive += 1;
				cFound = true;
			}
		}
		
		if (!cFound) 
		{
			CircleTrack c;
			c.pos = pos;
			c.radius = radius;
			c.lastSeen = ofGetFrameNum();
			c.isAlive = 0;
			c.drawMe = false;
			c.isSetUp = false;
			c.iD = circID;
			
			if (c.radius) {
				myCircles.push_back(c);
				circID++;
			}
		}
	}	
	
	grayDiffTemp = gray;
	cvReleaseMemStorage(&storage);
	
	for (int x = 0; x<myCircles.size(); x++) {
		bool isSetupNow = myCircles[x].isSetUp;
		if (!isSetupNow) {
			myCircles[x].setup();
		}
	}
	

	vector<CircleTrack>::iterator iter = myCircles.begin();
	while (iter != myCircles.end()) {
		
		double life = iter->lastSeen;
		int isAlive = iter->isAlive;
		
		if ((ofGetFrameNum()-life) > 30 ) {
			
			int iD = iter->iD;
			ofPoint tracePos = iter->pos;
			int	radiusT = iter->radius;
			
			iter = myCircles.erase(iter);
			cout << "shape deleted at: "<< tracePos.x << ", " << tracePos.y << endl;
			
			
		} else {
			
			if (iter->isAlive > 3) {
				iter->drawMe = true;
			} else {
				iter->drawMe = false;
			}
			iter++;
		}
	}
}


//--------------------------------------------------------------
void testApp::drawCircles(){
	ofPushStyle();
	for (int i=0; i<myCircles.size(); i++) {
		myCircles[i].debug = true;
		myCircles[i].draw();   // draw the particle
	}
	ofPopStyle();
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	panel.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	panel.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	panel.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}
