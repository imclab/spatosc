#ifndef __vBaseNode_H
#define __vBaseNode_H

#include <string>
#include <vector>
#include <iostream>
#include "vMath.h"

class vSoundConn;
class vAudioManager;

class vBaseNode
{
	// TODO: remove friend classes and provide real getter methods:
	
	friend class vSoundConn;
	friend class vPlugin;
	friend class vPlugin_dmitri;
	friend class vAudioManager;

public:
	
	vBaseNode(std::string nodeID);
	~vBaseNode();

	std::string getID() { return id; }
	
	virtual void debugPrint();

	virtual void setPosition(double x, double y, double z);
	virtual void setRotation(double pitch, double roll, double yaw);

protected:

	std::string id;
	
	Vector3 pos;
	Vector3 rot;
	
	bool active;
	
	std::vector<vSoundConn*> connectTO;
	std::vector<vSoundConn*> connectFROM;
	
	bool updateFlag;
	
};


#endif
