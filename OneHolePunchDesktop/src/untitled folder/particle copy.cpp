/*
 *  particle.cpp
 *  opencvExample
 *
 *  Created by Tamar Ziv on 19/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "particle.h"
#include "ofMain.h"

particle::particle( ofxVec3f _pos, int _id, float _size, ofTexture _tex ) {
	
    ofSetCircleResolution(22);				// reduce circle res for performance
    
    texture = _tex;
	pos = _pos;								// get birth place from pixels
	pId = _id;
    size = _size;
    
    life = (int)ofRandom(10, 50);           // set particle life span
	initLife = life;

    vel = ofxVec3f(0,0,0);					// initiate velocity	
	acc = ofxVec3f(0,0,0);					// initiate acceleration
	damping = 1; //2; //0.05;
    
    myMask.loadImage("mask.tif");
	myMask.resize(size*2,size*2);

}


void particle::update() {
	
	life--;

	float randomness = 10.0;
	int step = 3;
	int div = 4000;
	
    // initiate velocity for debugging	
    vel = ofxVec3f(0,1,0);	
    
	// forces etc 
	ofxVec3f rand = ofxVec3f(ofRandom(-randomness, +randomness),ofRandom(-randomness, +randomness),0);
	ofxVec3f diffAttract = posAttract - pos;
	ofxVec3f diffRepel = posRepel - pos;

	float dist = diffAttract.length();
	diffAttract = diffAttract.normalize();
	diffRepel = diffRepel.normalize();
	
	acc += force*diffAttract;
	acc += -force*diffRepel;

	acc *= damping;
	vel += acc;
	vel.limit(5);
	pos += vel;
	acc *=0;
	vel *=0;
        
    size *= 0.95;
    size = ofClamp(size, 20, 1000);
	
}
	

void particle::draw() {


    ofPushMatrix();
	ofTranslate(pos.x-size/2, pos.y-size/2, 0);
	ofPushStyle();
    
        // draw the mask
        
        glEnable(GL_BLEND);  
        glColorMask(false, false, false, true);  
        glBlendFunc(GL_SRC_ALPHA, GL_ZERO);  
        glColor4f(0,0,0,1.0f); 
        myMask.resize(size*2,size*2);
        myMask.draw(0, 0, 0);
        
        
        
        // draw the images
        
        glColorMask(true,true,true,true);  
        glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);  
        glColor4f(1,1,1,0.8f);  
        texture.draw(0,0,size*2,size*2);
	ofPopStyle();
	ofPopMatrix();

}

void particle::goToTarget(ofxVec3f _posTarg, float _multiplier)
{
	posAttract = _posTarg;
	force = _multiplier;
}

void particle::runFromTarget(ofxVec3f _posTarg, float _multiplier)
{
	posRepel = _posTarg;
	force = _multiplier;

}

void particle::setAccel(ofxVec3f _acc)
{
	acc = _acc;	
}


void particle::die() {
	
}