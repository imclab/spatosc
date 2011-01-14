/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "TutorialApplication.h"
#include <spatosc/spatosc.h>

TutorialApplication::TutorialApplication()
{
    audioScene_.setTranslator(translator_);
    soundSource_ = audioScene_.getOrCreateSoundSource("sound_a");
    soundSource_->setChannelID(1);

    listener_ = audioScene_.getOrCreateListener("listener");
}

bool TutorialApplication::keyPressed(const OIS::KeyEvent &arg)
{
    Ogre::Vector3 currentPosition(headNode_->getPosition());
    switch (arg.key)
    {
        case OIS::KC_LEFT:
            std::cout << "left\n";
            currentPosition.x -= 1;
            headNode_->setPosition(currentPosition);
            soundSource_->setPosition(currentPosition.x, 
                    currentPosition.y, currentPosition.z);
            break;
        case OIS::KC_RIGHT:
            std::cout << "right\n";
            currentPosition.x += 1;
            headNode_->setPosition(currentPosition);
            soundSource_->setPosition(currentPosition.x, 
                    currentPosition.y, currentPosition.z);
            break;
        case OIS::KC_UP:
            std::cout << "up\n";
            currentPosition.z += 1;
            headNode_->setPosition(currentPosition);
            soundSource_->setPosition(currentPosition.x, currentPosition.y, currentPosition.z);
            break;
        case OIS::KC_DOWN:
            std::cout << "down\n";
            currentPosition.z -= 1;
            headNode_->setPosition(currentPosition);
            soundSource_->setPosition(currentPosition.x, currentPosition.y, currentPosition.z);
            break;
        case OIS::KC_0:
            std::cout << "return to origin\n";
            currentPosition = Ogre::Vector3(0, 0, 0);
            headNode_->setPosition(currentPosition);
            soundSource_->setPosition(currentPosition.x, currentPosition.y, currentPosition.z);
            break;
        default:
            break;
    }

    return BaseApplication::keyPressed(arg);
}

void TutorialApplication::createScene()
{
    Ogre::Entity *ogreHead = mSceneMgr->createEntity("Head", "suzanne.mesh");

    headNode_ = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    headNode_->attachObject(ogreHead);

    // set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // create a light
    Ogre::Light *l = mSceneMgr->createLight("MainLight");
    l->setPosition(20, 80, 50);
}

int main(int argc, char *argv[])
{
    // Create application object
    TutorialApplication app;

    try 
    {
        app.go();
    } 
    catch(const Ogre::Exception& e)
    {
        std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
    }

    return 0;
}
