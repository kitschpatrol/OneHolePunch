#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	ofSetFrameRate(30);	
	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);
	ofSetVerticalSync(true);
	
	grabber.setDesiredFrameRate(30);
	
	cout << "OF Width: " << ofGetWidth() << endl;
	cout << "OF Height: " << ofGetHeight() << endl;	
	
	// Camera view is wider than screen!!!
	grabber.initGrabber(360, 480, true); // Init sizes seem to do nothing at all, this is native iphone live feed res.

	// this is screen aspect ratio-consistent crop from grabber
	// there will be gutters!
	float cvImageScaleFactor = 1; // divide by this for CV, use power of two // TODO this is broken!	
	grayCv.allocate(320 / cvImageScaleFactor, 480 / cvImageScaleFactor);
	
	
	CvSeq* circles = new CvSeq;
	
	circID = 0;	
	
	drawDebug = true;
}

//--------------------------------------------------------------
void testApp::update(){

	
	grabber.update();
	
	if (grabber.isFrameNew()) {
		
		// crop out the color pixels we want
		
		
		
		unsigned char * cameraPixels = grabber.getPixels();
		unsigned char * grayPixels = grayCv.getPixels();
	
		// aspect-correct workable size is 320 x 480
		// camera feed is 360 x 480
		// TODO resize?
		
		for (int x = 0; x < 320; x++){
			for (int y = 0; y < 480; y++){
				int cameraPos = (y * 360 + (x + 20)) * 3; // camera pix have 20px x offset for gutter
				int grayPos = y * 320 + x;	
				
				// Use green channel for grayscale trasi
				// grayPixels[grayPos] = cameraPixels[cameraPos];  // R
				grayPixels[grayPos] = cameraPixels[cameraPos + 1]; // G
				// grayPixels[grayPos] = cameraPixels[cameraPos + 2]; // B
			}
		}
		
		grayCv.setFromPixels(grayPixels, 320, 480);

//		houghCircles(grayCv);
	}
	
	//	colorCvSmall.scaleIntoMe(colorCv, CV_INTER_NN);
	// grayCv = colorCvSmall;
}

//--------------------------------------------------------------
void testApp::draw(){	
	ofBackground(255, 0, 0); // See any gaps easily
	ofSetColor(255);
	
	if (drawDebug) {
		grayCv.draw(0, 0, 720, 960);
	}
	else {
		grabber.draw(-40, 0, 720, 960);
	}
	


	//drawCircles();	
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
	
	
	//cout << "Number of Circles: " << circles->total << endl;	
	
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



