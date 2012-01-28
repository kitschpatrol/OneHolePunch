#pragma once

#include "ofmain.h"

class CircleTrack {
	public:
		CircleTrack(ofPoint _pos);    
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
	
		int alpha;
};
