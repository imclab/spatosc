/** @file
 * The vSoundConn class.
 */
#ifndef __vSoundConn_H
#define __vSoundConn_H

#include "vBaseNode.h"

/**
 * Audio connection between two nodes.
 */
class vSoundConn
{
	// TODO: get rid of friend classes, and add proper getter methods:
	friend class vAudioManager;
	friend class vPlugin;
	friend class vPlugin_dmitri;
	friend class vListener;

public:
	
	vSoundConn(vBaseNode *src, vBaseNode *snk);
	~vSoundConn();
	
protected:

	std::string id;
	
	vBaseNode *src;
	vBaseNode *snk;
	
	float distanceEffect;
	float rolloffEffect;
	float dopplerEffect;
	float diffractionEffect;
	
	
};

#endif
