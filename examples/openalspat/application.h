#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <tr1/memory>

class GUI;
class AudioScene;

class Application 
{
    private:
        std::tr1::shared_ptr<GUI> gui_;
        std::tr1::shared_ptr<AudioScene> audioScene_;

    public:
        /// Initialize clutter and openal systems
        static void initSubsystems();
        Application();
        void start();
};

#endif // __APPLICATION_H__
