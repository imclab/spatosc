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
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
#include <spatosc/spatosc.h>

namespace Ogre {
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
        spatosc::Scene audioScene_;
        spatosc::SoundSource *soundSource_;
        spatosc::Listener *listener_;
};

#endif // #ifndef __TutorialApplication_h_
