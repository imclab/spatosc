#include "vSoundConn.h"

// *****************************************************************************

vSoundConn::vSoundConn(vBaseNode *src, vBaseNode *snk)
{
	id = src->id_ + "-" + snk->id_ + ".conn";
	this->src = src;
	this->snk = snk;
	distanceEffect = 100.0;
	rolloffEffect = 100.0;
	dopplerEffect = 100.0;
	diffractionEffect = 100.0;
	
	// set updateFlag on at least one of the nodes for initial computation:
	src->updateFlag_ = true;
}


vSoundConn::~vSoundConn()
{
	// destructor
}
