/** @file
 * The vSoundSource class.
 */
#ifndef __vSoundSource_H
#define __vSoundSource_H

#include "vBaseNode.h"

<<<<<<< HEAD
// FIXME: tmatth Wed Jan 12 16:05:10 EST 2011: why is this virtual?
class vSoundSource : /*virtual*/ public vBaseNode
=======
/**
 * Sound source node.
 */
class vSoundSource : virtual public vBaseNode
>>>>>>> master
{

public:
	
	vSoundSource(const std::string &nodeID);
	~vSoundSource();
	
	void debugPrint();

	void setChannelID(int channel);
	int getChannelID() const { return channelID_; }

protected:

	int channelID_;
	
};

#endif
