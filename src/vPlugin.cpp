#include <iostream>

#include "vPlugin.h"
#include "vSoundConn.h"
#include "vBaseNode.h"


vPlugin::vPlugin()
{
	type_ = CONSOLE;
}

vPlugin::~vPlugin()
{
}

std::string vPlugin::getTypeString()
{
    switch (type_)
    {
        case (CONSOLE):
            return "CONSOLE";
            break;
        case (DMITRI):
            return "DMITRI";
            break;
        case (MULTICAST):
            return "MULTICAST";
            break;
        default:
            return "";
    }
}


void vPlugin::update(vSoundConn *conn)
{
    std::cout << "Computation update for " << conn->src_->id_ << " -> " << conn->snk_->id_ << " :" <<std::endl;

    Vector3 vect = conn->snk_->pos_ - conn->src_->pos_;
    double distance = (double)vect.Mag();
    double azim = atan2(vect.y, vect.x);
    double elev = atan2(sqrt(pow(vect.x,2) + pow(vect.y,2)), vect.z) / (M_PI/2);
    if (elev < 0) elev = 0.0; // for now, force sources to be above equator

    double distanceScalar = 1 / (1.0 + pow(distance, (double)conn->distanceEffect_ * 0.01));

    double vdel = distance * (1/SPEED_OF_SOUND) * .01 * conn->dopplerEffect_;  // speed of sound
    double gain = 20 * log10(distanceScalar);

    /*
    // SRC INCIDENCE:
    Vector3 srcDir = EulerToQuat(conn->src->rot) * Vector3(0,1,0);
    double srcIncidence = AngleBetweenVectors(srcDir, vect);
    double srcIncidenceGain = conn->src->rolloff->getValue( (srcIncidence * conn->src->spread) / M_PI );

    // SNK INCIDENCE:
    Vector3 snkDir = EulerToQuat(conn->snk->rot) * Vector3(0,1,0);
    double snkIncidence = AngleBetweenVectors(Vector3(0,0,0)-snkDir, vect);
    double snkIncidenceGain = conn->snk->rolloff->getValue( (snkIncidence * conn->snk->spread) / M_PI );

    // final incidence scalars with connection's rolloffEffect applied:
    double srcScalar = (double) (1.0 - (.01*xconn->rolloffEffect  * (1.0 - srcIncidenceGain)));
    double snkScalar = (double) (1.0 - (.01*xconn->rolloffEffect  * (1.0 - snkIncidenceGain)));
     */

    std::cout << "  dist:\t" << distance << std::endl;
    std::cout << "  azim:\t" << azim << std::endl;
    std::cout << "  elev:\t" << elev << std::endl;

    std::cout << "  gain:\t" << gain << " dB" << std::endl;
    std::cout << "  delay:\t" << vdel << " ms" << std::endl;

}
