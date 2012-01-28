#import "GuiView.h"
#include "ofxiPhoneExtras.h"

@implementation GuiView

// called automatically after the view is loaded, can be treated like the constructor or setup() of this class
-(void)viewDidLoad {
	myApp = (testApp*)ofGetAppPtr();
}

// Outlets
-(void)setSliderValueString:(NSString *)value {
	sliderValueText.text = value;
}

-(void)setFrameRateString:(NSString *)frameRate {
	frameRateText.text = frameRate;
}

-(void)setCircleCountString:(NSString *)circleCount {
	circleCountText.text = circleCount;
}


// Inlets
-(IBAction)back:(id)sender {
	self.view.hidden = YES;
}


//-(IBAction)adjustLerpRad:(id)sender {
// 	UISlider * slider = sender;
//	
//	myApp->lerpRad = [slider value];
//	
//	string statusStr = " Value: " + ofToString(myApp->lerpRad);
//
//	[self setSliderValueString:ofxStringToNSString(statusStr)];   
//}
//
//
//-(IBAction)adjustLerpPos:(id)sender {
// 	UISlider * slider = sender;
//	
//	myApp->lerpPos = [slider value];
//	
//	string statusStr = " Value: " + ofToString(myApp->lerpPos, 2);
//	[self setSliderValueString:ofxStringToNSString(statusStr)];   
//}


-(IBAction)adjustHueRes:(id)sender {
 	UISlider * slider = sender;
	
	myApp->hueRes = [slider value];
	
	string statusStr = " Value: " + ofToString(myApp->hueRes, 2);
	[self setSliderValueString:ofxStringToNSString(statusStr)];   
}


-(IBAction)adjustHueMinDist:(id)sender {
    UISlider * slider = sender;
	
	myApp->minDist = [slider value];
	
	string statusStr = " Value: " + ofToString(myApp->minDist, 2);
	[self setSliderValueString:ofxStringToNSString(statusStr)]; 
}


-(IBAction)adjustHueParam1:(id)sender {
    UISlider * slider = sender;
	
	myApp->param1 = [slider value];
	
	string statusStr = " Value: " + ofToString(myApp->param1, 2);
	[self setSliderValueString:ofxStringToNSString(statusStr)]; 
}


-(IBAction)adjustHueParam2:(id)sender {
	UISlider * slider = sender;
	
	myApp->param2 = [slider value];
	
	string statusStr = " Value: " + ofToString(myApp->param2, 2);
	[self setSliderValueString:ofxStringToNSString(statusStr)]; 
}

-(IBAction)adjustMinRadius:(id)sender {
	UISlider * slider = sender;
	
	myApp->minRadius = [slider value];
	
	string statusStr = " Value: " + ofToString(myApp->minRadius, 0);
	[self setSliderValueString:ofxStringToNSString(statusStr)]; 
}


-(IBAction)adjustMaxRadius:(id)sender {
	UISlider * slider = sender;
	
	myApp->maxRadius = [slider value];
	
	string statusStr = " Value: " + ofToString(myApp->maxRadius, 0);
	[self setSliderValueString:ofxStringToNSString(statusStr)]; 
}

-(IBAction)adjustBlurAmount:(id)sender {
		UISlider * slider = sender;
	
	myApp->blurAmount = [slider value];
	// Must be odd
	
	if (myApp->blurAmount % 2 == 0) {
		myApp->blurAmount = myApp->blurAmount - 1;
	}
	
	string statusStr = " Value: " + ofToString(myApp->blurAmount, 0);
	[self setSliderValueString:ofxStringToNSString(statusStr)]; 	
}


-(IBAction)debugSwitch:(id)sender {
	UISwitch * toggle = sender;
	
	if ([toggle isOn]) {
		myApp->enableDebug();
	}
	else {
		myApp->disableDebug();		
	}
}


@end
