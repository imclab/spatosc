#include "vAudioManager.h"
#include "vBaseNode.h"
#include "vSoundConn.h"

// *****************************************************************************
vBaseNode::vBaseNode(std::string nodeID) {
	
	this->id = nodeID;
	this->active = true;
	this->updateFlag = true;
	
	connectTO.clear();
	connectFROM.clear();
}

vBaseNode::~vBaseNode()
{
	// destructor
}

void vBaseNode::debugPrint()
{
	std::cout << "  " << id << ":" << std::endl;
	std::cout << "    pos:\t" << pos.x << "," << pos.y << "," << pos.z << std::endl;
	std::cout << "    rot:\t" << rot.x << "," << rot.y << "," << rot.z << std::endl;
	std::cout << "    active?\t" << active << std::endl;
}

void vBaseNode::setPosition(double x, double y, double z)
{
	pos = Vector3(x,y,z);
	updateFlag = true;
	vAudioManager::Instance().update(this);
}

void vBaseNode::setRotation(double pitch, double roll, double yaw)
{
	rot = Vector3(pitch,roll,yaw);
	updateFlag=true;
	vAudioManager::Instance().update(this);
}


