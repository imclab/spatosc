/** @file The OscSender class.
 */
#ifndef __OSC_SENDER_H__
#define __OSC_SENDER_H__

#include <lo/lo.h>
#include <string>

#define SPATOSC_ARGS_END LO_ARGS_END

namespace spatosc
{

/** General-purpose wrapper around liblo to send OSC messages.
 */
class OscSender 
{
    public:
        OscSender();
        OscSender(const std::string &host, const std::string &port);
        std::string toString() const;
        const char * host() { return host_.c_str(); }
        const char * port() { return port_.c_str(); }
         /**
          * Send a message with one or more string, float and/or int args. 
          * The list of arguments MUST be terminated with SPATOSC_ARGS_END.
          * Here is an example: 
          * \code
          * sendMessage("/oscpath", "fsi", 5.5, "foo", 4, SPATOSC_ARGS_END);
          * \endcode
          */
        void sendMessage(const std::string &OSCpath, const char *types, ...) const;
    private:
        void sendMessage(const std::string &OSCpath, const char *types, va_list ap) const;
        void sendMessage(const std::string &OSCpath, lo_message msg) const;
        std::string host_;
        std::string port_;
        lo_address address_;
};

} // end of namespace spatosc

#endif // _OSC_SENDER_H_

