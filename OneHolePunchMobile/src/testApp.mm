#include "testApp.h"
#include "GuiView.h"

GuiView * guiViewController;

void testApp::setup(){	
	ofSetFrameRate(30);
	ofEnableAlphaBlending();	
	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);
	ofSetVerticalSync(true); // Does this actually do anything?
	ofRegisterTouchEvents(this);
	//ofxiPhoneAlerts.addListener(this);	
	ofxAccelerometer.setup();
	ofSetCircleResolution(30);	
	
	cout << "OF Width: " << ofGetWidth() << endl;
	cout << "OF Height: " << ofGetHeight() << endl;	
	
	// Camera view is wider than screen!!!
	grabber.setDesiredFrameRate(30);	
	grabber.initGrabber(cameraWidth, cameraHeight, true); // Init sizes seem to do nothing at all, this is native iphone live feed res.

	// this is screen aspect ratio-consistent crop from grabber
	// there will be gutters!

	// set up cv image, this is where we search for circles
	cvImageScaleFactor = 2; // divide by this for CV, use power of two // do this as a param to CV instead?
	cvScaledWidth = cvOriginalWidth / cvImageScaleFactor;
	cvScaledHeight = cvOriginalHeight / cvImageScaleFactor;	
	grayCv.allocate(cvScaledWidth, cvScaledHeight);
	
	cout << "cvScaledWidth: " << cvScaledWidth << endl;
	cout << "cvScaledHeight: " << cvScaledHeight << endl;

	// Initialize Variables
	CvSeq* circles = new CvSeq;
	circID = 0;
	debug = false;	
	currentTrackedCircleCount = 0;
	
	// Initial cv parameters (should match sliders)
	blurAmount = 5;
	hueRes = 1;
	minDist = 35;
	param1 = 2;
	param2 = 20;
	minRadius = 40;
	maxRadius = 300;	
	
	// Initialize Particles
//	particle::bounceFactor = 0.7;
//	particle::accelerometerForce = 0.2;	
	
	// Circle tracking
	tex.allocate(ofGetWidth(), ofGetHeight(), GL_RGB );	
	
	// Accelerometer Debug Arrow
	arrow.loadImage("arrow.png");
	arrow.setAnchorPercent(1.0, 0.5);	
	
	// Sounds, the .caf file is big-endian and doesn't work?
	popSound.loadSound("sounds/punched.wav");	
	popSound.setVolume(1);	
	popSound.setMultiPlay(true);
	
	// GUI
	guiViewController	= [[GuiView alloc] initWithNibName:@"GuiView" bundle:nil];
	[ofxiPhoneGetUIWindow() addSubview:guiViewController.view];
}


void testApp::update(){
	// Update GUI status text
	if (!guiViewController.view.hidden) {
		[guiViewController setFrameRateString: ofxStringToNSString(ofToString("FPS: " + ofToString(ofGetFrameRate(), 0)))];
		[guiViewController setCircleCountString: ofxStringToNSString(ofToString("Circles: " + ofToString(currentTrackedCircleCount)))];	
	}

	// Update camera
	grabber.update();
	
	if (grabber.isFrameNew()) {
		// crop out the pixels we want from the grabber
		// this is weird because aspect-correct workable size is 320 x 480
		// but camera feed is 360 x 480
		// so we grab from the center of the camera feed
		unsigned char * cameraPixels = grabber.getPixels();
		unsigned char * grayPixels = grayCv.getPixels();
		
		for (int x = 0; x < cvScaledWidth; x++){
			for (int y = 0; y < cvScaledHeight; y++){
				int cameraPos = ((y * cvImageScaleFactor) * cameraWidth + ((x * cvImageScaleFactor) + 20)) * 3; // camera pix have 20px x offset for gutter
				int grayPos = y * cvScaledWidth + x;	
		
				// go straight to grayscale... saves a CV step. Pick a channel here
				// grayPixels[grayPos] = cameraPixels[cameraPos];  // R
				grayPixels[grayPos] = cameraPixels[cameraPos + 1]; // G
				// grayPixels[grayPos] = cameraPixels[cameraPos + 2]; // B
			}
		}
		
		grayCv.setFromPixels(grayPixels, cvScaledWidth, cvScaledHeight);

		houghCircles(grayCv);
	}
	
	for (int i = 0; i < punched.size(); i++) {
		punched[i].update(ofxAccelerometer.getForce().x, ofxAccelerometer.getForce().y);
		//punched[i].mouse = ofVec3f(mouseX, mouseY, 0);
	}
	
	ofSoundUpdate();	
}


