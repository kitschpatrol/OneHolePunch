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
    
        CircleTrack ( ofPoint _pos );
    
        void setup();
        void update();
        void draw();
        
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

	
};

	


	
	
	