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
#include "spat_application.h"
#include "spatosc/geotransform.h"

static const double OSC_FLUSH_INTERVAL = 1 / 15.0; // How many seconds between position updates being sent over OSC 
SpatApplication::SpatApplication() :
    headNode_(0)
{
    createAudioScene();
}

void SpatApplication::createAudioScene()
{
    audioScene_.setSynchronous(false); // we will need to call flushMessages() once in a while
    audioScene_.setTranslator<spatosc::SpatdifTranslator>("127.0.0.1", spatosc::SpatdifTranslator::DEFAULT_SEND_PORT);
    audioScene_.getTransform().rotate(90.0, 0.0, 0.0);
    soundSourceOne_ = audioScene_.createSoundSource("source1");
    soundSourceTwo_ = audioScene_.createSoundSource("source2");
    listener_ = audioScene_.createListener("listener1");
}

bool SpatApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
    if (! processUnbufferedInput(evt)) 
        return false;
    return ret;
}

bool SpatApplication::processUnbufferedInput(const Ogre::FrameEvent &evt)
{
    if (headNode_ == 0)
        return false;
    static double timeLeftBeforeOscFlush = 0.0;
    const static double move = 10.0;
    Ogre::Vector3 nodeOneTransVector(Ogre::Vector3::ZERO);
    Ogre::Vector3 nodeTwoTransVector(Ogre::Vector3::ZERO);
    // ----------------------------------- arrows: move entity one -----------------
    if (mKeyboard->isKeyDown(OIS::KC_A) || mKeyboard->isKeyDown(OIS::KC_LEFT))
    {
        nodeOneTransVector.x -= move; // Move entity one left
    }
    if (mKeyboard->isKeyDown(OIS::KC_D) || mKeyboard->isKeyDown(OIS::KC_RIGHT))
    {
        nodeOneTransVector.x += move; // Move entity one right
    }
    if (mKeyboard->isKeyDown(OIS::KC_W))
    {
        nodeOneTransVector.y -= move; // Move entity one up
    }
    if (mKeyboard->isKeyDown(OIS::KC_S))
    {
        nodeOneTransVector.y += move; // Move entity one down
    }
    if (mKeyboard->isKeyDown(OIS::KC_UP))
    {
        nodeOneTransVector.z -= move; // Move entity one up
    }
    if (mKeyboard->isKeyDown(OIS::KC_DOWN))
    {
        nodeOneTransVector.z += move; // Move entity one down
    }
    // -----------------------------------hjkl: move entity two -----------------
    if (mKeyboard->isKeyDown(OIS::KC_H))
    {
        nodeTwoTransVector.x -= move; // Move entity two left
    }
    if (mKeyboard->isKeyDown(OIS::KC_L))
    {
        nodeTwoTransVector.x += move; // Move entity two right
    }
    if (mKeyboard->isKeyDown(OIS::KC_K))
    {
        nodeTwoTransVector.y -= move; // Move entity two up
    }
    if (mKeyboard->isKeyDown(OIS::KC_J))
    {
        nodeTwoTransVector.y += move; // Move entity two down
    }
    // return to origin
    if (mKeyboard->isKeyDown(OIS::KC_O))
    {
        headNode_->setPosition(0.0, 0.0, 0.0);
        soundSourceOne_->setPosition(0.0, 0.0, 0.0);
        return true;
    }
    // make sure that we scale the translation by the framerate, 
    // to avoid movement varying with the framerate
    nodeOneTransVector *= evt.timeSinceLastFrame;
    nodeTwoTransVector *= evt.timeSinceLastFrame;
    // translate the nodes:
    headNode_->translate(nodeOneTransVector, Ogre::Node::TS_LOCAL);
    nodeTwo_->translate(nodeTwoTransVector, Ogre::Node::TS_LOCAL);
    soundSourceOne_->setPosition(headNode_->getPosition().x, headNode_->getPosition().y, headNode_->getPosition().z);
    // We can avoid flushing OSC messages too often by calling spatosc::Scene::setSynchronous(false), like we did. We then need to call flushMessages() every now and then.
    timeLeftBeforeOscFlush -= evt.timeSinceLastFrame;
    if (timeLeftBeforeOscFlush <= 0.0)
    {
        timeLeftBeforeOscFlush = OSC_FLUSH_INTERVAL;
        audioScene_.flushMessages();
        //spatosc::Connection *connection = audioScene_.getConnection(soundSourceOne_, listener_);
        //std::cout << soundSourceOne_->getPosition() << std::endl;
        //std::cerr << "azim:" << connection->azimuth() * spatosc::TO_DEGREES << ", elevation:" << 
            //connection->elevation() * spatosc::TO_DEGREES << std::endl;
    }
    return true;
}

void SpatApplication::createScene()
{
    // First node:
    Ogre::Entity *ogreHead = mSceneMgr->createEntity("Head", "suzanne.mesh");
    headNode_ = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    headNode_->attachObject(ogreHead);
    // FIXME: Fri Jan 14 20:00:09 EST 2011:tmatth: should adjust mesh instead of 
    // scaling and rotating here
    headNode_->scale(20.0, 20.0, 20.0);
    headNode_->pitch(Ogre::Radian(M_PI * 0.5));

    // Second node:
    // TODO: 2011-02-01:aalex:use some other node type for the second node
    Ogre::Entity *ogreHeadTwo = mSceneMgr->createEntity("HeadTwo", "suzanne.mesh");
    nodeTwo_ = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    nodeTwo_->attachObject(ogreHeadTwo);
    nodeTwo_->scale(20.0, 20.0, 20.0);
    nodeTwo_->pitch(Ogre::Radian(M_PI * 0.5));

    // set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8, 0.6, 0.2));

    // create a light
    Ogre::Light *l = mSceneMgr->createLight("MainLight");
    l->setPosition(20, 80, 50);
}

int main(int argc, char *argv[])
{
    // Create application object
    SpatApplication app;

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
