#include "circleTrack.h"

CircleTrack::CircleTrack( ofPoint _pos ) {
	pos = _pos;   
}


void CircleTrack::setup () {
	isSetUp = false;
    
	if (!isSetUp) {
		isSetUp = true;
		debug = false;
	}
}


void CircleTrack::draw () {    
	lerpPosX = ofLerp(lerpPosX, pos.x, 0.1);
	lerpPosY = ofLerp(lerpPosY, pos.y, 0.1);
	lerpRad = ofLerp(lerpRad, radius, 0.08);
      
	if (drawMe) {
		ofPushMatrix();
		
		// Draw circle
		ofTranslate(lerpPosX, lerpPosY, 0);									
		ofPushStyle();
		ofSetColor(0, 0, 0,255);
		ofCircle(0, 0, lerpRad);
		ofCircle(0, 0, 3);
		ofFill();
		
		// Draw debug text
		char reportCirc[1024];
		ofSetColor(255, 0, 0);
		sprintf(reportCirc, " Id: %i \n r: %i \n pos: %i, %i\nlife: %i", iD, radius, (int)pos.x, (int)pos.y, isAlive );			
		ofDrawBitmapString(reportCirc, 0, radius);
		ofPopStyle();

		ofPopMatrix();
	}
}