#ifndef __vPlugin_dmitri_H
#define __vPlugin_dmitri_H

#include <lo/lo.h>
#include <string>
#include "vPlugin.h"

class vSoundConn;

class vPlugin_dmitri : public vPlugin
{

public:

	vPlugin_dmitri(const std::string &ip);
	~vPlugin_dmitri();

	void update(vSoundConn *conn);

private:

    static const double SPACEMAP_RADIUS;

	lo_address destAddr_;
	lo_server lo_serv_;
};


#endif
