#include "vListener.h"
#include "vAudioManager.h"
#include "vSoundConn.h"

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

    vAudioManager::connIterator c;
	std::cout << "    listen to:\t";
	for (c = connectFROM_.begin(); c != connectFROM_.end(); ++c)
	{
		std::cout << (*c)->src->getID() << " ";
	}
	if (connectFROM_.empty())
        std::cout << "<NO CONNECTIONS>";
	std::cout << std::endl;
}
