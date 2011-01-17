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
 
TutorialApplication::TutorialApplication() : headNode_(0)
{
    audioScene_.setTranslator(translator_);
    soundSource_ = audioScene_.getOrCreateSoundSource("sound_a");
    soundSource_->setChannelID(1);

    listener_ = audioScene_.getOrCreateListener("listener");
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
    if (not processUnbufferedInput(evt)) 
        return false;

    return ret;
}


bool TutorialApplication::processUnbufferedInput(const Ogre::FrameEvent &evt)
{
    if (headNode_ == 0)
        return false;
    static double updateSoundSource = 0.0;
    const static double move = 10.0;
    std::cout << "processUnbufferd\n";
    Ogre::Vector3 transVector(Ogre::Vector3::ZERO);
    if (mKeyboard->isKeyDown(OIS::KC_A) or
            mKeyboard->isKeyDown(OIS::KC_LEFT))
    {
        transVector.x -= move;
    }
    if (mKeyboard->isKeyDown(OIS::KC_D) or
            mKeyboard->isKeyDown(OIS::KC_RIGHT))
    {
        transVector.x += move;
    }
    if (mKeyboard->isKeyDown(OIS::KC_W) or
            mKeyboard->isKeyDown(OIS::KC_UP))
    {
        transVector.y -= move;
    }
    if (mKeyboard->isKeyDown(OIS::KC_S) or
            mKeyboard->isKeyDown(OIS::KC_DOWN))
    {
        transVector.y += move;
    }
    // make sure that we scale the translation by the framerate, 
    // to avoid movement varying with the framerate
    transVector *= evt.timeSinceLastFrame;
    headNode_->translate(transVector, Ogre::Node::TS_LOCAL);
    updateSoundSource -= evt.timeSinceLastFrame;
    if (updateSoundSource < 0.0)
    {
        soundSource_->setPosition(transVector.x, 
                transVector.y, transVector.z);
        updateSoundSource = 0.5; // update every 0.5 seconds
    }
    return true;
}

bool TutorialApplication::keyPressed(const OIS::KeyEvent &arg)
{
#if 0
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
#endif

    return BaseApplication::keyPressed(arg);
}

void TutorialApplication::createScene()
{
    std::cout << "createScene\n";
    Ogre::Entity *ogreHead = mSceneMgr->createEntity("Suzanne", "suzanne.mesh");

    headNode_ = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    headNode_->attachObject(ogreHead);
    // FIXME: Fri Jan 14 20:00:09 EST 2011: should adjust mesh instead of 
    // scaling and rotating here
    headNode_->scale(20.0, 20.0, 20.0);
    Ogre::Radian pitch(M_PI * 0.5);
    headNode_->pitch(pitch);

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
