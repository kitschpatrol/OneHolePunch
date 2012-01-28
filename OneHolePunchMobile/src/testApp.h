#pragma once


#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"


//ON IPHONE NOTE INCLUDE THIS BEFORE ANYTHING ELSE
#include "ofxOpenCv.h"

#include "circleTrack.h"

class testApp : public ofxiPhoneApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void touchDown(ofTouchEventArgs &touch);
		void touchMoved(ofTouchEventArgs &touch);
		void touchUp(ofTouchEventArgs &touch);
		void touchDoubleTap(ofTouchEventArgs &touch);
		void touchCancelled(ofTouchEventArgs &touch);


		ofVideoGrabber grabber;
		
		ofxCvColorImage colorCv;
	//		ofxCvColorImage colorCvSmall;
		
	
	
	ofxCvGrayscaleImage grayCv;

	
	static const int cameraWidth = 360;
	static const int cameraHeight = 480;	
	static const int cvOriginalWidth = 320;
	static const int cvOriginalHeight = 480;
	
	float cvImageScaleFactor;	
	int cvScaledWidth;
	int cvScaledHeight;
	
	// GUI
	bool drawDebug;
	
	
	ofImage colorView;

	
	
	/////////// circles ////////
	void houghCircles(ofxCvGrayscaleImage sourceImg );		/// calculate circle detection
	void drawCircles();
	
	vector <CircleTrack> myCircles;
	CvMemStorage* storage;
	CvSeq* circles;
	IplImage* gray; 
	int circID;
	ofPoint previousPos;
	float previousRadius;
	
	string fNameSaved;
	
	
	// GUI parameters
	double hueRes;
	double minDist;
	double param1;
	double param2;
	int minRadius;
	int maxRadius;
	float lerpRad;
	float lerpPos;	
	
		
};
