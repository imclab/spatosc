/*
 * This file is part of Spatosc.
 *
 * Copyright (c) 2010 Society for Arts and Technologies <info@sat.qc.ca>
 *
 * Spatosc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Spatosc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Spatosc.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <iostream>

#include <osgViewer/CompositeViewer>
#include <osgViewer/View>
#include <osgViewer/ViewerEventHandlers>
#include <osg/GraphicsContext>
#include <osgDB/ReadFile>
#include <osg/Timer>

#include <spinFramework/spinApp.h>
#include <spinFramework/sceneManager.h>
#include <spinFramework/spinClientContext.h>
#include <spinFramework/ViewerManipulator.h>
#include <spinFramework/spinUtil.h>
#include <spinFramework/ShapeNode.h>
#include <spinFramework/TextNode.h>

#include <spatosc/spatosc.h>
#include <spatosc/geotransform.h>

extern pthread_mutex_t sceneMutex;

spatosc::Scene audioScene_;

using namespace spin;

int node_callback(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
    // make sure there is at least one argument (ie, a method to call):
    if (!argc) return 1;

    std::string nodeID = std::string(path);
    nodeID = nodeID.substr(nodeID.rfind("/")+1);

    // get the method (argv[0]):
    std::string method;
    if (lo_is_string_type((lo_type)types[0])) method = std::string((char *)argv[0]);
    else return 1;

    spatosc::Node *node = audioScene_.getNode(nodeID);
    if (node)
    {
    	std::vector<double> floatArgs;
        std::vector<const char*> stringArgs;
        for (int i=1; i<argc; i++)
        {
            if (lo_is_numerical_type((lo_type)types[i]))
            {
                floatArgs.push_back( (double) lo_hires_val((lo_type)types[i], argv[i]) );
            } else {
                stringArgs.push_back( (const char*) argv[i] );
            }
        }

    	if ((method=="global6DOF") && (floatArgs.size()==6))
        {
            node->setPosition(floatArgs[0], floatArgs[1], floatArgs[2]);
            node->setOrientation(floatArgs[3], floatArgs[4], floatArgs[5]);
    	}
    }

    return 1;
}



int main(int argc, char **argv)
{
	std::cout << "spinspat launching ..." << std::endl;

	double maxFrameRate = 60;
    bool dmitri = false;
    std::string dmitriIP = "127.0.0.1";
    std::string spatdifIP = "127.0.0.1";

	// *************************************************************************
	// set up SPIN:

	spinClientContext spinListener;
	spinApp &spin = spinApp::Instance();


	std::string sceneID = "default";
   // spin.getSceneID();


	// *************************************************************************
	// deal with args:

	osg::ArgumentParser arguments(&argc,argv);
	arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
	arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" is a SPIN viewer that renders a test scene for spatosc");
	arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options]");
	arguments.getApplicationUsage()->addCommandLineOption("-h or --help", "Display this information");
	arguments.getApplicationUsage()->addCommandLineOption("--scene-id <uniqueID>", "Specify the scene ID to listen to (Default: '" + sceneID + "')");
	arguments.getApplicationUsage()->addCommandLineOption("--remote-ip <IP address>", "Specify the remote IP address of the spatdif spatializer (Default: '" + spatdifIP + "')");
	arguments.getApplicationUsage()->addCommandLineOption("--dmitri <IP address>", "Use the D-Mitri translator and send messages to the specified DCP address (requires a properly configured spacemap and OSC mapping control defined in CueStation");

	if (arguments.read("-h") || arguments.read("--help"))
	{
		arguments.getApplicationUsage()->write(std::cout);
		return 0;
	}

	osg::ArgumentParser::Parameter param_spinID(sceneID);
	arguments.read("--scene-id", param_spinID);
	spin.setSceneID(sceneID);

	osg::ArgumentParser::Parameter param_remoteIP(spatdifIP);
	if (arguments.read("--remote-ip", param_remoteIP));
	
    osg::ArgumentParser::Parameter param_dmitri(dmitriIP);
	if (arguments.read("--dmitri", param_dmitri)) dmitri = true;
	
    
    // *************************************************************************
    if (!spinListener.start())
	{
        std::cout << "ERROR: could not start SPIN listener" << std::endl;
        exit(EXIT_FAILURE);
	}
	spin.sceneManager->setGraphical(true);

    // *************************************************************************
	// add extra handlers for spatosc:
	// (one for the scene, one for the nodes:

	lo_server_add_method(spin.getContext()->lo_rxServs_[0], NULL, NULL, node_callback, NULL);


	// *************************************************************************
	// set up viewer:

	osgViewer::CompositeViewer viewer = osgViewer::CompositeViewer(arguments);
 	viewer.setThreadingModel(osgViewer::CompositeViewer::CullDrawThreadPerContext);
	viewer.getUsage(*arguments.getApplicationUsage());

    osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
    viewer.addView(view.get());

    view->getCamera()->setClearColor(osg::Vec4(0.0, 0.0, 0.0, 0.0));
    view->setUpViewInWindow(50,50,640,480);
    view->setSceneData(spin.sceneManager->rootNode.get());
	view->setLightingMode(osg::View::HEADLIGHT);

	view->addEventHandler(new osgViewer::StatsHandler);
	view->addEventHandler(new osgViewer::ThreadingHandler);
	view->addEventHandler(new osgViewer::WindowSizeHandler);
	view->addEventHandler(new osgViewer::HelpHandler(arguments.getApplicationUsage()));

	// *************************************************************************
	// create a camera manipulator

	osg::ref_ptr<ViewerManipulator> manipulator = new ViewerManipulator();
	manipulator->setPicker(true);
	manipulator->setMover(false);

	view->setCameraManipulator(manipulator.get());
	
	// *************************************************************************
	// report any unread options
	arguments.reportRemainingOptionsAsUnrecognized();
	if (arguments.errors())
	{
		arguments.writeErrorMessages(std::cout);
		return 1;
	}

	// *************************************************************************
	// create test scene:


    if (dmitri)
    {
	    //spatosc::DmitriTranslator *dmitriTranslator = dynamic_cast<spatosc::DmitriTranslator>(audioScene_.addTranslator<spatosc::DmitriTranslator>("dmitri", dmitriIP.c_str(), spatosc::DmitriTranslator::DEFAULT_SEND_PORT));
        
        // POINTER METHOD:
        spatosc::DmitriTranslator *dmitriTranslator = new spatosc::DmitriTranslator(dmitriIP.c_str());
        audioScene_.addTranslator("dmitri", dmitriTranslator);
        
        //dmitriTranslator->setEquatorRadius(1000.0);
    }
    
    // also send to spatdif translator for this example (for doppler component):
    audioScene_.addTranslator<spatosc::SpatdifTranslator>("spatdif", spatdifIP.c_str(), spatosc::SpatdifTranslator::DEFAULT_SEND_PORT);

    
    //audioScene_.setOrientation(90.0, 0.0, 0.0);
    spatosc::SoundSource *src1 = audioScene_.createSoundSource("source-1");
    src1->setIntProperty("bus", 1);
    src1->setIntProperty("enableDelay", 1);
    spatosc::SoundSource *src2 = audioScene_.createSoundSource("source-2");
    src2->setIntProperty("bus", 2);
	audioScene_.createListener("listener");
	// listener is looking forward along Y axis (camera is above, looking down)
	//listener_->setOrientation(-90.0, 0.0, 0.0);

    // create copy of scene in SPIN:

	spin.SceneMessage("sss", "createNode", "source-1", "ShapeNode", LO_ARGS_END);
	spin.SceneMessage("sss", "createNode", "source-2", "ShapeNode", LO_ARGS_END);
	spin.SceneMessage("sss", "createNode", "source-1-label", "TextNode", LO_ARGS_END);
	spin.SceneMessage("sss", "createNode", "source-2-label", "TextNode", LO_ARGS_END);

	spin.SceneMessage("sss", "createNode", "listener", "ModelNode", LO_ARGS_END);
	spin.SceneMessage("sss", "createNode", "listener-label", "TextNode", LO_ARGS_END);

	usleep(500); // add a delay to make sure node is created

    // important: need to setReportMode so that 6DOF messages are received
	spin.NodeMessage("listener", "si", "setReportMode", (int)GroupNode::GLOBAL_6DOF, LO_ARGS_END);
	spin.NodeMessage("source-1", "si", "setReportMode", (int)GroupNode::GLOBAL_6DOF, LO_ARGS_END);
	spin.NodeMessage("source-2", "si", "setReportMode", (int)GroupNode::GLOBAL_6DOF, LO_ARGS_END);
	
    
    spin.NodeMessage("source-1", "si",   "setShape", (int)ShapeNode::SPHERE, LO_ARGS_END);
	spin.NodeMessage("source-1", "si",   "setInteractionMode", (int)GroupNode::DRAG, LO_ARGS_END);
	spin.NodeMessage("source-1", "sfff", "setTranslation", -1.0, 2.0, 0.0, LO_ARGS_END);
	spin.NodeMessage("source-1", "sfff", "setScale", 0.2, 0.2, 0.2, LO_ARGS_END);
	spin.NodeMessage("source-1-label", "ss",   "setParent", "source-1", LO_ARGS_END);
	spin.NodeMessage("source-1-label", "sfff", "setTranslation", 0.0, 0.0, 1.0, LO_ARGS_END);
	spin.NodeMessage("source-1-label", "sfff", "setScale", 5.0, 5.0, 5.0, LO_ARGS_END);
	spin.NodeMessage("source-1-label", "ss", "setTextValue", "SoundSource 1", LO_ARGS_END);
	spin.NodeMessage("source-1-label", "si",   "setBillboard", (int)TextNode::STAY_UP, LO_ARGS_END);

    spin.NodeMessage("source-2", "si",   "setShape", (int)ShapeNode::SPHERE, LO_ARGS_END);
	spin.NodeMessage("source-2", "si",   "setInteractionMode", (int)GroupNode::DRAG, LO_ARGS_END);
	spin.NodeMessage("source-2", "sfff", "setTranslation", 1.0, 2.0, 0.0, LO_ARGS_END);
	spin.NodeMessage("source-2", "sfff", "setScale", 0.2, 0.2, 0.2, LO_ARGS_END);
	spin.NodeMessage("source-2-label", "ss",   "setParent", "source-2", LO_ARGS_END);
	spin.NodeMessage("source-2-label", "sfff", "setTranslation", 0.0, 0.0, 1.0, LO_ARGS_END);
	spin.NodeMessage("source-2-label", "sfff", "setScale", 5.0, 5.0, 5.0, LO_ARGS_END);
	spin.NodeMessage("source-2-label", "ss", "setTextValue", "SoundSource 2", LO_ARGS_END);
	spin.NodeMessage("source-2-label", "si",   "setBillboard", (int)TextNode::STAY_UP, LO_ARGS_END);

	spin.NodeMessage("listener", "ss", "setModelFromFile", "listener.osg", LO_ARGS_END);
	spin.NodeMessage("listener-label", "ss",   "setParent", "listener", LO_ARGS_END);
	spin.NodeMessage("listener-label", "sfff", "setTranslation", 0.0, 0.0, 0.5, LO_ARGS_END);
	spin.NodeMessage("listener-label", "si",   "setBillboard", (int)TextNode::STAY_UP, LO_ARGS_END);




	// *************************************************************************
	// start viewer:

	viewer.realize();

	// ask for refresh:
	spin.SceneMessage("s", "refresh", LO_ARGS_END);

	osg::Timer_t lastFrameTick = osg::Timer::instance()->tick();
	double minFrameTime = 1.0 / maxFrameRate;

	// program loop:
	while(not viewer.done())
	{
		if (spinListener.isRunning())
		{
			double dt = osg::Timer::instance()->delta_s(lastFrameTick, osg::Timer::instance()->tick());

			if (dt >= minFrameTime)
			{
				pthread_mutex_lock(&sceneMutex);
                viewer.frame();
				pthread_mutex_unlock(&sceneMutex);

                /*
				viewer.advance();
				viewer.eventTraversal();
				pthread_mutex_lock(&sceneMutex);
				viewer.updateTraversal();
				viewer.renderingTraversals();
				pthread_mutex_unlock(&sceneMutex);
				*/
				// save time when the last time a frame was rendered:
				lastFrameTick = osg::Timer::instance()->tick();
				dt = 0;
			}

			unsigned int sleepTime = static_cast<unsigned int>(1000000.0*(minFrameTime-dt));
			if (sleepTime > 100) sleepTime = 100;
			OpenThreads::Thread::microSleep(sleepTime);
		
		} else {
			
			if (manipulator.valid())
			{
				view->setCameraManipulator(NULL);
				manipulator.release();
			}
			
			viewer.setDone(true);
		}
	}

    // make sure we're done in case we didn't quit via interrupt
	spinListener.stop();

    return 0;
}
