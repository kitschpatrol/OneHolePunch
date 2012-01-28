#include "particle.h"

float particle::bounceFactor = 0.7;
float particle::accelerometerForce = 0.2;	

particle::particle( ofVec3f _pos, int _id, float _size, ofTexture _tex ) {
	texture = _tex;
	pos = _pos;								// get birth place from pixels
	pId = _id;
	size = _size;
	
	vel = ofVec3f(0,0,0);
	acc = ofVec3f(0,0,0);

	myMask.loadImage("images/mask.tif"); // Y U NO in subfolder?
	myMask.resize(size*2,size*2);
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
	ofPushStyle();
	
	// draw the mask        
	glEnable(GL_BLEND);  
	glColorMask(false, false, false, true);  
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);  
	glColor4f(0, 0, 0, 1.0); 
	myMask.resize(size * 2, size * 2);
	myMask.draw(0, 0, 0);
	
	
	// draw the images
	glColorMask(true,true,true,true);  
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);  
	glColor4f(1,1,1,0.8f);  
	texture.draw(0,0,size*2,size*2);
	
	ofPopStyle();
	ofPopMatrix();
}