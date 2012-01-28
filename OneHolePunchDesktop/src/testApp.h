#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "cv.h"
#include <math.h>
#include "circleTrack.h"
#include "particle.h"



using namespace std;

#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

        

    
        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
		#else
		  ofVideoPlayer 		vidPlayer;
		#endif

        ofxCvColorImage			colorImg;
        ofxCvColorImage			colorRoiImg;

        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;
        ofxCvGrayscaleImage 	grayDiffTemp;		// for median filtering....

        ofxCvContourFinder 	contourFinder;
    
        ofTexture           tex;
        ofPixels            pix;

		int 				threshold;
        int                 circCount;
		bool				bLearnBakground;

        
        int 				camWidth;
        int 				camHeight;
        int                 destHeight;
        int                 destWidth;
        
    
        /////////// circles ////////
        
        void houghCircles( ofxCvGrayscaleImage sourceImg );		/// calculate circle detection
        void drawCircles();
            
        vector <CircleTrack> myCircles;
        vector <particle> punched;

        CvMemStorage* storage;
        CvSeq* circles;
        IplImage* gray; 
    
        int circID;

        ofPoint previousPos;
        float previousRadius;
        float lerpPosX;
        float lerpPosY;    
        float lerpRad;

};

