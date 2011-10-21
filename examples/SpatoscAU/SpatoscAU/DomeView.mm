#import "DomeView.h"

@interface DomeView (DomeViewPrivate)
-(void)updateCoords:(NSPoint)pos;
-(void)updateCoordsWithLockedAzimuth:(NSPoint)pos;
-(void)updateCoordsWithLockedZenith:(NSPoint)pos;

-(void)drawSpanArc;
-(void)drawSourcePoint;
-(void)drawWallCircle:(NSRect)rect;
-(void)drawCenterDot;
-(void)drawAzimuthLine;
-(void)drawZenithCircle;
-(void)drawCrosshairs;
-(void)drawCoordLabels:(NSRect)rect;

-(NSPoint)domeToScreen:(NSPoint)d;
-(NSPoint)screenToDome:(NSPoint)s;

-(PolarAngles)pointToPolar:(NSPoint)pos;
-(NSPoint)polarToPoint:(PolarAngles)heading;
@end

static const float kPolarRadius = 1.0f;
static const float kDotRadius = 5.0f;
//static const float kMargin = 6.0f;
static const float kMargin = 0.0f;
static const int kNumDivisions = 12;

@implementation DomeView


NSString *kDomeViewDataChangedNotification = @"DomeViewDataChangedNotification";
NSString *kDomeViewBeginGestureNotification= @"DomeViewBeginGestureNotification";
NSString *kDomeViewEndGestureNotification= @"DomeViewEndGestureNotification";

- (id)initWithFrame:(NSRect)frameRect
{
    //frameRect.origin.x -= kMargin;
    //frameRect.origin.y -= kMargin;
    //frameRect.size.width += kMargin * 2;
    //frameRect.size.height += kMargin * 2;
    
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
        
        mDomeFrame = NSMakeRect(kMargin, kMargin, w - (kMargin * 2), h - (kMargin * 2));
        
        mRadius = (w / 2) - kMargin;
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
    if (!mBackgroundCache)
    {
        mBackgroundCache = [[NSImage alloc] initWithSize: [self frame].size];

		[mBackgroundCache lockFocus];
        
        [self drawWallCircle:rect];
        
        [mBackgroundCache unlockFocus];
    }

    //[mBackgroundCache drawInRect: mDomeFrame fromRect: NSZeroRect operation: NSCompositeSourceOver fraction: 1.0];
     
    [self drawCrosshairs];
    [self drawCenterDot];
    [self drawSpanArc];
    [self drawSourcePoint];
    [self drawAzimuthLine];
    [self drawZenithCircle];
    [self drawCoordLabels:rect];
}



