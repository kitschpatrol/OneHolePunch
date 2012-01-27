#include "testApp.h"
#include "ofMain.h"

int main(){
	ofSetupOpenGL(960, 640, OF_FULLSCREEN);			// <-------- setup the GL context
	ofRunApp(new testApp);
}
