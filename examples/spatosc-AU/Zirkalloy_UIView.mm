#import "Zirkalloy_UIView.h"
#import "ZKMRNDeviceConstants.h"

extern NSString *kDomeViewDataChangedNotification;
extern NSString *kDomeViewBeginGestureNotification;
extern NSString *kDomeViewEndGestureNotification;

extern NSString *kShapeViewDataChangedNotification;
extern NSString *kShapeViewBeginGestureNotification;
extern NSString *kShapeViewEndGestureNotification;

static NSString *kGainChangedNotification = @"Zirkalloy_GainChangedNotification";
static NSString *kGainBeginGestureNotification = @"Zirkalloy_GainBeginGestureNotification";
static NSString *kGainEndGestureNotification = @"Zirkalloy_GainEndGestureNotification";

@interface Zirkalloy_UIView (UIViewPrivate)
#pragma mark ____ PRIVATE FUNCTIONS
    - (void)registerAUListeners;
    - (void)unregisterAUListeners;

    - (void)refreshView;

#pragma mark ____ LISTENER CALLBACK DISPATCHEE ____
    - (void)eventListener:(void *) inObject event:(const AudioUnitEvent *)inEvent value:(Float32)inValue;
@end

#pragma mark ____ LISTENER CALLBACK DISPATCHER ____

// This listener responds to parameter changes, gestures, and property notifications
void EventListenerDispatcher (void *inRefCon, void *inObject, const AudioUnitEvent *inEvent, UInt64 inHostTime, Float32 inValue)
{
	Zirkalloy_UIView *SELF = (Zirkalloy_UIView *)inRefCon;
	[SELF eventListener:inObject event: inEvent value: inValue];
}

@implementation Zirkalloy_UIView

-(void) awakeFromNib
{    
    mBackgroundColor = [NSColor windowBackgroundColor];
    
    [self setAutoresizingMask:NSViewNotSizable];
}

#pragma mark ____ (INIT /) DEALLOC ____
- (void)dealloc
{
    [self unregisterAUListeners];
	[mBackgroundColor release];
		
	[[NSNotificationCenter defaultCenter] removeObserver: self];

    [super dealloc];
}

#pragma mark ____ PUBLIC FUNCTIONS ____
- (void)setAU:(AudioUnit)inAU
{
	// remove previous listeners
	if (mAU) 
		[self unregisterAUListeners];

    // Dome view observers
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onDataChanged:) name: kDomeViewDataChangedNotification object: domeView];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onBeginGesture:) name: kDomeViewBeginGestureNotification object: domeView];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onEndGesture:) name: kDomeViewEndGestureNotification object: domeView];

    // Shape view observers
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onSpanDataChanged:) name: kShapeViewDataChangedNotification object: shapeView];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onBeginSpanGesture:) name: kShapeViewBeginGestureNotification object: shapeView];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onEndSpanGesture:) name: kShapeViewEndGestureNotification object: shapeView];

    // Shape view observers
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onGainChanged:) name: kGainChangedNotification object: gainSlider];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onBeginGainGesture:) name: kGainBeginGestureNotification object: gainSlider];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(onEndGainGesture:) name: kGainEndGestureNotification object: gainSlider];
    
    mAU = inAU;
    
    CAStreamBasicDescription desc;
    UInt32 descSize = sizeof(desc);
    AudioUnitGetProperty(mAU, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 0, &desc, &descSize);
    
    [self setChannelCount:desc.mChannelsPerFrame];
    
	// add new listeners
	[self registerAUListeners];
    
    [self refreshView];
}

- (void)setChannelCount:(UInt32)count
{
    isStereo = (count != 1);
    
    [stereoSides setHidden:!isStereo];
    
    [channelMenu removeAllItems];
    
    if (!isStereo)
    {
        for (int i = 1; i <= DEVICE_NUM_CHANNELS; i += 1)
        {
            [channelMenu addItemWithTitle: [NSString stringWithFormat:@"%d", i]];
        }
    }
    else
    {
        for (int i = 1; i <= DEVICE_NUM_CHANNELS; i += 2)
        {
            [channelMenu addItemWithTitle: [NSString stringWithFormat:@"%d-%d", i, i+1]];
        }
    }
}

- (void)drawRect:(NSRect)rect
{
	[mBackgroundColor set];
	NSRectFill(rect);
    
	[super drawRect: rect];
}

#pragma mark ____ INTERFACE ACTIONS ____

