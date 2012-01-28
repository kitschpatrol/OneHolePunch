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

#include "ofxVectorMath.h"
#include "ofMain.h"


class particle {
	public:
	particle ( ofxVec3f _pos, int _id, float _size, ofTexture _tex );

    ofxVec3f posAttract;
	ofxVec3f posRepel;
	int force, pId;
	
	ofxVec3f pos;
	ofxVec3f vel;
	ofxVec3f acc;
    
	int life;
	int initLife;
	int damping;
	float size, iniSize;
    ofTexture texture;
    ofImage myMask;
    ofImage myCircle;
	
	void update();
	void draw();
	void die();
	
	void goToTarget(ofxVec3f _posTarg, float _multiplier);
	void runFromTarget(ofxVec3f _posTarg, float _multiplier);
	void setAccel(ofxVec3f _acc = ofxVec3f(0.0f, 0.0f, 0.0f));
};


#endif

