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
#include <spatosc.h>

namespace Ogre {
    class SceneNode;
}

class TutorialApplication : public BaseApplication
{
    public: 
        TutorialApplication();
    private:
        virtual bool keyPressed(const OIS::KeyEvent &arg);
        virtual void createScene();
        Ogre::SceneNode *headNode_;
        spatosc::Scene audioScene_;
        std::tr1::shared_ptr<spatosc::Translator> translator_;
        spatosc::SoundSource *soundSource_;
        spatosc::Listener *listener_;
};

#endif // #ifndef __TutorialApplication_h_
