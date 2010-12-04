#include "vSoundSource.h"


// *****************************************************************************
vSoundSource::vSoundSource(std::string nodeID) : vBaseNode(nodeID)
{

	busID = -1;
}

vSoundSource::~vSoundSource()
{
	// destructor
}

void vSoundSource::debugPrint()
{
	vBaseNode::debugPrint();

	std::cout << "    busID\t" << busID << std::endl;
}


void vSoundSource::setBus(int bus)
{
	busID = bus;

}
