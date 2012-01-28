#include "particle.h"

float particle::bounceFactor = 0.2;
float particle::accelerometerForce = 0.8;	

particle::particle( ofVec3f _pos, int _id, float _size, ofImage _circleImage) {
	circleImage = _circleImage;
	pos = _pos;								// get birth place from pixels
	pId = _id;
	size = _size;
	
	vel = ofVec3f(0,0,0);
	acc = ofVec3f(0,0,0);
}


void particle::update(float accelX, float accelY) {	
	vel.x += particle::accelerometerForce * accelX * ofRandomuf();
	vel.y += -particle::accelerometerForce * accelY * ofRandomuf();
	
	// add vel to pos
	pos += vel;
	
	// check boundaries
	if (pos.x < size) {
		pos.x = size;
		vel.x *= -particle::bounceFactor;
	} else if(pos.x >= ofGetWidth() - size) {
		pos.x = ofGetWidth() - size;
		vel.x *= -particle::bounceFactor;
	}
	
	if (pos.y < size) {
		pos.y = size;
		vel.y *= -particle::bounceFactor;
	} else if(pos.y >= ofGetHeight() - size) {
		pos.y = ofGetHeight() - size;
		vel.y *= -particle::bounceFactor; 
	}
	
	size *= 0.95;
	size = ofClamp(size, 20, 1000);
}


void particle::draw() {
	ofPushMatrix();
	ofTranslate(pos.x - size / 2, pos.y - size / 2, 0);
	circleImage.draw(0, 0);
	ofPopMatrix();
}