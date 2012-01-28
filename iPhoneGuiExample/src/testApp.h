#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

class testApp : public ofxiPhoneApp {
	
public:
	
	float lengthRatio;
	int numPoints;
	bool bFill;
    
    
    /////////
    
    double hueRes;
    double minDist;
	double param1;
    double param2;
    int minRadius;
    int maxRadius;
    
    float lerpRad;
	float lerpPos;
    
    //////////
    
	void setup();
	void update();
	void draw();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	void touchCancelled(ofTouchEventArgs &touch);

};


