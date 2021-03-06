/*
*	File:		www.mikewoz.comspatosc_CocoaView.m
*	
*	Version:	1.0
* 
*	Created:	10-09-13
*	
*	Copyright:  Copyright � 2010 www.mikewoz.com, All Rights Reserved
* 
*	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc. ("Apple") in 
*				consideration of your agreement to the following terms, and your use, installation, modification 
*				or redistribution of this Apple software constitutes acceptance of these terms.  If you do 
*				not agree with these terms, please do not use, install, modify or redistribute this Apple 
*				software.
*
*				In consideration of your agreement to abide by the following terms, and subject to these terms, 
*				Apple grants you a personal, non-exclusive license, under Apple's copyrights in this 
*				original Apple software (the "Apple Software"), to use, reproduce, modify and redistribute the 
*				Apple Software, with or without modifications, in source and/or binary forms; provided that if you 
*				redistribute the Apple Software in its entirety and without modifications, you must retain this 
*				notice and the following text and disclaimers in all such redistributions of the Apple Software. 
*				Neither the name, trademarks, service marks or logos of Apple Computer, Inc. may be used to 
*				endorse or promote products derived from the Apple Software without specific prior written 
*				permission from Apple.  Except as expressly stated in this notice, no other rights or 
*				licenses, express or implied, are granted by Apple herein, including but not limited to any 
*				patent rights that may be infringed by your derivative works or by other works in which the 
*				Apple Software may be incorporated.
*
*				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO WARRANTIES, EXPRESS OR 
*				IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
*				AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE 
*				OR IN COMBINATION WITH YOUR PRODUCTS.
*
*				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL 
*				DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*				OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
*				REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER 
*				UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN 
*				IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#import "spatosc_CocoaView.h"

/*
enum {
	kParam_One =0,
	kNumberOfParameters=1
};
*/

#pragma mark ____ LISTENER CALLBACK DISPATCHER ____
void ParameterListenerDispatcher (void *inRefCon, void *inObject, const AudioUnitParameter *inParameter, Float32 inValue)
{
	spatosc_CocoaView *SELF = (spatosc_CocoaView *)inRefCon;
    [SELF _parameterListener:inObject parameter:inParameter value:inValue];
}

// This listener responds to parameter changes, gestures, and property notifications
void EventListenerDispatcher (void *inRefCon, void *inObject, const AudioUnitEvent *inEvent, UInt64 inHostTime, Float32 inValue)
{
	spatosc_CocoaView *SELF = (spatosc_CocoaView *)inRefCon;
	[SELF eventListener:inObject event: inEvent value: inValue];
	
	float azim, elev, dist;
	
	switch (inEvent->mArgument.mParameter.mParameterID)
	{
		case kSpatosc_Gain:
			printf("Gain: %f\n", inValue);
			break;
		case kSpatosc_Azim:
			AudioUnitGetParameter([SELF getAU], kSpatosc_Elev, kAudioUnitScope_Global, 0, &elev);
			AudioUnitGetParameter([SELF getAU], kSpatosc_Dist, kAudioUnitScope_Global, 0, &dist);
			lo_send(SELF->txAddr, "/AED", "fff", inValue, elev, dist);
			break;
		case kSpatosc_Elev:
			AudioUnitGetParameter([SELF getAU], kSpatosc_Azim, kAudioUnitScope_Global, 0, &azim);
			AudioUnitGetParameter([SELF getAU], kSpatosc_Dist, kAudioUnitScope_Global, 0, &dist);
			lo_send(SELF->txAddr, "/AED", "fff", azim, inValue, dist);
			break;
		case kSpatosc_Dist:
			AudioUnitGetParameter([SELF getAU], kSpatosc_Azim, kAudioUnitScope_Global, 0, &azim);
			AudioUnitGetParameter([SELF getAU], kSpatosc_Elev, kAudioUnitScope_Global, 0, &elev);
			lo_send(SELF->txAddr, "/AED", "fff", azim, elev, inValue);
			break;
	}
	/*
	AudioUnitGetParameter(				AudioUnit					inUnit,
						  AudioUnitParameterID		inID,
						  AudioUnitScope				inScope,
						  AudioUnitElement			inElement,
						  AudioUnitParameterValue *	outValue)			__OSX_AVAILABLE_STARTING(__MAC_10_0,__IPHONE_2_0);
	*/
	/*
	struct AudioUnitParameter
	{
		AudioUnit				mAudioUnit;
		AudioUnitParameterID	mParameterID;
		AudioUnitScope			mScope;
		AudioUnitElement		mElement;
	};
	 */
	
	//printf("got parameter update: %d = %f\n", (int)inEvent->mArgument.mParameter.mParameterID, inValue);
}