#pragma mark ____ Field changes ____
- (IBAction) azimuthChanged:(id)sender
{
	float floatValue = [sender floatValue];
	AudioUnitParameter azimuthParameter = {mAU, Zirkalloy::pack(kZirkalloyParam_Azimuth, mActiveChannel), kAudioUnitScope_Global, 0 };
	
	NSAssert(	AUParameterSet(mAUEventListener, sender, &azimuthParameter, (Float32)floatValue, 0) == noErr,
                @"[Zirkalloy_UIView azimuthChanged:] AUParameterSet()");
}

- (IBAction) zenithChanged:(id)sender
{
	float floatValue = [sender floatValue];
	AudioUnitParameter zenithParameter = {mAU, Zirkalloy::pack(kZirkalloyParam_Zenith, mActiveChannel), kAudioUnitScope_Global, 0 };

	NSAssert(	AUParameterSet(mAUEventListener, sender, &zenithParameter, (Float32)floatValue, 0) == noErr,
                @"[Zirkalloy_UIView zenithChanged:] AUParameterSet()");
}

- (IBAction) azimuthSpanChanged:(id)sender
{
	float floatValue = [sender floatValue];
	AudioUnitParameter azimuthSpanParameter = {mAU, Zirkalloy::pack(kZirkalloyParam_AzimuthSpan, mActiveChannel), kAudioUnitScope_Global, 0 };
	
	NSAssert(	AUParameterSet(mAUEventListener, sender, &azimuthSpanParameter, (Float32)floatValue, 0) == noErr,
             @"[Zirkalloy_UIView azimuthChanged:] AUParameterSet()");
}

- (IBAction) zenithSpanChanged:(id)sender
{
	float floatValue = [sender floatValue];
	AudioUnitParameter zenithSpanParameter = {mAU, Zirkalloy::pack(kZirkalloyParam_ZenithSpan, mActiveChannel), kAudioUnitScope_Global, 0 };
    
	NSAssert(	AUParameterSet(mAUEventListener, sender, &zenithSpanParameter, (Float32)floatValue, 0) == noErr,
             @"[Zirkalloy_UIView zenithChanged:] AUParameterSet()");
}

- (IBAction) gainChanged:(id)sender
{
	float floatValue = [sender floatValue];
	AudioUnitParameter gainParameter = {mAU, Zirkalloy::pack(kZirkalloyParam_Gain, mActiveChannel), kAudioUnitScope_Global, 0 };
    
	NSAssert(	AUParameterSet(mAUEventListener, sender, &gainParameter, (Float32)floatValue, 0) == noErr,
             @"[Zirkalloy_UIView zenithChanged:] AUParameterSet()");
}


- (IBAction) channelChanged:(id)sender
{
    int channelIndex = [sender indexOfSelectedItem];
    
    if (isStereo)
        channelIndex *= 2;
    
    channelIndex += 1;
    
    AudioUnitParameter channelParameter = {mAU, Zirkalloy::pack(kZirkalloyParam_Channel, 0), kAudioUnitScope_Global, 0 };
    
	NSAssert(	AUParameterSet(mAUEventListener, sender, &channelParameter, (Float32)channelIndex, 0) == noErr,
             @"[Zirkalloy_UIView channelChanged:] AUParameterSet()");

    if (isStereo)
    {
        channelParameter.mParameterID = Zirkalloy::pack(kZirkalloyParam_Channel, 1);
        
        NSAssert(	AUParameterSet(mAUEventListener, sender, &channelParameter, (Float32)channelIndex + 1, 0) == noErr,
                 @"[Zirkalloy_UIView channelChanged:] AUParameterSet()");
    }
}

- (IBAction) activeChannelSelected:(id) sender
{
    mActiveChannel = [sender selectedSegment];
    [self refreshView];
}

#pragma mark ____ Dome view actions ___
- (void) onDataChanged:(NSNotification *) aNotification
{
    AudioUnitParameter azimuthParameter = { mAU, Zirkalloy::pack(kZirkalloyParam_Azimuth, mActiveChannel), kAudioUnitScope_Global, 0 };
    AudioUnitParameter zenithParameter  = { mAU, Zirkalloy::pack(kZirkalloyParam_Zenith, mActiveChannel),  kAudioUnitScope_Global, 0 };
    
	NSAssert(	AUParameterSet(mAUEventListener, azimuthField, &azimuthParameter, (Float32)domeView.azimuth, 0) == noErr,
                @"[Zirkalloy_UIView azimuthChanged:] AUParameterSet()");

	NSAssert(	AUParameterSet(mAUEventListener, zenithField, &zenithParameter, (Float32)domeView.zenith, 0) == noErr,
                @"[Zirkalloy_UIView zenithChanged:] AUParameterSet()");
}

