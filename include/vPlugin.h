#ifndef __vPlugin_H
#define __vPlugin_H

#include "vSoundConn.h"

class vPlugin
{
    public:

        enum PluginType { CONSOLE, DMITRI, MULTICAST };

        vPlugin();
        ~vPlugin();

        std::string getTypeString();

        /**
         * The main work of the plugin is done by the update() method, which is
         * called whenever there is a change to some parameters within a connection.
         */
        virtual void update(vSoundConn *conn);

    protected:
        PluginType type_;
};


#endif
