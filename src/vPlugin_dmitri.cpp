#include "vPlugin_dmitri.h"
#include "OSCutil.h"
#include "vSoundSource.h"
#include "vSoundConn.h"

#include <iostream>

const double vPlugin_dmitri::SPACEMAP_RADIUS = 750.0;

// *****************************************************************************
vPlugin_dmitri::vPlugin_dmitri(const std::string &ip) : vPlugin()
{
	destAddr_ = lo_address_new(ip.c_str(), "18033");
	lo_serv_ = lo_server_new("18099", NULL);

	std::cout << "Sending to D-Mitri on: " << lo_address_get_url(destAddr_) << std::endl;
	std::cout << "Outgoing address is:   " << lo_server_get_url(lo_serv_) << std::endl;
}

vPlugin_dmitri::~vPlugin_dmitri()
{
	// destructor
	lo_server_free(lo_serv_);
	lo_address_free(destAddr_);
}

void vPlugin_dmitri::update(vSoundConn *conn)
{
	std::string str;
	vSoundSource *src = dynamic_cast<vSoundSource*>(conn->src_);

	if (!src) return;
	if (src->getChannelID() < 0) return;

	float r = conn->elevation() / (M_PI/2);
	if (r < 0) r = 0.0; // for now, force sources to be above equator

	float spacemapX = cos(conn->azimuth()) * r * SPACEMAP_RADIUS;
	float spacemapY = sin(conn->azimuth()) * r * SPACEMAP_RADIUS;

	str = "/spacemap/" + OSCutil::stringify(src->getChannelID()) + "/x";
	lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, str.c_str(), "f", spacemapX);

	str = "/spacemap/" + OSCutil::stringify(src->getChannelID()) + "/y";
	lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, str.c_str(), "f", spacemapY);

	// now from distance, compute gain and variable delay:

	//double vdel = distance * (1/SPEED_OF_SOUND) * .01 * conn->dopplerEffect;  // speed of sound
	double gain = 20 * log10(conn->distanceScalar());

	str = "Input " + OSCutil::stringify(src->getChannelID()) + " Level";
	lo_send_from(destAddr_, lo_serv_, LO_TT_IMMEDIATE, "/set", "sf", str.c_str(), gain);
}

std::string vPlugin_dmitri::getTypeString() const
{
    return "DMITIRI";
}