- (void) onBeginGesture:(NSNotification *) aNotification 
{
	AudioUnitEvent event;
	AudioUnitParameter parameter = {mAU, Zirkalloy::pack(kZirkalloyParam_Azimuth, mActiveChannel), kAudioUnitScope_Global, 0 };
	event.mArgument.mParameter = parameter;
	event.mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
	
	AUEventListenerNotify (mAUEventListener, self, &event);
		
	event.mArgument.mParameter.mParameterID = Zirkalloy::pack(kZirkalloyParam_Zenith, mActiveChannel);
	AUEventListenerNotify (mAUEventListener, self, &event);
}

- (void) onEndGesture:(NSNotification *) aNotification
{
	AudioUnitEvent event;
	AudioUnitParameter parameter = {mAU, Zirkalloy::pack(kZirkalloyParam_Azimuth, mActiveChannel), kAudioUnitScope_Global, 0 };
	event.mArgument.mParameter = parameter;
	event.mEventType = kAudioUnitEvent_EndParameterChangeGesture;
	
	AUEventListenerNotify (mAUEventListener, self, &event);
	
	event.mArgument.mParameter.mParameterID = Zirkalloy::pack(kZirkalloyParam_Zenith, mActiveChannel);
	AUEventListenerNotify (mAUEventListener, self, &event);	
}

#pragma mark ____ Shape view actions ____
- (void) onSpanDataChanged:(NSNotification *) aNotification
{    
    AudioUnitParameter azimuthSpanParameter = { mAU, Zirkalloy::pack(kZirkalloyParam_AzimuthSpan, mActiveChannel), kAudioUnitScope_Global, 0 };
    AudioUnitParameter zenithSpanParameter  = { mAU, Zirkalloy::pack(kZirkalloyParam_ZenithSpan, mActiveChannel),  kAudioUnitScope_Global, 0 };
    
	NSAssert(	AUParameterSet(mAUEventListener, azimuthField, &azimuthSpanParameter, (Float32)shapeView.azimuthSpan, 0) == noErr,
             @"[Zirkalloy_UIView azimuthChanged:] AUParameterSet()");
    
	NSAssert(	AUParameterSet(mAUEventListener, zenithField, &zenithSpanParameter, (Float32)shapeView.zenithSpan, 0) == noErr,
             @"[Zirkalloy_UIView zenithChanged:] AUParameterSet()");
}

- (void) onBeginSpanGesture:(NSNotification *) aNotification 
{
	AudioUnitEvent event;
	AudioUnitParameter parameter = {mAU, Zirkalloy::pack(kZirkalloyParam_AzimuthSpan, mActiveChannel), kAudioUnitScope_Global, 0 };
	event.mArgument.mParameter = parameter;
	event.mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
	
	AUEventListenerNotify (mAUEventListener, self, &event);
    
	event.mArgument.mParameter.mParameterID = Zirkalloy::pack(kZirkalloyParam_ZenithSpan, mActiveChannel);
	AUEventListenerNotify (mAUEventListener, self, &event);
}

- (void) onEndSpanGesture:(NSNotification *) aNotification
{
	AudioUnitEvent event;
	AudioUnitParameter parameter = {mAU, Zirkalloy::pack(kZirkalloyParam_AzimuthSpan, mActiveChannel), kAudioUnitScope_Global, 0 };
	event.mArgument.mParameter = parameter;
	event.mEventType = kAudioUnitEvent_EndParameterChangeGesture;
	
	AUEventListenerNotify (mAUEventListener, self, &event);
	
	event.mArgument.mParameter.mParameterID = Zirkalloy::pack(kZirkalloyParam_ZenithSpan, mActiveChannel);
	AUEventListenerNotify (mAUEventListener, self, &event);	
}

#pragma mark ____ Gain slider actions ____
- (void) onGainChanged:(NSNotification *) aNotification
{    
    AudioUnitParameter gainParameter = { mAU, Zirkalloy::pack(kZirkalloyParam_Gain, mActiveChannel), kAudioUnitScope_Global, 0 };
    
	NSAssert(	AUParameterSet(mAUEventListener, azimuthField, &gainParameter, (Float32)gainSlider.floatValue, 0) == noErr,
             @"[Zirkalloy_UIView azimuthChanged:] AUParameterSet()");
}