NSString *spatosc_GestureSliderMouseDownNotification = @"CAGestureSliderMouseDownNotification";
NSString *spatosc_GestureSliderMouseUpNotification = @"CAGestureSliderMouseUpNotification";

@implementation spatosc_GestureSlider

/*	We create our own custom subclass of NSSlider so we can do begin/end gesture notification
	We cannot override mouseUp: because it will never be called. Instead we do a clever trick in mouseDown to send mouseUp notifications */
- (void)mouseDown:(NSEvent *)inEvent {
	[[NSNotificationCenter defaultCenter] postNotificationName: spatosc_GestureSliderMouseDownNotification object: self];
	
	[super mouseDown: inEvent];	// this call does not return until mouse tracking is complete
								// once tracking is done, we know the mouse has been released, so we can send our mouseup notification

	[[NSNotificationCenter defaultCenter] postNotificationName: spatosc_GestureSliderMouseUpNotification object: self];
}
	
@end

@implementation spatosc_CocoaView
#pragma mark ____ (INIT /) DEALLOC ____
- (void)dealloc {
    [self _removeListeners];
    [super dealloc];
}

#pragma mark ____ PUBLIC FUNCTIONS ____
- (void)setAU:(AudioUnit)inAU {
	// remove previous listeners
	if (mAU) [self _removeListeners];
	mAU = inAU;
	
	// Dome view observers
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onDataChanged:) name: kDomeViewDataChangedNotification object: domeView];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onBeginGesture:) name: kDomeViewBeginGestureNotification object: domeView];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onEndGesture:) name: kDomeViewEndGestureNotification object: domeView];
	
	// Manual controls:
	
   	mParameter[0].mAudioUnit = inAU;
	mParameter[0].mParameterID = kSpatosc_Gain;
	mParameter[0].mScope = kAudioUnitScope_Global;
	mParameter[0].mElement = 0;
	
	mParameter[1].mAudioUnit = inAU;
	mParameter[1].mParameterID = kSpatosc_Azim;
	mParameter[1].mScope = kAudioUnitScope_Global;
	mParameter[1].mElement = 0;	

	mParameter[2].mAudioUnit = inAU;
	mParameter[2].mParameterID = kSpatosc_Elev;
	mParameter[2].mScope = kAudioUnitScope_Global;
	mParameter[2].mElement = 0;	

	mParameter[3].mAudioUnit = inAU;
	mParameter[3].mParameterID = kSpatosc_Dist;
	mParameter[3].mScope = kAudioUnitScope_Global;
	mParameter[3].mElement = 0;	
	
	txAddr = lo_address_new("localhost", "7777");

	// add new listeners
	[self _addListeners];
	
	// initial setup
	[self _synchronizeUIWithParameterValues];
}

- (AudioUnit)getAU
{
	return mAU;
}

#pragma mark ____ INTERFACE ACTIONS ____

- (IBAction)gainChanged:(id)sender {
    float floatValue = [sender floatValue];
	NSAssert(AUParameterSet(mParameterListener, sender, &mParameter[0], (Float32)floatValue, 0) == noErr, @"[spatosc_CocoaView gainChanged:] AUParameterSet()");
    if (sender == gainSlider) {
        [gainField setFloatValue:floatValue];
    } else {
        [gainSlider setFloatValue:floatValue];
    }
}

- (IBAction)azimChanged:(id)sender {
    float floatValue = [sender floatValue];
	
	NSAssert(AUParameterSet(mParameterListener, sender, &mParameter[1], (Float32)floatValue, 0) == noErr, @"[spatosc_CocoaView azimChanged:] AUParameterSet()");
    if (sender == azimSlider) {
        [azimField setFloatValue:floatValue];
    } else {
        [azimSlider setFloatValue:floatValue];
    }
	
	/*
	AudioUnitParameter azimParameter = {mAU, spatosc::pack(kSpatosc_Azim, 0), kAudioUnitScope_Global, 0 };
	NSAssert(AUParameterSet(mAUEventListener, sender, &azimuthParameter, (Float32)floatValue, 0) == noErr, @"[spatosc_CocoaView azimChanged:] AUParameterSet()");
	*/
	
}

