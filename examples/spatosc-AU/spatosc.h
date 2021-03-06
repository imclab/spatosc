/*
*	File:		spatosc.h
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

#include "AUEffectBase.h"
#include "AudioToolbox/AudioUnitUtilities.h"
#include "spatoscVersion.h"
#include "lo/lo.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __spatosc_h__
#define __spatosc_h__


#pragma mark ____spatosc Parameters

typedef struct PolarAngles
{
    Float32     azimuth;
    Float32     zenith;
} PolarAngles;

// parameters
enum {
	//Add your parameters here...
	kSpatosc_Gain =0,
	kSpatosc_Azim =1,
	kSpatosc_Elev =2,
	kSpatosc_Dist =3,
	kNumberOfParameters=4
	/*
	kSpatosc_AzimSpan =4,
	kSpatosc_ElevSpan =5,
	kNumberOfParameters=6
	 */
};

const CFStringRef kSpatosc_Gain_Name = CFSTR("Gain");
const float kSpatosc_Gain_Min = 0.0f;
const float kSpatosc_Gain_Max = 1.0f;
const float kSpatosc_Gain_Def = 0.75f;

const CFStringRef kSpatosc_Azim_Name = CFSTR("Azimuth");
const float kSpatosc_Azim_Min = -180.0f;
const float kSpatosc_Azim_Max = 180.0f;
const float kSpatosc_Azim_Def = 0.0f;

const CFStringRef kSpatosc_Elev_Name = CFSTR("Elevation");
const float kSpatosc_Elev_Min = 0.0f;
const float kSpatosc_Elev_Max = 90.0f;
const float kSpatosc_Elev_Def = 0.0f;

const CFStringRef kSpatosc_Dist_Name = CFSTR("Distance");
const float kSpatosc_Dist_Min = 0.0f;
const float kSpatosc_Dist_Max = 1000.0f;
const float kSpatosc_Dist_Def = 1.0f;


// NEW stuff
/*
const CFStringRef kSpatosc_AzimSpan_Name = CFSTR("Asimuth Span");
const float kSpatosc_AzimSpan_Min = 0.0f;
const float kSpatosc_AzimSpan_Max = kSpatosc_Azim_Max * 2.0;
const float kSpatosc_AzimSpan_Def = 0.0f;

const CFStringRef kSpatosc_ElevSpan_Name = CFSTR("Elevation Span");
const float kSpatosc_ElevSpan_Min = 0.0f;
const float kSpatosc_ElevSpan_Max = 90.0f;
const float kSpatosc_ElevSpan_Def = 0.0f;
*/

// OLD stuff (replace with NEW):
const float kMin_AzimuthSpan = 0.0f;
const float kMax_AzimuthSpan = kSpatosc_Azim_Max * 2;
const float kDefault_AzimuthSpan = 0.0f;

const float kMin_ZenithSpan = 0.0f;
const float kMax_ZenithSpan = 90.0f;
const float kDefault_ZenithSpan = 0.0f;






#pragma mark ____spatosc
class spatosc : public AUEffectBase
{
public:
	spatosc(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~spatosc () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new spatoscKernel(this); }
	
	virtual	OSStatus			GetParameterValueStrings(AudioUnitScope			inScope,
														 AudioUnitParameterID		inParameterID,
														 CFArrayRef *			outStrings);
    
	virtual	OSStatus			GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo);
    
	virtual OSStatus			GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &			outDataSize,
												Boolean	&			outWritable );
	
	virtual OSStatus			GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 		inElement,
											void *			outData);
	
 	virtual	bool				SupportsTail () { return false; }
	
	/*! @method Version */
	virtual OSStatus		Version() { return kspatoscVersion; }
	
	

	
	
protected:
		class spatoscKernel : public AUKernelBase		// most real work happens here
	{
public:
		spatoscKernel(AUEffectBase *inAudioUnit )
		: AUKernelBase(inAudioUnit)
	{
	}
		
		// *Required* overides for the process method for this effect
		// processes one channel of interleaved samples
        virtual void 		Process(	const Float32 	*inSourceP,
										Float32		 	*inDestP,
										UInt32 			inFramesToProcess,
										UInt32			inNumChannels,
										bool			&ioSilence);
		
        virtual void		Reset();
		
		//private: //state variables...
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif