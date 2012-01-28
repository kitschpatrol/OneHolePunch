#include "ofMain.h"
#include "testApp.h"

int main(){
	ofAppiPhoneWindow * iOSWindow = new ofAppiPhoneWindow();
	
	//iOSWindow->enableAntiAliasing(4);
	iOSWindow->setOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);	
	iOSWindow->enableRetinaSupport();



	cout << "Screen width: " << iOSWindow->getScreenSize().x << endl;
	cout << "Screen height: " << iOSWindow->getScreenSize().y << endl;	

	
	//ofSetupOpenGL(iOSWindow, 480, 320, OF_FULLSCREEN);
	ofSetupOpenGL(iOSWindow, iOSWindow->getScreenSize().x, iOSWindow->getScreenSize().y, OF_FULLSCREEN);	
	ofRunApp(new testApp);
}
