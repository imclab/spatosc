/** @file
 * The vSoundConn class.
 */
#ifndef __vSoundConn_H
#define __vSoundConn_H

#include <string>
class vBaseNode;

/**
 * Audio connection between two nodes.
 */
class vSoundConn
{
private:
	// TODO: get rid of friend classes, and add proper getter methods:
	friend class vAudioManager;
	friend class vPlugin;
	friend class vPlugin_dmitri;
	friend class vListener;

public:
	
	vSoundConn(vBaseNode *src, vBaseNode *snk);
	~vSoundConn();
    /**
     * Update distance, azimuth and elevation values
     */
    void update();

    double distance() const { return distance_; }
    double distanceScalar() const { return distanceScalar_; }
    double azimuth() const { return azim_; }
    double elevation() const { return elev_; }
	
protected:

	std::string id_;
	
	vBaseNode *src_;
	vBaseNode *snk_;
	
    double distance_;
    double distanceScalar_;
    double azim_;
    double elev_;
	float distanceEffect_;
	float rolloffEffect_;
	float dopplerEffect_;
	float diffractionEffect_;
};

#endif
