/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
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
#ifndef __TUTORIAL_APPLICATION_H_
#define __TUTORIAL_APPLICATION_H_

#include "BaseApplication.h"
#include <spatosc/spatosc.h>

namespace Ogre
{
    class SceneNode;
}

class TutorialApplication : public BaseApplication
{
    public: 
        TutorialApplication();
    private:
        void createAudioScene();
        virtual bool processUnbufferedInput(const Ogre::FrameEvent& evt);
        virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
        virtual void createScene();
        Ogre::SceneNode *headNode_;
        Ogre::SceneNode *nodeTwo_;
        spatosc::Scene audioScene_;
        spatosc::SoundSource *soundSourceOne_;
        spatosc::SoundSource *soundSourceTwo_;
        spatosc::Listener *listener_;
};

#endif // __TUTORIAL_APPLICATION_H_
