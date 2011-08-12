#import <Cocoa/Cocoa.h>
#import <AudioUnit/AudioUnit.h>
#import <AudioToolbox/AudioToolbox.h>

#import "Zirkalloy_ShapeView.h"
#import "Zirkalloy_DomeView.h"
#import "LeopardHeaderGradient.h"
#import "Zirkalloy.h"

@interface Zirkalloy_UIView : NSView
{	
	IBOutlet Zirkalloy_DomeView         *domeView;
	IBOutlet Zirkalloy_ShapeView        *shapeView;
    IBOutlet NSTextField				*azimuthField;
	IBOutlet NSTextField				*zenithField;
    IBOutlet NSTextField				*azimuthSpanField;
	IBOutlet NSTextField				*zenithSpanField;
    IBOutlet NSTextField                *gainField;
    IBOutlet NSSlider                   *gainSlider;
    IBOutlet NSPopUpButton              *channelMenu;
    IBOutlet NSBox                      *stereoSides;
    IBOutlet LeopardHeaderGradient      *headerBox;
	
    // Other Members
    AudioUnit 				mAU;
	AUEventListenerRef		mAUEventListener;
    
    // Active channel in interface
    // 0: L
    // 1: R
    UInt16                  mActiveChannel;
    
    BOOL                    isStereo;

	NSColor				   *mBackgroundColor;
}

#pragma mark ____ PUBLIC FUNCTIONS ____
- (void)setAU:(AudioUnit)inAU;
- (void)setChannelCount:(UInt32)count;

#pragma mark ____ INTERFACE ACTIONS ____
- (IBAction) azimuthChanged:(id)sender;
- (IBAction) zenithChanged:(id)sender;
- (IBAction) azimuthSpanChanged:(id)sender;
- (IBAction) zenithSpanChanged:(id)sender;
- (IBAction) gainChanged:(id)sender;
- (IBAction) channelChanged:(id)sender;
- (IBAction) activeChannelSelected:(id)sender;
@end
