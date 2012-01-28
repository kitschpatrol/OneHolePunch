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
	
	minCircleRadius = 10;
	maxCircleRadius = 50;
	
	// Initialize Particles
	particle::bounceFactor = 0.2;
	particle::accelerometerForce = 3;	
	
	// Accelerometer Debug Arrow
	arrow.loadImage("arrow.png");
	arrow.setAnchorPercent(1.0, 0.5);	
	
	// Sounds, the .caf file is big-endian and doesn't work?
	popSound.loadSound("sounds/punched.wav");	
	popSound.setVolume(1);	
	popSound.setMultiPlay(true);
	
	circleMask.loadImage("images/circleMask.png");
	
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
		
				// go straight to grayscale via green channel... saves a CV step
				grayPixels[grayPos] = cameraPixels[cameraPos + 1]; // G
			}
		}
		
		grayCv.setFromPixels(grayPixels, cvScaledWidth, cvScaledHeight);

		houghCircles(grayCv);
	}
	
	for (int i = 0; i < punched.size(); i++) {
		punched[i].update(ofxAccelerometer.getForce().x, ofxAccelerometer.getForce().y);
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
		
		// transform to camera world
		pos.x = (int)cvPoint(cvRound(p[0]),cvRound(p[1])).x; // (int)ofMap(cvPoint(cvRound(p[0]),cvRound(p[1])).x, 0, cvScaledWidth, 0, cameraWidth - 80) + 40;  
		pos.y = (int)cvPoint(cvRound(p[0]),cvRound(p[1])).y; //(int)ofMap(cvPoint(cvRound(p[0]),cvRound(p[1])).y, 0, cvScaledHeight, 0, cameraHeight);
		float radius = cvRound(p[2]); //(int)ofMap(cvRound(p[2]), 0, cvScaledHeight, 0, cameraHeight);
		
		bool cFound = false;
		
		for (int circs = 0; circs < myCircles.size(); circs++) {
			ofPoint& posCirc = myCircles[circs].pos;
			float dist = ofDistSquared(pos.x, pos.y, posCirc.x, posCirc.y);
			//cout << "distance is: " << dist << endl;
			
			// check to see if there is a circle near an existing tracked circle
			if ( dist < 1000 ) {
				myCircles[circs].lastSeen = ofGetFrameNum();
				myCircles[circs].radius = radius;
				myCircles[circs].pos = pos;
				myCircles[circs].isAlive += 1;
				cFound = true;
			}
		}
		
		if (!cFound) {
			radius *= 1.1; // Grow 10%
			CircleTrack c = CircleTrack(pos);
			c.pos = pos;
			c.radius = radius;
			c.lastSeen = ofGetFrameNum();
			c.isAlive = 0;
			c.drawMe = false;
			c.isSetUp = false;
			c.iD = circID;
			
			if (c.radius > 0) {
				myCircles.push_back(c);
				circID++;
				
				// Type conversion overkill to debug loadData() error
				int cropX = ofMap(pos.x - radius, 0, cvScaledWidth, 20, cameraWidth - 20);
				int cropY = ofMap(pos.y - radius, 0, cvScaledHeight, 0, cameraHeight);
				int cropWidth = ofMap(radius * 2, 0, cvScaledWidth, 0, cameraWidth - 40); // shrink camera
				int cropHeight = ofMap(radius * 2, 0, cvScaledHeight, 0, cameraHeight);
				
				
				cout << "Cropx: " << cropX << endl;
				cout << "Cropy: " << cropY << endl;
				cout << "Cropw: " << cropWidth << endl;
				cout << "Croph: " << cropHeight << endl;	
				
				// Create the circle texture
				ofImage circleImage;
				circleImage.allocate(cropWidth, cropHeight, OF_IMAGE_COLOR_ALPHA);

				// Resize the mask to match circle
				ofImage scaledCircleMask;
				scaledCircleMask.setUseTexture(false);
				scaledCircleMask.clone(circleMask); // copy image
				scaledCircleMask.resize(cropWidth, cropHeight);
				
				unsigned char * cameraPixels = grabber.getPixels();
				unsigned char * circlePixels = circleImage.getPixels();
				unsigned char * maskPixels = scaledCircleMask.getPixels();
				
				for (int x = 0; x < cropWidth; x++){
					for (int y = 0; y < cropHeight; y++){
						int cameraPos = (((y + cropY) * cameraWidth) + (x + cropX)) * 3; // rgb
						int circlePos = ((y * cropWidth) + x) * 4; // rgba
						int maskPos = ((y * cropWidth) + x) * 3; // rgb (just a doesn't work!?)
						
						circlePixels[circlePos] = cameraPixels[cameraPos]; // R
					  circlePixels[circlePos + 1] = cameraPixels[cameraPos + 1]; // G
						circlePixels[circlePos + 2] = cameraPixels[cameraPos + 2]; // B				
						circlePixels[circlePos + 3] = maskPixels[maskPos]; // A
					}
				}
				
				scaledCircleMask.clear();

				circleImage.setFromPixels(circlePixels, cropWidth, cropHeight, OF_IMAGE_COLOR_ALPHA);
				
				particle circleParticle = particle(pos, circID, radius, circleImage);
				punched.push_back(circleParticle);

				// Play the sound... Bigger circles have lower pitch
				minCircleRadius = fminf((float)radius, minCircleRadius);
				maxCircleRadius = fmaxf((float)radius, maxCircleRadius);
				float playSpeed = ofMap(radius, minCircleRadius, maxCircleRadius, 3, 0.5);
				
				// Shake the phone
				vibrate();

				popSound.setSpeed(playSpeed); // TODO tweak this range
				popSound.play();
				
				printf("punched %f,%f with radius %f (min: %f and max: %f) and playback speed %f\n", pos.x,pos.y, (float)radius, minCircleRadius, maxCircleRadius, playSpeed);																	
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
		if ( (ofGetFrameNum()-life) > 15) {
			
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
