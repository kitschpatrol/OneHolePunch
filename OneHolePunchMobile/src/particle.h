#pragma once

#include "ofMain.h"

class particle {
	public:
		particle(ofVec3f _pos, int _id, float _size, ofImage _circleImage);

    ofVec3f posAttract;
		ofVec3f posRepel;
	
		ofVec3f pos;
		ofVec3f vel;
		ofVec3f acc;

		int force;
		int pId;
		int life;
		int initLife;
		int damping;
		float size;
		float iniSize;

	
	float rotation;
	float rotationVelocity;
	float rotationAcceleration;
    
    ofImage circleImage;
    ofImage myMask;
	
		void update(float accelX, float accelY);
		void draw();
	
		// parameters
		static float bounceFactor;
		static float accelerometerForce;
};

