/*
*	File:		SpatoscAU.cpp
*	
*	Version:	1.0
* 
*	Created:	9/20/11
*	
*	Copyright:  Copyright 2011 __MyCompanyName__. All rights reserved.
* 
*	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc. ("Apple") in 
*				consideration of your agreement to the following terms, and your use, installation, modification 
*				or redistribution of this Apple software constitutes acceptance of these terms.  If you do 
*				not agree with these terms, please do not use, install, modify or redistribute this Apple 
*				software.
*
*				In consideration of your agreement to abide by the following terms, and subject to these terms, 
*				Apple grants you a personal, non-exclusive license, under Apple's Copyrights in this 
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
/*=============================================================================
	SpatoscAU.cpp
	
=============================================================================*/
#include "SpatoscAU.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(SpatoscAU)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SpatoscAU::SpatoscAU
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int SpatoscAU::sourceCount = 0;

SpatoscAU::SpatoscAU(AudioUnit component)
	: AUEffectBase(component)
{
    // only need to do this once (not with every new AU instance).
    // TODO: move this elsewhere
    spatosc::SingletonScene::Instance().addTranslator("basic", new spatosc::BasicTranslator("osc.udp://127.0.0.1:18032"));
    spatosc::SingletonScene::Instance().createListener("listener");
    
    
	CreateElements();
//<<<<<<< HEAD
	Globals()->UseIndexedParameters(kNumberOfParameters);
	//SetParameter(kParam_One, kDefaultValue_ParamOne );
    //SetParameter(kSpatosc_Gain, kSpatosc_Gain_Def );
//=======
	
    // create a unique name for this source:
    std::ostringstream o;
	o << "source" << SpatoscAU::sourceCount++;
    id = o.str();
    source = spatosc::SingletonScene::Instance().createSoundSource(id);
    source->setStringProperty("setMediaURI","plugin://plugins/testnoise~");
    spatosc::SingletonScene::Instance().debugPrint();

    //SetProperty(kSoundSourceID, kAudioUnitScope_Global,<#AudioUnitElement inElement#>, id, sizeof(id));
    
    Globals()->UseIndexedParameters(kNumberOfParameters);
    //SetParameter(kParam_One, kDefaultValue_ParamOne );
    SetParameter(kSpatosc_Gain, kSpatosc_Gain_Def );
//>>>>>>> 3b6b1e5d2f96019bc22eeb181b56f6a8af53875b
	SetParameter(kSpatosc_Azim, kSpatosc_Azim_Def );
	SetParameter(kSpatosc_Elev, kSpatosc_Elev_Def );
	SetParameter(kSpatosc_Dist, kSpatosc_Dist_Def );
	//SetParameter(kSpatosc_AzimSpan, kSpatosc_AzimSpan_Def );
	//SetParameter(kSpatosc_ElevSpan, kSpatosc_ElevSpan_Def );
    
    
    
    channelCount++;
    //NSLog(@"channelCount: %d", channelCount);
    printf("channelCount: %d", channelCount);
        
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SpatoscAU::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			SpatoscAU::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SpatoscAU::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			SpatoscAU::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	OSStatus result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            /*    
            case kSpatosc_Gain:
                AUBase::FillInParameterName (outParameterInfo, kSpatosc_Gain_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
                outParameterInfo.minValue = kSpatosc_Gain_Min;
                outParameterInfo.maxValue = kSpatosc_Gain_Max;
                outParameterInfo.defaultValue = kSpatosc_Gain_Def;
                break;
             */
            case kSpatosc_Azim:
                AUBase::FillInParameterName (outParameterInfo, kSpatosc_Azim_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Degrees;
                outParameterInfo.minValue = kSpatosc_Azim_Min;
                outParameterInfo.maxValue = kSpatosc_Azim_Max;
                outParameterInfo.defaultValue = kSpatosc_Azim_Def;
                break;
            case kSpatosc_Elev:
                AUBase::FillInParameterName (outParameterInfo, kSpatosc_Elev_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Degrees;
                outParameterInfo.minValue = kSpatosc_Elev_Min;
                outParameterInfo.maxValue = kSpatosc_Elev_Max;
                outParameterInfo.defaultValue = kSpatosc_Elev_Def;
                break;
            case kSpatosc_Dist:
                AUBase::FillInParameterName (outParameterInfo, kSpatosc_Dist_Name, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Meters;
                outParameterInfo.minValue = kSpatosc_Dist_Min;
                outParameterInfo.maxValue = kSpatosc_Dist_Max;
                outParameterInfo.defaultValue = kSpatosc_Dist_Def;
                break;
			default:
                result = kAudioUnitErr_InvalidParameter;
                break;
            }
	} else {
        result = kAudioUnitErr_InvalidParameter;
    }
    


	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SpatoscAU::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			SpatoscAU::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	if (inScope == kAudioUnitScope_Global) 
	{
		switch (inID) 
		{
			case kAudioUnitProperty_CocoaUI:
				outWritable = false;
				outDataSize = sizeof (AudioUnitCocoaViewInfo);
				return noErr;
					
		}
	}

	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SpatoscAU::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus			SpatoscAU::GetProperty(	AudioUnitPropertyID inID,
															AudioUnitScope 		inScope,
															AudioUnitElement 	inElement,
															void *				outData )
{
	if (inScope == kAudioUnitScope_Global) 
	{
		switch (inID) 
		{
			case kAudioUnitProperty_CocoaUI:
			{
				// Look for a resource in the main bundle by name and type.
				CFBundleRef bundle = CFBundleGetBundleWithIdentifier( CFSTR("com.audiounit.SpatoscAU") );
				
				if (bundle == NULL) return fnfErr;
                
				CFURLRef bundleURL = CFBundleCopyResourceURL( bundle, 
                    CFSTR("SpatoscAUCocoaViewFactory"), 
                    CFSTR("bundle"), 
                    NULL);
                
                if (bundleURL == NULL) return fnfErr;

				AudioUnitCocoaViewInfo cocoaInfo;
				cocoaInfo.mCocoaAUViewBundleLocation = bundleURL;
				cocoaInfo.mCocoaAUViewClass[0] = CFStringCreateWithCString(NULL, "SpatoscAUCocoaViewFactory", kCFStringEncodingUTF8);
				
				*((AudioUnitCocoaViewInfo *)outData) = cocoaInfo;
				
				return noErr;
			}
		}
	}


	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}


#pragma mark ____SpatoscAUEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SpatoscAU::SpatoscAU
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SpatoscAU::SpatoscAUKernel::SpatoscAUKernel(AUEffectBase *inAudioUnit )
: AUKernelBase(inAudioUnit)
{
    ///using namespace spatosc;
    //Scene scene;    
    //scene.addTranslator("basic", new BasicTranslator("osc.udp//127.0.0.1:18032"));
    //scene.addTranslator("basic", "BasicTranslator", "osc.udp//127.0.0.1:18032");
    //scene.createListener("listener");
    
    //SoundSource *foo = scene.createSoundSource("foo");
    //SoundSource *bar = scene.createSoundSource("bar");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SpatoscAU::SpatoscAUKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		SpatoscAU::SpatoscAUKernel::Reset()
{
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SpatoscAU::SpatoscAUKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		SpatoscAU::SpatoscAUKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, // for version 2 AudioUnits inNumChannels is always 1
                                                    bool			&ioSilence )
{

	//This code will pass-thru the audio data.
	//This is where you want to process data to produce an effect.

	
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
    //Float32 gain = GetParameter( kSpatosc_Gain );
	
	while (nSampleFrames-- > 0) {
		Float32 inputSample = *sourceP;
		
		//The current (version 2) AudioUnit specification *requires* 
	    //non-interleaved format for all inputs and outputs. Therefore inNumChannels is always 1
		
		sourceP += inNumChannels;	// advance to next frame (e.g. if stereo, we're advancing 2 samples);
									// we're only processing one of an arbitrary number of interleaved channels

        // here's where you do your DSP work
        Float32 outputSample = inputSample;
		
		*destP = outputSample;
		destP += inNumChannels;
	}
}
