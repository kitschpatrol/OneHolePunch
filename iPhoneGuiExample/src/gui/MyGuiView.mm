//
//  MyGuiView.m
//  iPhone Empty Example
//
//  Created by theo on 26/01/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MyGuiView.h"
#include "ofxiPhoneExtras.h"


@implementation MyGuiView

// called automatically after the view is loaded, can be treated like the constructor or setup() of this class
-(void)viewDidLoad {
	myApp = (testApp*)ofGetAppPtr();
}

//----------------------------------------------------------------
-(void)setStatusString:(NSString *)trackStr{
	displayText.text = trackStr;
}

//----------------------------------------------------------------
-(IBAction)more:(id)sender{
	myApp->lengthRatio += 0.1;
	
	string statusStr = " Status: ratio is " + ofToString(myApp->lengthRatio, 2);
	[self setStatusString:ofxStringToNSString(statusStr)];		
}

//----------------------------------------------------------------
-(IBAction)less:(id)sender{
	myApp->lengthRatio -= 0.1;
	if( myApp->lengthRatio < 0.1 ){
		myApp->lengthRatio = 0.1;
	}

	string statusStr = " Status: ratio is " + ofToString(myApp->lengthRatio, 2);
	[self setStatusString:ofxStringToNSString(statusStr)];		
}

//----------------------------------------------------------------
-(IBAction)hide:(id)sender{
	self.view.hidden = YES;
}

//----------------------------------------------------------------
-(IBAction)adjustPoints:(id)sender{
	
	UISlider * slider = sender;
	printf("slider value is - %f\n", [slider value]);
	
	myApp->numPoints = 3 + [slider value] * 28;
	
	string statusStr = " Status: numPoints is " + ofToString(myApp->numPoints);
	[self setStatusString:ofxStringToNSString(statusStr)];
	
}


//----------------------------------------------------------------

//----------------------------------------------------------------



-(IBAction)adjustLerpRad:(id)sender;
{
 	UISlider * slider = sender;
	printf("slider adjustLerpRad is - %f\n", [slider value]);
	
	myApp->lerpRad = 3 + [slider value] * 28;
	
	string statusStr = " Status: lerpRad is " + ofToString(myApp->lerpRad);
	[self setStatusString:ofxStringToNSString(statusStr)];   
}
//----------------------------------------------------------------

-(IBAction)adjustLerpPos:(id)sender;
{
 	UISlider * slider = sender;
	printf("slider adjustLerpPos is - %f\n", [slider value]);
	
	myApp->lerpPos = 3 + [slider value] * 28;
	
	string statusStr = " Status: lerp pos is " + ofToString(myApp->lerpPos);
	[self setStatusString:ofxStringToNSString(statusStr)];   
}
//----------------------------------------------------------------


-(IBAction)adjustHueRes:(id)sender;
{
 	UISlider * slider = sender;
	printf("slider adjustHueRes is - %f\n", [slider value]);
	
	myApp->hueRes = 3 + [slider value] * 28;
	
	string statusStr = " Status: hueRes is " + ofToString(myApp->hueRes);
	[self setStatusString:ofxStringToNSString(statusStr)];   
}
//----------------------------------------------------------------

-(IBAction)adjustHueMinDist:(id)sender
{
    UISlider * slider = sender;
	printf("slider adjustHueRes is - %f\n", [slider value]);
	
	myApp->minDist = [slider value];
	
	string statusStr = " Status: minDist is " + ofToString(myApp->minDist);
	[self setStatusString:ofxStringToNSString(statusStr)]; 
}

//----------------------------------------------------------------

-(IBAction)adjustHueParam1:(id)sender
{
    UISlider * slider = sender;
	printf("slider param1 is - %f\n", [slider value]);
	
	myApp->param1 = 3 + [slider value] * 28;
	
	string statusStr = " Status: param1 is " + ofToString(myApp->param1);
	[self setStatusString:ofxStringToNSString(statusStr)]; 
}

//----------------------------------------------------------------

-(IBAction)adjustHueParam2:(id)sender
{
    UISlider * slider = sender;
	printf("slider param2 is - %f\n", [slider value]);
	
	myApp->param2 = 3 + [slider value] * 28;
	
	string statusStr = " Status: param2 is " + ofToString(myApp->param2);
	[self setStatusString:ofxStringToNSString(statusStr)]; 
}

//----------------------------------------------------------------
//----------------------------------------------------------------



-(IBAction)fillSwitch:(id)sender{
	
	UISwitch * toggle = sender;
	printf("switch value is - %i\n", [toggle isOn]);
	
	myApp->bFill = [toggle isOn];
	
	string statusStr = " Status: fill is " + ofToString(myApp->bFill);
	[self setStatusString:ofxStringToNSString(statusStr)];	
}

@end
