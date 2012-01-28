#include "particle.h"

float particle::bounceFactor = 0.2;
float particle::accelerometerForce = 6;	

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
	
	rotationVelocity = (vel.x / 2) + (vel.y / 2); //particle::accelerometerForce * accelR * ofRandomuf();
	
	rotation += rotationVelocity;
	
	// add vel to pos
	pos += vel;
	
	// check boundaries
	if (pos.x < size / 2) {
		pos.x = size / 2;
		vel.x *= -particle::bounceFactor;
	} else if(pos.x >= ofGetWidth() - size / 2) {
		pos.x = ofGetWidth() - size / 2;
		vel.x *= -particle::bounceFactor;
	}
	
	if (pos.y < size / 2) {
		pos.y = size / 2;
		vel.y *= -particle::bounceFactor;
	} else if (pos.y >= ofGetHeight() - size / 2) {
		pos.y = ofGetHeight() - size / 2;
		vel.y *= -particle::bounceFactor; 
	}
	
	size *= 0.98;
	size = ofClamp(size, iniSize * 0.25, 1000);
}


void particle::draw() {
	ofPushMatrix();
	//ofTranslate(-size / 2, -size / 2, 0);
	ofTranslate(pos.x, pos.y);
	ofRotate(rotation);	
	circleImage.draw(-size / 2, -size / 2, size, size);
	ofPopMatrix();
}