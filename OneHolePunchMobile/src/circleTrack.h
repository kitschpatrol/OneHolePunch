#pragma once
#include "ofmain.h"


class CircleTrack {

	public:
		ofPoint pos;
		ofPoint acc;
		ofPoint vel;
		int iD;
		int radius;
		int isAlive;
		long lastSeen;
		bool drawMe;
		bool isSetUp;
		bool debug;
    
        float lerpPosX;
        float lerpPosY;    
        float lerpRad;

	
	void setup () 
    {
		isSetUp = false;
		
		if (!isSetUp) 
        {
			isSetUp = true;
			debug = false;
		}
	}
	
 
    
	
	void draw () 
    {
		int yLoc = (int)2*radius;
		int xLoc = (int)(2*radius * (4/3));
		
        lerpPosX = ofLerp(lerpPosX, pos.x, 0.1);
        lerpPosY = ofLerp(lerpPosY, pos.y, 0.1);
        lerpRad = ofLerp(lerpRad, radius, 0.08);
        
        
		if (drawMe) 
        {
			ofPushMatrix();
			ofTranslate(lerpPosX, lerpPosY, 0);						
			
			if (true) {
				ofPushStyle();
				ofNoFill();
				ofSetColor(0, 255, 0,255);
				ofCircle(0, 0, lerpRad);
				ofCircle(0, 0, 3);
				ofFill();
				char reportCirc[1024];
				ofSetColor(255, 0, 0);
				sprintf(reportCirc, " Id: %i \n r: %i \n pos: %i, %i", iD, radius, (int)pos.x, (int)pos.y );			
				ofDrawBitmapString(reportCirc, 0, radius);
				ofPopStyle();
			}
            
			ofPopMatrix();
		}
	} 
};

	


	
	
	