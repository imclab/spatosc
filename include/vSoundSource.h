/**
 * The vSoundSource class.
 */
#ifndef __vSoundSource_H
#define __vSoundSource_H

#include "vBaseNode.h"

/**
 * Sound source node.
 */
class vSoundSource : virtual public vBaseNode
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
