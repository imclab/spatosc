/**
 * This file is part of spatosc.
 *
 * Copyright (c) 2010 Society for Arts and Technology <info@sat.qc.ca>
 *
 * spatosc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * spatosc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with spatosc.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "application.h"
#include "gui.h"
#include "openal_renderer.h"
    
void Application::initSubsystems()
{
    AudioScene::init(); 
    GUI::clutterInit(); 
}

Application::Application() : 
    audioScene_(new AudioScene),
    gui_(new GUI)
{
}

void Application::start()
{
    audioScene_->start();
    // this is the main loop
    gui_->run();
}

