/*
 *  circleTrack.h
 *  lolWellFresh
 *
 *  Created by Tamar Ziv on 17/4/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */



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
		
		if (drawMe) 
        {
			ofPushMatrix();
			ofTranslate(pos.x, pos.y, 0);						
			
			if (true) {
				ofPushStyle();
				ofNoFill();
				ofSetColor(0, 255, 0,255);
				ofCircle(0, 0, radius);
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

	


	
	
	