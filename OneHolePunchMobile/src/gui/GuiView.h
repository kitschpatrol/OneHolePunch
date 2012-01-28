#import <UIKit/UIKit.h>

#include "testApp.h"

@interface GuiView : UIViewController {
	IBOutlet UILabel *sliderValueText;
	IBOutlet UILabel *frameRateText;
	IBOutlet UILabel *circleCountText;
	
	testApp *myApp;		// points to our instance of testApp
}

-(void)setSliderValueString:(NSString *)value;
-(void)setFrameRateString:(NSString *)frameRate;
-(void)setCircleCountString:(NSString *)circleCount;

// our circle finder sliders
//-(IBAction)adjustLerpRad:(id)sender;
//-(IBAction)adjustLerpPos:(id)sender;

-(IBAction)adjustHueRes:(id)sender;
-(IBAction)adjustHueMinDist:(id)sender;
-(IBAction)adjustHueParam1:(id)sender;
-(IBAction)adjustHueParam2:(id)sender;
-(IBAction)adjustMinRadius:(id)sender;
-(IBAction)adjustMaxRadius:(id)sender;
-(IBAction)adjustBlurAmount:(id)sender;

-(IBAction)back:(id)sender;

@end
