#include "vSoundConn.h"
#include "vBaseNode.h"

// *****************************************************************************

vSoundConn::vSoundConn(vBaseNode *src, vBaseNode *snk)
{
	id_ = src->id_ + "-" + snk->id_ + ".conn";
	this->src_ = src;
	this->snk_ = snk;
	distanceEffect_ = 100.0;
	rolloffEffect_ = 100.0;
	dopplerEffect_ = 100.0;
	diffractionEffect_ = 100.0;
	
	// set updateFlag on at least one of the nodes for initial computation:
	src->updateFlag_ = true;
}


vSoundConn::~vSoundConn()
{
	// destructor
}
