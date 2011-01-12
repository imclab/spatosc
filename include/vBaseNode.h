/** @file
 * The vBaseNode class. 
 */

#ifndef __vBaseNode_H
#define __vBaseNode_H

#include <string>
#include <vector>
#include <iostream>
#include "vMath.h"

class vSoundConn;
class vAudioManager;

/**
 * Base class for a node in the scene.
 */
class vBaseNode
{
	// TODO: remove friend classes and provide real getter methods:
	
	friend class vSoundConn;
	friend class vPlugin;
	friend class vPlugin_dmitri;
	friend class vAudioManager;

public:
	
	vBaseNode(const std::string &nodeID);
	~vBaseNode();

    /**
     * Returns the identifier of this node.
     */
	std::string getID() { return id_; }
	
    /**
     * Prints debug info about this node to the console.
     */
	virtual void debugPrint();
    
    /**
     * Sets this node's position in the 3D space.
     */
	virtual void setPosition(double x, double y, double z);

    /**
     * Sets this node's orientation.
     */
	virtual void setRotation(double pitch, double roll, double yaw);

protected:

	std::string id_;
	
	Vector3 pos_;
	Vector3 rot_;
	
	bool active_;
	
	std::vector<vSoundConn*> connectTO_;
	std::vector<vSoundConn*> connectFROM_;
	
	bool updateFlag_;
};


#endif
