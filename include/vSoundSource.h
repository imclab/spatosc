#ifndef __vSoundSource_H
#define __vSoundSource_H

#include "vBaseNode.h"

class vSoundSource : virtual public vBaseNode
{

public:
	
	vSoundSource(std::string nodeID);
	~vSoundSource();
	
	void debugPrint();

	void setBus(int bus);
	int getBus() { return busID; }

protected:

	int busID;
	
};

#endif
