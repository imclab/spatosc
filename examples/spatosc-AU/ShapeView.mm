#import "Zirkalloy_ShapeView.h"

@interface Zirkalloy_ShapeView (ShapeViewPrivate)
-(void)updateCoords:(NSPoint)pos;
-(void)updateCoordsWithLockedAzimuth:(NSPoint)pos;
-(void)updateCoordsWithLockedZenith:(NSPoint)pos;

-(void)drawCrosshairs;

-(NSPoint)shapeToScreen:(NSPoint)sh;
-(NSPoint)screenToShape:(NSPoint)sc;
@end

static const float kPolarRadius = 1.0f;
static const float kDotRadius = 5.0f;

@implementation Zirkalloy_ShapeView

NSString *kShapeViewDataChangedNotification = @"Zirkalloy_ShapeViewDataChangedNotification";
NSString *kShapeViewBeginGestureNotification = @"Zirkalloy_ShapeViewBeginGestureNotification";
NSString *kShapeViewEndGestureNotification = @"Zirkalloy_ShapeViewEndGestureNotification";

- (id)initWithFrame:(NSRect)frameRect
{
	if ((self = [super initWithFrame:frameRect]) != nil)
    {
        float w;
        float h;
        // Aspect ratio
        if (frameRect.size.width > frameRect.size.height)
        {
            w = h = frameRect.size.height;
        }
        else
        {
            w = h = frameRect.size.width;
        }
        
        mShapeFrame = NSMakeRect(0.0f, 0.0f, w, h);
        
        mCentre.x = frameRect.size.width / 2;
        mCentre.y = frameRect.size.height / 2;
        
		[self setPostsFrameChangedNotifications: YES];
	}
	return self;
}

-(void) dealloc
{	
    if (mBackgroundCache)
    {
        [mBackgroundCache release];
        mBackgroundCache = NULL;
	}
    
	[super dealloc];
}

- (void)drawRect:(NSRect)rect
{    
    NSBezierPath * path;
    NSPoint centre = { mShapeFrame.size.width / 2, mShapeFrame.size.height / 2 };
    
    if (!mBackgroundCache)
    {
        mBackgroundCache = [[NSImage alloc] initWithSize: [self frame].size];

//        NSString *filepath = [[NSBundle bundleForClass: [Zirkalloy_ShapeView class]] pathForImageResource: @"bgShape"];
//        mBackgroundCache = [[NSImage alloc] initByReferencingFile: filepath];     
        
		[mBackgroundCache lockFocus];
        
        [[NSColor whiteColor] setFill];
        [[NSColor blackColor] setStroke];
        
        path = [NSBezierPath bezierPathWithRect:rect];
        [path fill];
        [path stroke];
        
        NSRect centreDot = { { centre.x - kDotRadius / 2, centre.y - kDotRadius / 2 },
            { kDotRadius, kDotRadius } };
        path = [NSBezierPath bezierPathWithOvalInRect:centreDot];
        [[NSColor blackColor] setFill];
        [path fill];
        
        [mBackgroundCache unlockFocus];
    }
    
    [mBackgroundCache drawInRect: rect fromRect: rect operation: NSCompositeSourceOver fraction: 1.0];
    
    NSPoint screenOffset = [self shapeToScreen:mShapeSize];
    NSRect span = { { mCentre.x - screenOffset.x, mCentre.y - screenOffset.y },
                    { screenOffset.x * 2, screenOffset.y * 2 } };
    
    path = [NSBezierPath bezierPathWithRect:span];
    [[NSColor grayColor] setFill];
    [path fill];
    [path stroke];
    
    [self drawCrosshairs];
}

-(void) drawCrosshairs
{
    NSBezierPath * xAxis = [NSBezierPath bezierPath];
    NSBezierPath * yAxis = [NSBezierPath bezierPath];
    
    [xAxis moveToPoint: NSMakePoint(mShapeFrame.origin.x, mShapeFrame.origin.y + mShapeFrame.size.height / 2)];
    [xAxis lineToPoint: NSMakePoint(mShapeFrame.origin.x + mShapeFrame.size.width, mShapeFrame.origin.y + mShapeFrame.size.height / 2)];
    
    [yAxis moveToPoint: NSMakePoint(mShapeFrame.origin.x + mShapeFrame.size.width / 2, mShapeFrame.origin.y)];
    [yAxis lineToPoint: NSMakePoint(mShapeFrame.origin.x + mShapeFrame.size.width / 2, mShapeFrame.origin.y + mShapeFrame.size.height)];
    
    [[[NSColor blackColor] colorWithAlphaComponent:0.5f] setStroke];
    [xAxis stroke];
    [yAxis stroke];
}


