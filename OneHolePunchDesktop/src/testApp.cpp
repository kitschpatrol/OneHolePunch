#include "testApp.h"

void testApp::setup()
{
	ofSetFrameRate(60);
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);
	ofEnableSmoothing();
	ofSetCircleResolution(30);
    ofBackground(0, 0, 0);	

    camWidth = 320;	// try to grab at this size. 
	camHeight = 240;
	
    destHeight = 50;
    destWidth = 50;
    
	// set up video
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(320,240);
	
	colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	
	CvSeq* circles = new CvSeq;
    tex.allocate( destWidth,destHeight, GL_RGB );

	circID = 0;
    circCount = 0;
}

void testApp::update(){	
	vidGrabber.grabFrame();
	
	if (vidGrabber.isFrameNew()) {
        
        ofPixels  videoTexture;
		colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
		grayImage = colorImg;
		houghCircles(grayImage);	
        pix = vidGrabber.getPixelsRef();
        
        for ( int i=0 ; i<punched.size(); i++) {
            punched[i].update();
            punched[i].goToTarget( ofVec3f( mouseX,mouseY,0), 100);
            
            for ( int j=0 ; j<punched.size(); j++) 
            {
                if ( j != i ) punched[i].runFromTarget( punched[j].pos,1 );
            }
        }
	}
}


void testApp::draw()
{
    ofSetHexColor(0xffffff);
	grayImage.draw(0,0);

	// finally, a report:
	ofSetHexColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "threshold %i (press: +/-)\nnum circs found %i, fps: %f", threshold, circCount,ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);
	drawCircles();
    //tex.draw( 400,400,destWidth,destHeight);
    
    for ( int i=0 ; i<punched.size(); i++) {
        punched[i].draw();
    }
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
	circCount = circles->total;
    
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
	
	grayDiffTemp = gray;
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
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
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
