/*
 *  particle.h
 *  opencvExample
 *
 *  Created by Tamar Ziv on 19/4/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef PARTICLE
#define PARTICLE

#include "ofVectorMath.h"
#include "ofMain.h"
//#include "ofxiPhone.h"            // uncomment for iPhone
//#include "ofxiPhoneExtras.h"      // uncomment for iPhone    

class particle {
	public:
	particle ( ofVec3f _pos, int _id, float _size, ofTexture _tex );

    ofVec3f posAttract;
	ofVec3f posRepel;
	
	ofVec3f pos;
	ofVec3f vel;
	ofVec3f acc;
    ofVec3f mouse;

    
    int force, pId;
	int life;
	int initLife;
	int damping;
	float size, iniSize;
    
    int appWidth,appHeight;
    
    ofTexture texture;
    ofImage myMask;
	
	void update();
	void draw();
    
    /*
	void die();
	void goToTarget(ofVec3f _posTarg, float _multiplier);
	void runFromTarget(ofVec3f _posTarg, float _multiplier);
	void setAccel(ofVec3f _acc = ofVec3f(0.0f, 0.0f, 0.0f));
    */
};


#endif

