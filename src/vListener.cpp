#include "vListener.h"
#include "vAudioManager.h"

// *****************************************************************************
vListener::vListener(std::string nodeID) : vBaseNode(nodeID)
{
	
	
}

vListener::~vListener()
{
	// destructor
}

void vListener::debugPrint()
{
	vBaseNode::debugPrint();

	connIterator c;
	std::cout << "    listen to:\t";
	for (c = connectFROM.begin(); c != connectFROM.end(); c++)
	{
		std::cout << (*c)->src->getID() << " ";
	}
	if (!connectFROM.size()) std::cout << "<NO CONNECTIONS>";
	std::cout << std::endl;
}