- (IBAction)elevChanged:(id)sender {
    float floatValue = [sender floatValue];
	NSAssert(AUParameterSet(mParameterListener, sender, &mParameter[2], (Float32)floatValue, 0) == noErr, @"[spatosc_CocoaView elevChanged:] AUParameterSet()");
    if (sender == elevSlider) {
        [elevField setFloatValue:floatValue];
    } else {
        [elevSlider setFloatValue:floatValue];
    
	}
}
- (IBAction)distChanged:(id)sender {
    float floatValue = [sender floatValue];
	NSAssert(AUParameterSet(mParameterListener, sender, &mParameter[3], (Float32)floatValue, 0) == noErr, @"[spatosc_CocoaView distChanged:] AUParameterSet()");
    if (sender == azimSlider) {
        [distField setFloatValue:floatValue];
    } else {
        [distSlider setFloatValue:floatValue];
    }
}


#pragma mark ____ Dome view actions ___

- (void) onDataChanged:(NSNotification *) aNotification
{
    AudioUnitParameter azimParameter = { mAU, kSpatosc_Azim, kAudioUnitScope_Global, 0 };
    AudioUnitParameter elevParameter = { mAU, kSpatosc_Elev,  kAudioUnitScope_Global, 0 };
	
	NSAssert(AUParameterSet(mEventListener, azimField, &azimParameter, (Float32)domeView.azimuth, 0) == noErr, @"[spatosc_CocoaView azimChanged:] AUParameterSet()");
	NSAssert(AUParameterSet(mEventListener, elevField, &elevParameter, (Float32)domeView.zenith, 0) == noErr, @"[spatosc_CocoaView elevChanged:] AUParameterSet()");
}

- (void) onBeginGesture:(NSNotification *) aNotification 
{
	AudioUnitEvent event;
	AudioUnitParameter parameter = { mAU, kSpatosc_Azim, kAudioUnitScope_Global, 0 };
	event.mArgument.mParameter = parameter;
	event.mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
	
	AUEventListenerNotify (mEventListener, self, &event);
	
	event.mArgument.mParameter.mParameterID = kSpatosc_Elev;
	AUEventListenerNotify (mEventListener, self, &event);
}

- (void) onEndGesture:(NSNotification *) aNotification
{
	AudioUnitEvent event;
	AudioUnitParameter parameter = {mAU, kSpatosc_Azim, kAudioUnitScope_Global, 0 };
	event.mArgument.mParameter = parameter;
	event.mEventType = kAudioUnitEvent_EndParameterChangeGesture;
	
	AUEventListenerNotify (mEventListener, self, &event);
	
	event.mArgument.mParameter.mParameterID = kSpatosc_Elev;
	AUEventListenerNotify (mEventListener, self, &event);	
}


#pragma mark ____ NOTIFICATIONS ____

// This routine is called when the user has clicked on the slider. We need to send a begin parameter change gesture to alert hosts that the parameter may be changing value
-(void) handleMouseDown: (NSNotification *) aNotification {
	if ([aNotification object] == gainSlider) {
		AudioUnitEvent event;
		event.mArgument.mParameter = mParameter[0];
		event.mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
		
		AUEventListenerNotify (NULL, self, &event);		// NOTE, if you have an AUEventListenerRef because you are listening to event notification, 
														// pass that as the first argument to AUEventListenerNotify instead of NULL 
	}
	
	else if ([aNotification object] == azimSlider) {
		AudioUnitEvent event;
		event.mArgument.mParameter = mParameter[1];
		event.mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
		
		AUEventListenerNotify (NULL, self, &event);		// NOTE, if you have an AUEventListenerRef because you are listening to event notification, 
		// pass that as the first argument to AUEventListenerNotify instead of NULL 
	}
	
	else if ([aNotification object] == elevSlider) {
		AudioUnitEvent event;
		event.mArgument.mParameter = mParameter[2];
		event.mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
		
		AUEventListenerNotify (NULL, self, &event);		// NOTE, if you have an AUEventListenerRef because you are listening to event notification, 
		// pass that as the first argument to AUEventListenerNotify instead of NULL 
	}
	
	else if ([aNotification object] == distSlider) {
		AudioUnitEvent event;
		event.mArgument.mParameter = mParameter[3];
		event.mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
		
		AUEventListenerNotify (NULL, self, &event);		// NOTE, if you have an AUEventListenerRef because you are listening to event notification, 
		// pass that as the first argument to AUEventListenerNotify instead of NULL 
	}
}

