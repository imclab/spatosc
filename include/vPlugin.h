/** @file
 * The vPlugin class.
 */
#ifndef __vPlugin_H
#define __vPlugin_H

class vSoundConn;

/**
 * Base class for audio renderers.
 */
class vPlugin
{
    public:

        enum PluginType { CONSOLE, DMITRI, MULTICAST };

        vPlugin();
        ~vPlugin();

        /**
         * Returns the name of this vPlugin.
         * 
         * Each child of vPlugin has a unique string identifier.
         */
        std::string getTypeString();

        /**
         * The main work of the plugin is done by the update() method, which is
         * called whenever there is a change to some parameters within a connection.
         * 
         * For each pair of nodes in the scene that are connected, it might calculate the 
         * distance between those two, their relative position, gain and other attributes.
         * It should then make sure there is some audio rendering going, either by sending interprocess
         * messages to control some audio engine, or by rendering audio by itself.
         */
        virtual void update(vSoundConn *conn);

    protected:
        PluginType type_;
};


#endif