void testApp::draw(){	
	ofBackground(255, 0, 0); // See any gaps easily
	ofSetColor(255);
	
	grabber.draw(-40, 0, 720, 960);	
	
	if (debug) {
		grayCv.draw(0, 0);
		drawAccelArrow();		
	}

	drawCircles();	
	
	for (int i = 0; i < punched.size(); i++) {
		punched[i].draw();
	}	
}


void testApp::touchDown(ofTouchEventArgs &touch){
	// bring up gui TODO do this on double tap instead
	if (guiViewController.view.hidden ){
		guiViewController.view.hidden = NO;
	}
}


void testApp::houghCircles( ofxCvGrayscaleImage sourceImg) {
	IplImage* gray = sourceImg.getCvImage();	
	cvSmooth( gray, gray, CV_GAUSSIAN, blurAmount, blurAmount); // smooth it, otherwise a lot of false circles may be detected
	CvMemStorage* storage = cvCreateMemStorage(0);	
	//circles = cvHoughCircles(gray, storage, CV_HOUGH_GRADIENT, 2, gray->width/8, 300, 200 );
	circles = cvHoughCircles(gray, storage, CV_HOUGH_GRADIENT, param1, param2, minRadius, maxRadius);	
	currentTrackedCircleCount = circles->total;
	
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
			radius *=1.05;
			CircleTrack c = CircleTrack( pos );
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
				
				pix = grabber.getPixelsRef(); // do this here instead of draw?
				
				ofPixels pixCopy = pix;
				pixCopy.crop( pos.x-radius, (pos.y-radius), radius*2 , radius*2 );
				tex.loadData( pixCopy );
				
				ofTexture T;
				T.allocate(radius*2,radius*2, GL_RGB);
				T.loadData(pixCopy);
				particle P = particle( pos,circID,radius,T  );
				punched.push_back( P );
				printf("punched %f,%f\n", pos.x,pos.y);
				
			}
		}
	}	
	
	cvReleaseMemStorage(&storage);
	
	for ( int x = 0; x<myCircles.size(); x++ ) 
	{
		bool isSetupNow = myCircles[x].isSetUp;
		if ( !isSetupNow ) {
			myCircles[x].setup();
		}
	}
	
	
	vector<CircleTrack>::iterator iter = myCircles.begin();
	while (iter != myCircles.end()) {
		
		double life = iter->lastSeen;
		int isAlive = iter->isAlive;
    
		// kill old particles;
		if ( (ofGetFrameNum()-life) > 50 ) 
		{
			
			ofPoint tracePos = iter->pos;
			iter = myCircles.erase(iter);
			cout << "shape deleted at: "<< tracePos.x << ", " << tracePos.y << endl;
			
			
		} else {
			
			if (iter->isAlive > 0) // used to be 3? 
			{
				iter->drawMe = true;
			} else 
			{
				iter->drawMe = false;
			}
			iter++;
		}
	}
}


void testApp::drawCircles(){
	ofPushStyle();
	for (int i=0; i<myCircles.size(); i++) {
		myCircles[i].debug = true;
		myCircles[i].draw();   // draw the particle
	}
	ofPopStyle();
}

void testApp::enableDebug() {
	grayCv.setUseTexture(true);
	debug = true;
}

void testApp::disableDebug() {
	grayCv.setUseTexture(false);	
	debug = false;	
}

void testApp::drawAccelArrow() {
	float angle = 180 - RAD_TO_DEG * atan2( ofxAccelerometer.getForce().y, ofxAccelerometer.getForce().x );
	
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	ofScale(3, 3);	
	ofRotateZ(angle);
	arrow.draw(0,0);
	ofPopMatrix();	
}

void testApp::exit() {
	ofSoundStopAll();
	ofSoundShutdown();
}

void testApp::vibrate() {
	AudioServicesPlayAlertSound(kSystemSoundID_Vibrate);	
}

//--------------------------------------------------------------

void testApp::touchMoved(ofTouchEventArgs &touch){
	
}

void testApp::touchUp(ofTouchEventArgs &touch){
	
}

void testApp::touchDoubleTap(ofTouchEventArgs &touch){
	
}

void testApp::touchCancelled(ofTouchEventArgs& args){
	
}
