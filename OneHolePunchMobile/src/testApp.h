#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxOpenCv.h" // Docs warn to include first?
#include "circleTrack.h"

class testApp : public ofxiPhoneApp{
	
	public:
		void setup();
		void update();
		void draw();
		void exit();	
		
		void touchDown(ofTouchEventArgs &touch);
		void touchMoved(ofTouchEventArgs &touch);
		void touchUp(ofTouchEventArgs &touch);
		void touchDoubleTap(ofTouchEventArgs &touch);
		void touchCancelled(ofTouchEventArgs &touch);

		void enableDebug();
		void disableDebug();

	private:
		// Camera
		ofVideoGrabber grabber;	
		ofxCvGrayscaleImage grayCv;
		
		static const int cameraWidth = 360;
		static const int cameraHeight = 480;	
		static const int cvOriginalWidth = 320;
		static const int cvOriginalHeight = 480;
		
		float cvImageScaleFactor;	
		int cvScaledWidth;
		int cvScaledHeight;	

		// Circles
		int currentTrackedCircleCount;
		void houghCircles(ofxCvGrayscaleImage sourceImg);
		void drawCircles();
		vector <CircleTrack> myCircles;
		CvMemStorage* storage;
		CvSeq* circles;
		IplImage* gray; 
		int circID;
		
		// Debug
		bool debug;
		void drawAccelArrow();
		ofImage arrow;
	
		// GUI parameters
		int blurAmount;
		double hueRes;
		double minDist;
		double param1;
		double param2;
		int minRadius;
		int maxRadius;
	
		// Sound
		ofSoundPlayer  popSound;
};
