/*
 *  particle.cpp
 *  opencvExample
 *
 *  Created by Tamar Ziv on 19/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#define BOUNCE_FACTOR			0.7
#define ACCELEROMETER_FORCE		0.2

#include "particle.h"
#include "ofMain.h"

particle::particle( ofVec3f _pos, int _id, float _size, ofTexture _tex ) {
	
    ofSetCircleResolution(22);				// reduce circle res for performance
    
    texture = _tex;
	pos = _pos;								// get birth place from pixels
	pId = _id;
    size = _size;
    
    life = (int)ofRandom(10, 50);           // set particle life span
	initLife = life;

    vel = ofVec3f(0,0,0);					// initiate velocity	
	acc = ofVec3f(0,0,0);					// initiate acceleration
    mouse = ofVec3f(0,0,0);					// initiate acceleration
	damping = 1; //2; //0.05;
    
    myMask.loadImage("mask.tif");
	myMask.resize(size*2,size*2);
    
    appWidth = ofGetWidth();
    appHeight = ofGetHeight();

}


void particle::update() {
	
    /*
	life--;

	float randomness = 10.0;
	int step = 3;
	int div = 4000;
	
    // initiate velocity for debugging	
    //vel = ofVec3f(0,1,0);	
    
	// forces etc 
	ofVec3f rand = ofVec3f(ofRandom(-randomness, +randomness),ofRandom(-randomness, +randomness),0);
	ofVec3f diffAttract = posAttract - pos;
	ofVec3f diffRepel = posRepel - pos;

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
    
    ofPoint bounds = ofPoint( appWidth,appHeight );
    pos.x = ofClamp( pos.x , size, appWidth-size );
    pos.y = ofClamp( pos.y , size, appHeight-size );
        
    size *= 0.95;
    size = ofClamp(size, 20, 1000);
    
    */
    
    //  vel.x += ACCELEROMETER_FORCE * ofxAccelerometer.getForce().x * ofRandomuf();
    //  vel.y += -ACCELEROMETER_FORCE * ofxAccelerometer.getForce().y * ofRandomuf();        // this one is subtracted cos world Y is opposite to opengl Y

    ofVec3f dif = mouse - pos;
    dif.normalize();
    
    vel.x += ACCELEROMETER_FORCE * dif.x * ofRandomuf();
    vel.y += -ACCELEROMETER_FORCE * dif.y * ofRandomuf();        // this one is subtracted cos world Y is opposite to opengl Y
    
    // add vel to pos
    pos += vel;
    
    // check boundaries
    if(pos.x < size) {
        pos.x = size;
        vel.x *= -BOUNCE_FACTOR;
    } else if(pos.x >= ofGetWidth() - size) {
        pos.x = ofGetWidth() - size;
        vel.x *= -BOUNCE_FACTOR;
    }
    
    if(pos.y < size) {
        pos.y = size;
        vel.y *= -BOUNCE_FACTOR;
    } else if(pos.y >= ofGetHeight() - size) {
        pos.y = ofGetHeight() - size;
        vel.y *= -BOUNCE_FACTOR; 
    }
    
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

/*
void particle::goToTarget(ofVec3f _posTarg, float _multiplier)
{
	posAttract = _posTarg;
	force = _multiplier;
}

void particle::runFromTarget(ofVec3f _posTarg, float _multiplier)
{
	posRepel = _posTarg;
	force = _multiplier;

}

void particle::setAccel(ofVec3f _acc)
{
	acc = _acc;	
}


void particle::die() {
	
}
*/