#pragma mark ___ Events ___
-(void) mouseDown:(NSEvent *)e
{
	NSPoint mouseLoc = [self convertPoint:[e locationInWindow] fromView:nil];
	mMouseDown = YES;
	
	[[NSNotificationCenter defaultCenter] postNotificationName: kShapeViewBeginGestureNotification object:self];
    
    if ([e modifierFlags] & NSShiftKeyMask)
    {
        [self updateCoordsWithLockedZenith: mouseLoc];
    }
    else if ([e modifierFlags] & NSAlternateKeyMask)
    {
        [self updateCoordsWithLockedAzimuth: mouseLoc];
    }
    else
    {
        [self updateCoords: mouseLoc];
	}
	
	[self setNeedsDisplay:YES];	// update the display of the crosshairs
}

- (void)mouseDragged:(NSEvent *)e
{
	NSPoint mouseLoc = [self convertPoint:[e locationInWindow] fromView:nil];
	mMouseDown = YES;

    if ([e modifierFlags] & NSShiftKeyMask)
    {
        [self updateCoordsWithLockedZenith: mouseLoc];
    }
    else if ([e modifierFlags] & NSAlternateKeyMask)
    {
        [self updateCoordsWithLockedAzimuth: mouseLoc];
    }
    else
    {
        [self updateCoords: mouseLoc];
	}
}

- (void)mouseUp:(NSEvent *)e
{
	mMouseDown = NO;
	[[NSNotificationCenter defaultCenter] postNotificationName: kShapeViewEndGestureNotification object:self];
    
	[self setNeedsDisplay:YES];
}

-(void) handleBeginGesture
{
	mMouseDown = YES;
	[self setNeedsDisplay: YES];
}

-(void) handleEndGesture
{
	mMouseDown = NO;
	[self setNeedsDisplay: YES];
}

#pragma mark ___ Properties ___
-(void)setAzimuthSpan:(float)azimuthSpan
{
    mShapeSize.x = azimuthSpan / kMax_AzimuthSpan;
}

-(float)azimuthSpan
{
    return mShapeSize.x * kMax_AzimuthSpan;
}

-(void)setZenithSpan:(float)zenithSpan
{
    mShapeSize.y = zenithSpan / kMax_ZenithSpan;
}

-(float)zenithSpan
{
    return mShapeSize.y * kMax_ZenithSpan;
}

#pragma mark ___ Private ___
-(void)updateCoords:(NSPoint)pos
{
    mShapeSize = [self screenToShape:pos];
    
    // Clamp min/max
    if (mShapeSize.x > 1.0f)
        mShapeSize.x = 1.0f;
    if (mShapeSize.y > 1.0f)
        mShapeSize.y = 1.0f;
    
    [[NSNotificationCenter defaultCenter] postNotificationName: kShapeViewDataChangedNotification object:self];
}

-(void)updateCoordsWithLockedAzimuth:(NSPoint)pos
{
    float azimuthCopy = mShapeSize.x;
    
    mShapeSize = [self screenToShape:pos];
    mShapeSize.x = azimuthCopy;
    
    // Clamp min/max
    if (mShapeSize.x > 1.0f)
        mShapeSize.x = 1.0f;
    if (mShapeSize.y > 1.0f)
        mShapeSize.y = 1.0f;
    
    [[NSNotificationCenter defaultCenter] postNotificationName: kShapeViewDataChangedNotification object:self];
}

-(void)updateCoordsWithLockedZenith:(NSPoint)pos
{
    float zenithCopy = mShapeSize.y;
    
    mShapeSize = [self screenToShape:pos];
    mShapeSize.y = zenithCopy;
    
    // Clamp min/max
    if (mShapeSize.x > 1.0f)
        mShapeSize.x = 1.0f;
    if (mShapeSize.y > 1.0f)
        mShapeSize.y = 1.0f;
    
    [[NSNotificationCenter defaultCenter] postNotificationName: kShapeViewDataChangedNotification object:self];
}

-(NSPoint)shapeToScreen:(NSPoint)sh
{
    NSPoint sc;

    sc.x = sh.x * mCentre.x;
    sc.y = sh.y * mCentre.y;
    
    return sc;
}

-(NSPoint)screenToShape:(NSPoint)sc
{
    NSPoint sh;
    
    sh.x = fabsf(sc.x - mCentre.x) / mCentre.x;
    sh.y = fabsf(sc.y - mCentre.y) / mCentre.y;
    
    return sh;
}

@end