- (void) onBeginGainGesture:(NSNotification *) aNotification 
{
	AudioUnitEvent event;
	AudioUnitParameter parameter = {mAU, Zirkalloy::pack(kZirkalloyParam_Gain, mActiveChannel), kAudioUnitScope_Global, 0 };
	event.mArgument.mParameter = parameter;
	event.mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
	
	AUEventListenerNotify (mAUEventListener, self, &event);
}

- (void) onEndGainGesture:(NSNotification *) aNotification
{
	AudioUnitEvent event;
	AudioUnitParameter parameter = {mAU, Zirkalloy::pack(kZirkalloyParam_Gain, mActiveChannel), kAudioUnitScope_Global, 0 };
	event.mArgument.mParameter = parameter;
	event.mEventType = kAudioUnitEvent_EndParameterChangeGesture;
	
	AUEventListenerNotify (mAUEventListener, self, &event);
	
	event.mArgument.mParameter.mParameterID = kZirkalloyParam_ZenithSpan;
	AUEventListenerNotify (mAUEventListener, self, &event);	
}

void addParamListener (AUEventListenerRef listener, void* refCon, AudioUnitEvent *inEvent)
{
	inEvent->mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
	verify_noerr ( AUEventListenerAddEventType(	listener, refCon, inEvent));
	
	inEvent->mEventType = kAudioUnitEvent_EndParameterChangeGesture;
	verify_noerr ( AUEventListenerAddEventType(	listener, refCon, inEvent));
	
	inEvent->mEventType = kAudioUnitEvent_ParameterValueChange;
	verify_noerr ( AUEventListenerAddEventType(	listener, refCon, inEvent));	
}

#pragma mark ____ PRIVATE FUNCTIONS ____
- (void)registerAUListeners 
{
	if (mAU)
    {
		verify_noerr( AUEventListenerCreate(EventListenerDispatcher, self,
											CFRunLoopGetCurrent(), kCFRunLoopDefaultMode, 0.01, 0.1, 
											&mAUEventListener));
		
        for (int i = 0; i < 2; i++)
        {
            AudioUnitEvent auEvent;
            AudioUnitParameter parameter = {mAU, Zirkalloy::pack(kZirkalloyParam_Azimuth, i), kAudioUnitScope_Global, 0 };
            auEvent.mArgument.mParameter = parameter;
                
            addParamListener (mAUEventListener, self, &auEvent);
            
            auEvent.mArgument.mParameter.mParameterID = Zirkalloy::pack(kZirkalloyParam_Zenith, i);
            addParamListener (mAUEventListener, self, &auEvent);
            
            auEvent.mArgument.mParameter.mParameterID = Zirkalloy::pack(kZirkalloyParam_AzimuthSpan, i);
            addParamListener (mAUEventListener, self, &auEvent);
            
            auEvent.mArgument.mParameter.mParameterID = Zirkalloy::pack(kZirkalloyParam_ZenithSpan, i);
            addParamListener (mAUEventListener, self, &auEvent);

            auEvent.mArgument.mParameter.mParameterID = Zirkalloy::pack(kZirkalloyParam_Gain, i);
            addParamListener (mAUEventListener, self, &auEvent);
        
            auEvent.mArgument.mParameter.mParameterID = Zirkalloy::pack(kZirkalloyParam_Channel, i);
            addParamListener (mAUEventListener, self, &auEvent);
        }
	}
}

- (void)unregisterAUListeners 
{
	if (mAUEventListener) verify_noerr (AUListenerDispose(mAUEventListener));
	mAUEventListener = NULL;
	mAU = NULL;
}

