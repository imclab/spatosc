#ifndef __OSC_RECEIVER_H__
#define __OSC_RECEIVER_H__

#include <lo/lo.h>
#include <string>

namespace spatosc
{

/** General-purpose wrapper around liblo to receive OSC messages.
 */
class OscReceiver
{
    public:
        /**
         * Constructor. Note that the port is a string, not an int.
         */
        OscReceiver(const std::string &port);
        ~OscReceiver();
        /**
         * Returns the port as a C string.
         */
        const char * port() const { return port_.c_str(); }
        /**
         * Add a callback for a given OSC path.
         */
        void addHandler(const char *path, 
                const char *types, lo_method_handler handler, 
                void *user_data);
        int receive();
    private:
        std::string toString() const;
        std::string port_;
        lo_server server_;
#ifdef CONFIG_DEBUG
        static int genericHandler(const char *path, 
                const char *types, lo_arg **argv, 
                int argc, void *data, void *user_data);
#endif
        static void error(int num, const char *msg, const char *path);
};

} // end namespace spatosc

#endif // __OSC_RECEIVER_H__

