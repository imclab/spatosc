#ifndef __vListener_H
#define __vListener_H

#include "vBaseNode.h"

class vListener : public vBaseNode
{
	
public:
	
	vListener(std::string nodeID);
	~vListener();
	
	void debugPrint();
};


#endif