-(void)drawWallCircle:(NSRect)rect
{
    NSBezierPath * path;
    
    [[NSColor whiteColor] setFill];
    [[NSColor blackColor] setStroke];
    
    path = [NSBezierPath bezierPathWithOvalInRect:rect];
    [path fill];
    [path stroke];
}
-(void)drawCenterDot
{
    NSBezierPath * path;
    
    NSRect centreDot = {{ mCentre.x - kDotRadius / 2, mCentre.y - kDotRadius / 2 },{ kDotRadius, kDotRadius }};
    
    path = [NSBezierPath bezierPathWithOvalInRect:centreDot];
    
    [[NSColor redColor] setFill];
    [path fill];   
}
-(void)drawAzimuthLine
{
    NSBezierPath * line = [NSBezierPath bezierPath];   
    
    NSPoint centreDot = NSMakePoint(mCentre.x,mCentre.y);
    
    // Fetch source point
    NSPoint screen = [self domeToScreen:mSourcePoint];
    float screenX = screen.x;
    float screenY = screen.y;
    NSPoint sourceDot = NSMakePoint(screenX,screenY);
    
    [line moveToPoint:centreDot];
    [line lineToPoint:sourceDot];
    
    [[NSColor redColor] setStroke];
    [line stroke];   
}
-(void)drawZenithCircle
{
    NSBezierPath * path;
    
    float dist = sqrt(mSourcePoint.x*mSourcePoint.x + mSourcePoint.y*mSourcePoint.y);
    NSRect rect = {{mCentre.x-(mRadius*dist*2)/2, mCentre.y-(mRadius*dist*2)/2},{mRadius*dist*2,mRadius*dist*2}};
    
    [[NSColor redColor] setStroke];
    
    path = [NSBezierPath bezierPathWithOvalInRect:rect];
    [path stroke];
}
-(void) drawSpanArc
{
    float aDelta = mShapeSize.x * kSpatosc_AzimSpan_Max / 2;
    float zDelta = mShapeSize.y * kSpatosc_ElevSpan_Max / 2;
    
    float middle = mSourceHeading.azimuth + 90.0f;
    float minA = (middle - aDelta);
    float maxA = (middle + aDelta);
    float minA2 = minA;
    float maxA2 = maxA;
    BOOL reverse = NO;
    PolarAngles minZ = mSourceHeading, maxZ = mSourceHeading;
    
    minZ.zenith -= zDelta;
    maxZ.zenith += zDelta;
    
    if (minZ.zenith < kSpatosc_Elev_Min)
    {
        minZ.zenith = kSpatosc_Elev_Min;
        maxZ.zenith = zDelta * 2;
    }
    
    NSPoint pminZ = [self polarToPoint:minZ];
    NSPoint pmaxZ = [self polarToPoint:maxZ];

    float rminZ = sqrt(pminZ.x * pminZ.x + pminZ.y * pminZ.y) * mRadius;
    float rmaxZ = sqrt(pmaxZ.x * pmaxZ.x + pmaxZ.y * pmaxZ.y) * mRadius;
    
    // Check for opposite radii in the event of zenith span surpassing topmost zenith
    if ((pminZ.x * pmaxZ.x + pminZ.y * pmaxZ.y) < 0)
    {
        rmaxZ *= -1.0f;
    }
    
    // Handle high zenith span overlap
    if (mShapeSize.x * kSpatosc_AzimSpan_Max > kSpatosc_ElevSpan_Max / 2 &&
        mShapeSize.y * kSpatosc_ElevSpan_Max / 2 + mSourceHeading.zenith > kSpatosc_Elev_Max)
    {
        maxA = middle - (180 - aDelta);
        minA = middle + (180 - aDelta);
        reverse = YES;
    }
    
    NSBezierPath * path = [NSBezierPath bezierPath];
    
    /*
    printf("mCentre.x:%f,mCentre.y:%f,rmaxZ:%f,rminZ:%f\n",
           mCentre.x,mCentre.y,rmaxZ,rminZ);
    printf("aDelta:%f,zDelta:%f",aDelta,zDelta);
    printf("minA:%f,maxA:%f,maxA2:%f,minA2:%f\n",
           minA,maxA,maxA2,minA2);
    */
     
    [path appendBezierPathWithArcWithCenter:mCentre radius:rmaxZ startAngle:minA endAngle:maxA clockwise: reverse];
    [path appendBezierPathWithArcWithCenter:mCentre radius:rminZ startAngle:maxA2 endAngle:minA2 clockwise: YES];
    [path closePath];
    
    [[NSColor grayColor] setFill];
    [[NSColor blueColor] setStroke];
    [path setLineWidth:2.0f];
    [path stroke];
    [path fill];
}
-(void) drawSourcePoint
{
    // Fetch source point
    NSPoint screen = [self domeToScreen:mSourcePoint];
    float screenX = screen.x - kDotRadius;
    float screenY = screen.y - kDotRadius;
    NSRect sourceDot = { { screenX, screenY },
        { kDotRadius * 2, kDotRadius * 2 } };
    
    NSBezierPath * path = [NSBezierPath bezierPathWithOvalInRect:sourceDot];
    [[NSColor blueColor] setStroke];
    [path setLineWidth: 2.0f];
    [path stroke];
}
-(void) drawCrosshairs
{    
    [[[NSColor blackColor] colorWithAlphaComponent:0.5f] setStroke];
    
    for (int i = 0; i < kNumDivisions; i++)
    {
        float fraction = (i % (kNumDivisions / 4) == 0) ? 0.75f : 0.90f;
        NSBezierPath * line = [NSBezierPath bezierPath];
        float angle = ((float)i / (float)kNumDivisions) * M_PI * 2;
        NSPoint axis = {cos(angle), sin(angle)};
        [line moveToPoint: NSMakePoint(mCentre.x + axis.x * mRadius, mCentre.y + axis.y * mRadius)];
        [line lineToPoint: NSMakePoint(mCentre.x + axis.x * mRadius * fraction, mCentre.y + axis.y * mRadius * fraction)];
        
        [line stroke];
    }
    
    NSBezierPath * xAxis = [NSBezierPath bezierPath];
    NSBezierPath * yAxis = [NSBezierPath bezierPath];
    
    [xAxis moveToPoint: NSMakePoint(mDomeFrame.origin.x, mDomeFrame.origin.y + mDomeFrame.size.height / 2)];
    [xAxis lineToPoint: NSMakePoint(mDomeFrame.origin.x + mDomeFrame.size.width, mDomeFrame.origin.y + mDomeFrame.size.height / 2)];

    [yAxis moveToPoint: NSMakePoint(mDomeFrame.origin.x + mDomeFrame.size.width / 2, mDomeFrame.origin.y)];
    [yAxis lineToPoint: NSMakePoint(mDomeFrame.origin.x + mDomeFrame.size.width / 2, mDomeFrame.origin.y + mDomeFrame.size.height)];
    
    [[[NSColor blackColor] colorWithAlphaComponent:0.5f] setStroke];
    [xAxis stroke];
    [yAxis stroke];
}
-(void) drawCoordLabels:(NSRect)rect
{
    //note we are using the convenience method, so we don't need to autorelease the object
    NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:[NSFont fontWithName:@"Helvetica" size:14], NSFontAttributeName,[NSColor blackColor], NSForegroundColorAttributeName, nil];
    
    NSAttributedString * currentText = [[NSAttributedString alloc] initWithString:@"0" attributes: attributes];
    
    NSSize attrSize = [currentText size];
    
    [currentText drawAtPoint:NSMakePoint(mCentre.x+2,rect.size.height-attrSize.height)];
    
    currentText = [[NSAttributedString alloc] initWithString:@"-90" attributes: attributes];
    [currentText drawAtPoint:NSMakePoint(rect.size.width-attrSize.width,rect.size.height/2)];
}



