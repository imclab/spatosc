#include "vPlugin_dmitri.h"
#include "OSCutil.h"
#include "vSoundSource.h"

#include <iostream>

const double vPlugin_dmitri::SPACEMAP_RADIUS = 750.0;

// *****************************************************************************
vPlugin_dmitri::vPlugin_dmitri(const std::string &ip) : vPlugin()
{
	type = vPlugin::DMITRI;

	destAddr = lo_address_new(ip.c_str(), "18033");
	lo_serv = lo_server_new("18099", NULL);

	std::cout << "Sending to D-Mitri on: " << lo_address_get_url(destAddr) << std::endl;
	std::cout << "Outgoing address is:   " << lo_server_get_url(lo_serv) << std::endl;
}

vPlugin_dmitri::~vPlugin_dmitri()
{
	// destructor
	lo_server_free(lo_serv);
	lo_address_free(destAddr);
}

void vPlugin_dmitri::update(vSoundConn *conn)
{
	std::string str;
	vSoundSource *src = dynamic_cast<vSoundSource*>(conn->src);

	if (!src) return;
	if (src->getChannelID() < 0) return;

	Vector3 vect = conn->snk->pos_ - conn->src->pos_;
	double distance = (double)vect.Mag();
	double azim = atan2(vect.y, vect.x);
	double elev = atan2( sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.z );

	float r = elev / (M_PI/2);
	if (r < 0) r = 0.0; // for now, force sources to be above equator

	float spacemapX = cos(azim) * r * SPACEMAP_RADIUS;
	float spacemapY = sin(azim) * r * SPACEMAP_RADIUS;

	str = "/spacemap/" + OSCutil::stringify(src->getChannelID()) + "/x";
	lo_send_from(destAddr, lo_serv, LO_TT_IMMEDIATE, str.c_str(), "f", spacemapX);

	str = "/spacemap/" + OSCutil::stringify(src->getChannelID()) + "/y";
	lo_send_from(destAddr, lo_serv, LO_TT_IMMEDIATE, str.c_str(), "f", spacemapY);

	// now from distance, compute gain and variable delay:

	double distanceScalar = 1 / (1.0 + pow(distance,(double)conn->distanceEffect*.01));
	//double vdel = distance * (1/SPEED_OF_SOUND) * .01 * conn->dopplerEffect;  // speed of sound
	double gain = 20 * log10(distanceScalar);

	str = "Input " + OSCutil::stringify(src->getChannelID()) + " Level";
	lo_send_from(destAddr, lo_serv, LO_TT_IMMEDIATE, "/set", "sf", str.c_str(), gain);
}
