#ifndef __vPlugin_dmitri_H
#define __vPlugin_dmitri_H

#include <lo/lo.h>
#include <string>
#include "vPlugin.h"

#define SPACEMAP_RADIUS 750.0

class vPlugin_dmitri : public vPlugin
{

public:

	vPlugin_dmitri(std::string ip);
	~vPlugin_dmitri();

	void update(vSoundConn *conn);

private:

	lo_address destAddr;
	lo_server lo_serv;

};


#endif
