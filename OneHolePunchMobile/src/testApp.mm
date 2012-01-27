#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);

	ofSetFrameRate(30);
	grabber.setDesiredFrameRate(30);
	grabber.initGrabber(480, 360);
	
	colorCv.allocate(grabber.getWidth(), grabber.getHeight());
	grayCv.allocate(grabber.getWidth(), grabber.getHeight());
	
	CvSeq* circles = new CvSeq;
	
	circID = 0;	
	
	gotFirstCameraFrame = false;
	
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(0);
	
	grabber.update();
	
	if (grabber.isFrameNew()) {
		gotFirstCameraFrame = true;
		colorCv = grabber.getPixels();
		grayCv = colorCv;
		houghCircles(grayCv);
	}
	
	//	colorCvSmall.scaleIntoMe(colorCv, CV_INTER_NN);
	// grayCv = colorCvSmall;
}

//--------------------------------------------------------------
void testApp::draw(){	
	
	ofSetColor(255);
	
	//grabber.draw(0, 0);

	if (gotFirstCameraFrame) {
	
		//if ( circles != NULL ) cout << "Circles " << circles->total << endl;
		
	grayCv.draw(0,0);
	drawCircles();	

//		for (int i = 0; i < circles->total; i++) {
//			float* p = (float*)cvGetSeqElem( circles, i );
//			ofPoint pos;
//			pos.x = cvPoint(cvRound(p[0]),cvRound(p[1])).x;  
//			pos.y = cvPoint(cvRound(p[0]),cvRound(p[1])).y;
//			pos.x = (int)pos.x;
//			pos.y = (int)pos.y;
//			float radius = cvRound(p[2]);
//			
//			lerpPosX = ofLerp(lerpPosX, pos.x, 0.4);
//			lerpPosY = ofLerp(lerpPosY, pos.y, 0.4);
//			lerpRad = ofLerp(lerpRad, radius, 0.08);        
//			
//			ofSetColor( 255, 0, 0 );
//			ofPushMatrix();
//			ofTranslate(0, 0);
//			ofCircle( pos.x, pos.y, lerpRad );
//			ofPopMatrix();
//		}	
	}
	
	ofEnableAlphaBlending();
	ofSetColor(230, 0, 255, 200);
	ofRect(0, 0, ofGetWidth(), 16);
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("OneHolePunch", 5, 12);
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}



void testApp::houghCircles( ofxCvGrayscaleImage sourceImg) {
	
	IplImage* gray = sourceImg.getCvImage();	
	cvSmooth( gray, gray, CV_GAUSSIAN, 5, 5 ); // smooth it, otherwise a lot of false circles may be detected
	CvMemStorage* storage = cvCreateMemStorage(0);	
	circles = cvHoughCircles(gray, storage, CV_HOUGH_GRADIENT, 2, gray->width/8, 300, 200 );
	
	
	cout << "Circles: " << circles << endl;
	cout << "Circles Total: " << circles->total << endl;	
	
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



