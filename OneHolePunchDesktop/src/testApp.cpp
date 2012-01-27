#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
	#else
        vidPlayer.loadMovie("Movie_2.mov");
        vidPlayer.play();
	#endif

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);
    grayDiffTemp.allocate(320,240);


	bLearnBakground = true;
	threshold = 80;
    CvSeq* circles = new CvSeq;

    ofSetFrameRate(60);

    
	///// cpanel ////
	
	panel.setup("control", ofGetWidth()-250, 0, 250, ofGetHeight());
	
	panel.addPanel("image processing", 1, false);
	panel.setWhichPanel("image processing");	
	panel.addSlider("theshold", "THRESHOLD", 80,0,255, true);	
	panel.loadSettings("settings.xml");
	
	/// some graphic settings for blending etc
	
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);
	ofEnableSmoothing();
	ofSetCircleResolution(30);
	

    
    
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
        houghCircles(grayImage);


	}

}

//--------------------------------------------------------------
void testApp::draw(){

    
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(0,0);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);

	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
	ofRect(360,540,320,240);
	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(360,540);

	// or, instead we can draw each blob individually,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++)
    {
        contourFinder.blobs[i].draw(360,540);
    }
    

    // cout << "total circles: " << circles->total << endl;
    // draw bloody circles 
    
    for (int i = 0; i < circles->total; i++) 
	{
		float* p = (float*)cvGetSeqElem( circles, i );
		ofPoint pos;
		pos.x = cvPoint(cvRound(p[0]),cvRound(p[1])).x;  
		pos.y = cvPoint(cvRound(p[0]),cvRound(p[1])).y;
		pos.x = (int)pos.x;
		pos.y = (int)pos.y;
		float radius = cvRound(p[2]);
         
        lerpPosX = ofLerp(lerpPosX, pos.x, 0.1);
        lerpPosY = ofLerp(lerpPosY, pos.y, 0.1);
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

    panel.draw();
    drawCircles();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}
}



//--------------------------------------------------------------

void testApp::houghCircles( ofxCvGrayscaleImage sourceImg ) 
{
    IplImage* gray = sourceImg.getCvImage();	
	cvSmooth( gray, gray, CV_GAUSSIAN, 5, 5 ); // smooth it, otherwise a lot of false circles may be detected
	CvMemStorage* storage = cvCreateMemStorage(0);	
	circles = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2, gray->width/8, 300, 200 );
    
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
			cout << "created trace at: "<< tracePos.x << ", " << tracePos.y << endl;
			
			
			
			
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
