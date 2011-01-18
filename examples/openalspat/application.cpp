#include "application.h"
#include "gui.h"
#include "audio_scene.h"
    
void Application::initSubsystems()
{
    AudioScene::init(); 
    GUI::clutterInit(); 
}

Application::Application() : 
    audioScene_(new AudioScene),
    gui_(new GUI(*this))
{
}

void Application::start()
{
    gui_->run();
}

