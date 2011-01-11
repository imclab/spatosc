#include "vSoundSource.h"


// *****************************************************************************
vSoundSource::vSoundSource(const std::string &nodeID) : vBaseNode(nodeID), channelID_(-1)
{
}

vSoundSource::~vSoundSource()
{
	// destructor
}

void vSoundSource::debugPrint()
{
	vBaseNode::debugPrint();

	std::cout << "    channelID\t" << channelID_ << std::endl;
}


void vSoundSource::setChannelID(int channel)
{
	channelID_ = channel;

}