#pragma mark ___ Events ___
-(void) mouseDown:(NSEvent *)e
{
	NSPoint mouseLoc = [self convertPoint:[e locationInWindow] fromView:nil];
	mMouseDown = YES;
	
	[[NSNotificationCenter defaultCenter] postNotificationName: kDomeViewBeginGestureNotification object:self];
    
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
        if (true) {
            [self updateCoords: mouseLoc];
        }
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
    
	[self setNeedsDisplay:YES];
}

- (void)mouseUp:(NSEvent *)e
{
	mMouseDown = NO;
	[[NSNotificationCenter defaultCenter] postNotificationName: kDomeViewEndGestureNotification object:self];

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
-(void)setSourcePoint:(NSPoint)pos
{
    mSourcePoint = pos;
    mSourceHeading = [self pointToPolar:pos];
}

-(NSPoint)sourcePoint
{
    return mSourcePoint;
}

-(void)setHeading:(PolarAngles)heading
{
    mSourcePoint = [self polarToPoint:heading];
    mSourceHeading = heading;
}

-(PolarAngles)heading
{
    return mSourceHeading;
}

-(void)setAzimuth:(float)azimuth
{
    mSourceHeading.azimuth = azimuth;
    mSourcePoint = [self polarToPoint:mSourceHeading];
}

-(float)azimuth
{
    return mSourceHeading.azimuth;
}

-(void)setZenith:(float)zenith
{
    mSourceHeading.zenith = zenith;
    mSourcePoint = [self polarToPoint:mSourceHeading];
}

-(float)zenith
{
    return mSourceHeading.zenith;
}

#pragma mark ___ Private ___
// Converts a point in screen coordinates to a normalized offset in flattened
// dome view.
-(void)updateCoords:(NSPoint)pos
{
    NSPoint point = [self screenToDome:pos];
    
    float root = kPolarRadius*kPolarRadius - point.x*point.x - point.y*point.y;
    
    [self setSourcePoint:point];
    if (root < 0.0f) {
        [self setZenith:0.0f];
    }
    
    [[NSNotificationCenter defaultCenter] postNotificationName: kDomeViewDataChangedNotification object:self];
}

-(void)updateCoordsWithLockedAzimuth:(NSPoint)pos
{
    float azimuthCopy = [self azimuth];

    // Restrict positions to axis colinear to locked azimuth..
    // First compute unit vector (in dome space) for said axis
    NSPoint axis = mSourcePoint;
    float l = sqrt(axis.x * axis.x + axis.y * axis.y);
    axis.x /= l;
    axis.y /= l;
    
    // Project cursor position (in dome space) onto axis
    NSPoint pPos = [self screenToDome:pos];
    float mag = axis.x * pPos.x + axis.y * pPos.y;
    NSPoint rPos = {mag * axis.x, mag * axis.y};    
    
    // Handle passing zenith 90
    if ((rPos.x * mSourcePoint.x + rPos.y * mSourcePoint.y) < 0)
    {
        azimuthCopy += (azimuthCopy >= 0) ? -180 : 180;
    }
    
    [self setSourcePoint: rPos];
    [self setAzimuth: azimuthCopy];
    
    [[NSNotificationCenter defaultCenter] postNotificationName: kDomeViewDataChangedNotification object:self];
}

-(void)updateCoordsWithLockedZenith:(NSPoint)pos
{
    float zenithCopy = [self zenith];
    NSPoint rPos = [self screenToDome:pos];
    
    [self setSourcePoint: rPos];
    [self setZenith: zenithCopy];
    
    [[NSNotificationCenter defaultCenter] postNotificationName: kDomeViewDataChangedNotification object:self];
}

-(void)setAzimuthSpan:(float)azimuthSpan
{
    mShapeSize.x = azimuthSpan / kSpatosc_AzimSpan_Max;
}

-(float)azimuthSpan
{
    return mShapeSize.x * kSpatosc_AzimSpan_Max;
}

-(void)setZenithSpan:(float)zenithSpan
{
    mShapeSize.y = zenithSpan / kSpatosc_ElevSpan_Max;
}

-(float)zenithSpan
{
    return mShapeSize.y * kSpatosc_ElevSpan_Max;
}

-(NSPoint)domeToScreen:(NSPoint)d
{
    NSPoint s;
    s.x = -d.y * mRadius + mCentre.x;
    s.y = d.x * mRadius + mCentre.y;
    return s;
}

-(NSPoint)screenToDome:(NSPoint)s
{
    NSPoint d;
    d.x = (s.y - mCentre.y) / mRadius;
    d.y = -(s.x - mCentre.x) / mRadius;
    return d;
}

// Converts a point in a flattened dome view to radians / pi coordinates. 
-(PolarAngles)pointToPolar:(NSPoint)pos
{
    PolarAngles ret;
    ret.azimuth = (atan2f(pos.y, pos.x)) / M_PI * 180.0f;
    float root = kPolarRadius * kPolarRadius - pos.x * pos.x - pos.y * pos.y;
    // Rounding mistakes & distance clamping
    if (root < 0.0001f)
        root = 0.0f;
    ret.zenith = asinf(sqrtf(root) / kPolarRadius) / M_PI * 180.0f;
    return ret;
}

// Converts a heading in radians / pi coordinates to an offset in flattened
// dome view.
-(NSPoint)polarToPoint:(PolarAngles)heading
{
    NSPoint ret;
    ret.x = kPolarRadius * cosf(heading.zenith / 180.0f * M_PI) * cosf(heading.azimuth / 180.0f * M_PI);
    ret.y = kPolarRadius * cosf(heading.zenith / 180.0f * M_PI) * sinf(heading.azimuth / 180.0f * M_PI);
    return ret;
}

@end
