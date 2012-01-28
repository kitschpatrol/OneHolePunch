#include "particle.h"

float particle::bounceFactor = 0.2;
float particle::accelerometerForce = 3;	

particle::particle( ofVec3f _pos, int _id, float _size, ofImage _circleImage) {
	circleImage = _circleImage;
	pos = _pos;								// get birth place from pixels
	pId = _id;
	size = _size;
	
	rotation = 0;

	
	cout << "INI size: " << size << endl;
	vel = ofVec3f(0,0,0);
	acc = ofVec3f(0,0,0);
}


void particle::update(float accelX, float accelY) {	
	vel.x += particle::accelerometerForce * accelX * ofRandomuf();
	vel.y += -particle::accelerometerForce * accelY * ofRandomuf();
	
	//float accelR = 1;
	
	rotationVelocity = (vel.x); //particle::accelerometerForce * accelR * ofRandomuf();
	
	rotation += rotationVelocity;
	
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
	
	size *= 0.98;
	size = ofClamp(size, iniSize * 0.5, 1000);
}


void particle::draw() {
	ofPushMatrix();
	//ofTranslate(-size / 2, -size / 2, 0);
	ofTranslate(pos.x, pos.y);
	ofRotate(rotation);	
	circleImage.draw(-size / 2, -size / 2, size, size);
	ofPopMatrix();
}