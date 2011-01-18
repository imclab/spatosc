#include "application.h"
#include "gui.h"
#include "audio_scene.h"
    
void Application::initSubsystems()
{
    AudioScene::init(); 
    GUI::clutterInit(); 
}

Application::Application() : 
    gui_(new GUI), 
    audioScene_(new AudioScene)
{
}

void Application::start()
{
    gui_->run();
}