-(void) handleMouseUp: (NSNotification *) aNotification {
	if ([aNotification object] == gainSlider) {
		AudioUnitEvent event;
		event.mArgument.mParameter = mParameter[0];
		event.mEventType = kAudioUnitEvent_EndParameterChangeGesture;
	
		AUEventListenerNotify (NULL, self, &event);		// NOTE, if you have an AUEventListenerRef because you are listening to event notification, 
														// pass that as the first argument to AUEventListenerNotify instead of NULL 
	}
	else if ([aNotification object] == azimSlider) {
		AudioUnitEvent event;
		event.mArgument.mParameter = mParameter[1];
		event.mEventType = kAudioUnitEvent_EndParameterChangeGesture;
		
		AUEventListenerNotify (NULL, self, &event);		// NOTE, if you have an AUEventListenerRef because you are listening to event notification, 
		// pass that as the first argument to AUEventListenerNotify instead of NULL 
	}
	else if ([aNotification object] == elevSlider) {
		AudioUnitEvent event;
		event.mArgument.mParameter = mParameter[2];
		event.mEventType = kAudioUnitEvent_EndParameterChangeGesture;
		
		AUEventListenerNotify (NULL, self, &event);		// NOTE, if you have an AUEventListenerRef because you are listening to event notification, 
		// pass that as the first argument to AUEventListenerNotify instead of NULL 
	}
	else if ([aNotification object] == distSlider) {
		AudioUnitEvent event;
		event.mArgument.mParameter = mParameter[3];
		event.mEventType = kAudioUnitEvent_EndParameterChangeGesture;
		
		AUEventListenerNotify (NULL, self, &event);		// NOTE, if you have an AUEventListenerRef because you are listening to event notification, 
		// pass that as the first argument to AUEventListenerNotify instead of NULL 
	}
}


#pragma mark ____ PRIVATE FUNCTIONS ____

void addParamListener (AUEventListenerRef listener, void* refCon, AudioUnitEvent *inEvent)
{
	inEvent->mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
	verify_noerr ( AUEventListenerAddEventType(	listener, refCon, inEvent));
	
	inEvent->mEventType = kAudioUnitEvent_EndParameterChangeGesture;
	verify_noerr ( AUEventListenerAddEventType(	listener, refCon, inEvent));
	
	inEvent->mEventType = kAudioUnitEvent_ParameterValueChange;
	verify_noerr ( AUEventListenerAddEventType(	listener, refCon, inEvent));	
}


- (void)_addListeners {
	NSAssert (	AUListenerCreate(	ParameterListenerDispatcher, self, 
                                    CFRunLoopGetCurrent(), kCFRunLoopDefaultMode, 0.100, // 100 ms
                                    &mParameterListener	) == noErr,
                @"[spatosc_CocoaView _addListeners] AUListenerCreate()");
	
    int i;
    for (i = 0; i < kNumberOfParameters; ++i) {
        mParameter[i].mAudioUnit = mAU;
        NSAssert (	AUListenerAddParameter (mParameterListener, NULL, &mParameter[i]) == noErr,
                    @"[spatosc_CocoaView _addListeners] AUListenerAddParameter()");
    }
    
	verify_noerr( AUEventListenerCreate(EventListenerDispatcher, self,
										CFRunLoopGetCurrent(), kCFRunLoopDefaultMode, 0.01, 0.1, 
										&mEventListener));
	

	AudioUnitEvent auEvent;
	AudioUnitParameter parameter = {mAU, kSpatosc_Gain, kAudioUnitScope_Global, 0 };
	auEvent.mArgument.mParameter = parameter;
	
	addParamListener (mEventListener, self, &auEvent);

	auEvent.mArgument.mParameter.mParameterID = kSpatosc_Azim;
	addParamListener (mEventListener, self, &auEvent);
		
	auEvent.mArgument.mParameter.mParameterID = kSpatosc_Elev;
	addParamListener (mEventListener, self, &auEvent);
		
	auEvent.mArgument.mParameter.mParameterID = kSpatosc_Dist;
	addParamListener (mEventListener, self, &auEvent);
		

	
	
	/*
   	//[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleMouseDown:) name:spatosc_GestureSliderMouseDownNotification object: uiParam1Slider];
	//[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleMouseUp:) name:spatosc_GestureSliderMouseUpNotification object: uiParam1Slider];

	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleMouseDown:) name:spatosc_GestureSliderMouseDownNotification object: gainSlider];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleMouseUp:) name:spatosc_GestureSliderMouseUpNotification object: gainSlider];
	
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleMouseDown:) name:spatosc_GestureSliderMouseDownNotification object: azimSlider];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleMouseUp:) name:spatosc_GestureSliderMouseUpNotification object: azimSlider];
	
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleMouseDown:) name:spatosc_GestureSliderMouseDownNotification object: elevSlider];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleMouseUp:) name:spatosc_GestureSliderMouseUpNotification object: elevSlider];
	
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleMouseDown:) name:spatosc_GestureSliderMouseDownNotification object: distSlider];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleMouseUp:) name:spatosc_GestureSliderMouseUpNotification object: distSlider];
	 */


}

