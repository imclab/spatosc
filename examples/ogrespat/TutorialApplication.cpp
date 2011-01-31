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
    createAudioScene();
}

void TutorialApplication::createAudioScene()
{
    soundSource_ = audioScene_.createSoundSource("1");
    listener_ = audioScene_.createListener("1");
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

void TutorialApplication::createScene()
{
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