- (void)refreshView
{
    if (mAU)
    {
        Float32 azimuth, zenith;
        Float32 azimuthSpan, zenithSpan;
        Float32 gain;
        Float32  channel;
        
        NSAssert (	AudioUnitGetParameter(mAU, Zirkalloy::pack(kZirkalloyParam_Azimuth, mActiveChannel), kAudioUnitScope_Global, 0, &azimuth) == noErr,
                  @"[Zirkalloy_UIView refreshView] (x.1)");
        
        NSAssert (	AudioUnitGetParameter(mAU, Zirkalloy::pack(kZirkalloyParam_Zenith, mActiveChannel), kAudioUnitScope_Global, 0, &zenith) == noErr,
                  @"[Zirkalloy_UIView refreshView] (x.1)");
        
        NSAssert (	AudioUnitGetParameter(mAU, Zirkalloy::pack(kZirkalloyParam_AzimuthSpan, mActiveChannel), kAudioUnitScope_Global, 0, &azimuthSpan) == noErr,
                  @"[Zirkalloy_UIView refreshView] (x.1)");
        
        NSAssert (	AudioUnitGetParameter(mAU, Zirkalloy::pack(kZirkalloyParam_ZenithSpan, mActiveChannel), kAudioUnitScope_Global, 0, &zenithSpan) == noErr,
                  @"[Zirkalloy_UIView refreshView] (x.1)");
        
        NSAssert (	AudioUnitGetParameter(mAU, Zirkalloy::pack(kZirkalloyParam_Gain, mActiveChannel), kAudioUnitScope_Global, 0, &gain) == noErr,
                  @"[Zirkalloy_UIView refreshView] (x.1)");
        
        NSAssert (	AudioUnitGetParameter(mAU, Zirkalloy::pack(kZirkalloyParam_Channel, mActiveChannel), kAudioUnitScope_Global, 0, &channel) == noErr,
                  @"[Zirkalloy_UIView refreshView] (x.1)");
        
        [domeView setAzimuth:               azimuth];
        [azimuthField setFloatValue:        azimuth];
        
        [domeView setZenith:                zenith];
        [zenithField setFloatValue:         zenith];
        
        [domeView setAzimuthSpan:           azimuthSpan];
        [shapeView setAzimuthSpan:          azimuthSpan];
        [azimuthSpanField setFloatValue:    azimuthSpan];
        
        [domeView setZenithSpan:            zenithSpan];
        [shapeView setZenithSpan:           zenithSpan];
        [zenithSpanField setFloatValue:     zenithSpan];
        
        [gainSlider setFloatValue:          gain];
        [gainField setFloatValue:           gain];
        
        int channelIndex = (int)channel-1;
        if (isStereo)
            channelIndex /= 2;
        
        [channelMenu selectItemAtIndex: channelIndex];
        
        [self setNeedsDisplay:YES];
    }
}

#pragma mark ____ LISTENER CALLBACK DISPATCHEE ____
- (void)eventListener:(void *) inObject event:(const AudioUnitEvent *)inEvent value:(Float32)inValue
{
    // Drop events occurring from inactive side:
    if (Zirkalloy::sunpack(inEvent->mArgument.mParameter.mParameterID) != mActiveChannel)
        return;
    
	switch (inEvent->mEventType)
    {
		case kAudioUnitEvent_ParameterValueChange:
			switch (Zirkalloy::punpack(inEvent->mArgument.mParameter.mParameterID))
            {
				case kZirkalloyParam_Azimuth:
					[azimuthField setFloatValue: inValue];
                    [domeView setAzimuth: inValue];
                    [domeView setNeedsDisplay:YES];
                    break;
				case kZirkalloyParam_Zenith:
					[zenithField setFloatValue: inValue];
                    [domeView setZenith: inValue];
                    [domeView setNeedsDisplay:YES];
					break;
                case kZirkalloyParam_AzimuthSpan:
                    [azimuthSpanField setFloatValue: inValue];
                    [shapeView setAzimuthSpan: inValue];
                    [domeView setAzimuthSpan: inValue];
                    [shapeView setNeedsDisplay:YES];
                    [domeView setNeedsDisplay:YES];
                    break;
                case kZirkalloyParam_ZenithSpan:
                    [zenithSpanField setFloatValue: inValue];
                    [shapeView setZenithSpan: inValue];
                    [domeView setZenithSpan: inValue];
                    [shapeView setNeedsDisplay:YES];
                    [domeView setNeedsDisplay:YES];
                    break;
                case kZirkalloyParam_Gain:
                    [gainSlider setFloatValue: inValue];
                    [gainField setFloatValue: inValue];
                    break;
                case kZirkalloyParam_Channel:
                {
                    int channelIndex = (unsigned int)inValue - 1;
                    if (isStereo)
                        channelIndex /= 2;
                    [channelMenu selectItemAtIndex: channelIndex];
                    [domeView setNeedsDisplay:YES];
                    [shapeView setNeedsDisplay:YES];
                    break;
                }
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

/* If we get a mouseDown, that means it was not in the graph view, or one of the text fields. 
   In this case, we should make the window the first responder. This will deselect our text fields if they are active. */
- (void) mouseDown: (NSEvent *) theEvent
{
	[super mouseDown: theEvent];
	[[self window] makeFirstResponder: self];
}

- (BOOL) acceptsFirstResponder 
{
	return YES;
}

- (BOOL) becomeFirstResponder
{	
	return YES;
}

- (BOOL) isOpaque
{
	return YES;
}

@end
