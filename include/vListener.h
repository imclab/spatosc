/**
 * The vListener class.
 */
#ifndef __vListener_H
#define __vListener_H

#include "vBaseNode.h"

/**
 * Listener nodes can listen to sound sources.
 * 
 * The listener represents a person that listens to an audio scene. There is usually only one listener in a scene.
 */
class vListener : public vBaseNode
{
public:
	
	vListener(std::string nodeID);
	~vListener();
	
	void debugPrint();
};

#endif

