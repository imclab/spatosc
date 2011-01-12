#include "vBaseNode.h"
#include "vAudioManager.h"
#include "vSoundConn.h"

// *****************************************************************************
vBaseNode::vBaseNode(const std::string &nodeID)
{
	this->id_ = nodeID;
	this->active_ = true;
	this->updateFlag_ = true;
	
	connectTO_.clear();
	connectFROM_.clear();
}

vBaseNode::~vBaseNode()
{
	// destructor
}

void vBaseNode::debugPrint()
{
	std::cout << "  " << id_ << ":" << std::endl;
	std::cout << "    pos:\t" << pos_.x << "," << pos_.y << "," << pos_.z << std::endl;
	std::cout << "    rot:\t" << rot_.x << "," << rot_.y << "," << rot_.z << std::endl;
	std::cout << "    active?\t" << active_ << std::endl;
}

void vBaseNode::setPosition(double x, double y, double z)
{
	pos_ = Vector3(x,y,z);
	updateFlag_ = true;
	vAudioManager::Instance().update(this);
}

void vBaseNode::setRotation(double pitch, double roll, double yaw)
{
	rot_ = Vector3(pitch, roll, yaw);
	updateFlag_ = true;
	vAudioManager::Instance().update(this);
}