- (void)_removeListeners {
    int i;
    for (i = 0; i < kNumberOfParameters; ++i) {
        NSAssert (	AUListenerRemoveParameter(mParameterListener, NULL, &mParameter[i]) == noErr,
                    @"[spatosc_CocoaView _removeListeners] AUListenerRemoveParameter()");
    }
    
	NSAssert (	AUListenerDispose(mParameterListener) == noErr,
                @"[spatosc_CocoaView _removeListeners] AUListenerDispose()");
                
    [[NSNotificationCenter defaultCenter] removeObserver: self];
}

- (void)_synchronizeUIWithParameterValues {
	Float32 value;
    int i;
    
    for (i = 0; i < kNumberOfParameters; ++i) {
        // only has global parameters
        NSAssert (	AudioUnitGetParameter(mAU, mParameter[i].mParameterID, kAudioUnitScope_Global, 0, &value) == noErr,
                    @"[spatosc_CocoaView synchronizeUIWithParameterValues] (x.1)");
        NSAssert (	AUParameterSet (mParameterListener, self, &mParameter[i], value, 0) == noErr,
                    @"[spatosc_CocoaView synchronizeUIWithParameterValues] (x.2)");
        NSAssert (	AUParameterListenerNotify (mParameterListener, self, &mParameter[i]) == noErr,
                    @"[spatosc_CocoaView synchronizeUIWithParameterValues] (x.3)");
    }
}

#pragma mark ____ LISTENER CALLBACK DISPATCHEE ____
- (void)_parameterListener:(void *)inObject parameter:(const AudioUnitParameter *)inParameter value:(Float32)inValue {
    //inObject ignored in this case.
    
	switch (inParameter->mParameterID) {
		case kSpatosc_Gain:
			[gainSlider setFloatValue:inValue];
			[gainField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kSpatosc_Azim:
			[azimSlider setFloatValue:inValue];
			[azimField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kSpatosc_Elev:
			[elevSlider setFloatValue:inValue];
			[elevField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kSpatosc_Dist:
			[distSlider setFloatValue:inValue];
			[distField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
	}
}


- (void)eventListener:(void *) inObject event:(const AudioUnitEvent *)inEvent value:(Float32)inValue
{
	switch (inEvent->mEventType)
    {
		case kAudioUnitEvent_ParameterValueChange:
			switch (inEvent->mArgument.mParameter.mParameterID)
			{
				case kSpatosc_Gain:
					[gainSlider setFloatValue: inValue];
					[gainField setFloatValue: inValue];
					break;
				case kSpatosc_Azim:
					[azimSlider setFloatValue: inValue];
					[azimField  setFloatValue: inValue];
					[domeView setAzimuth: inValue];
					[domeView setNeedsDisplay:YES];
					break;
				case kSpatosc_Elev:
					[elevSlider setFloatValue: inValue];
					[elevField setFloatValue: inValue];
					[domeView setZenith: inValue];
					[domeView setNeedsDisplay:YES];
					break;
				case kSpatosc_Dist:
					[distSlider setFloatValue: inValue];
					[distField setFloatValue: inValue];
					break;
			}
			break;
		case kAudioUnitEvent_BeginParameterChangeGesture:
			[domeView handleBeginGesture];
			break;
		case kAudioUnitEvent_EndParameterChangeGesture:
			[domeView handleEndGesture];
			break;
	}
}



@end
