#include <iostream>
#include "oscsender.h"

namespace spatosc
{

OscSender::OscSender(const std::string &host, const std::string &toPort) :
    host_(host), toPort_(toPort.c_str()), fromPort_(""),
    address_(lo_address_new(host.c_str(), toPort_.c_str())),
    server_(lo_server_new(0, 0))
{
}

OscSender::OscSender(const std::string &host, const std::string &toPort, const std::string &fromPort) :
    host_(host), toPort_(toPort), fromPort_(fromPort),
    address_(lo_address_new(host.c_str(), toPort_.c_str())),
    server_(lo_server_new(fromPort_.c_str(), 0))
{
}

std::string OscSender::toString() const
{
    std::stringstream sstr;
    sstr << lo_server_get_port(server_);
    return "host: " + host_ + ", port: " + toPort_ + " (outgoing port: "+sstr.str()+")";
}


void OscSender::sendMessage(const std::string &OSCpath, const char *types, ...) const
{
    va_list ap;
    va_start(ap, types); // lo_message_add_varargs will call va_end internally
    sendMessage(OSCpath, types, ap);
}


void OscSender::sendMessage(const std::string &OSCpath, const char *types, va_list ap) const
{
    lo_message msg = lo_message_new();
    int err = lo_message_add_varargs(msg, types, ap);

    if (!err)
        sendMessage(OSCpath, msg);
    else
        std::cout << "ERROR (OscSender::sendMessage): " << err << std::endl;
}


void OscSender::sendMessage(const std::string &OSCpath, lo_message msg) const
{
    lo_send_message_from(address_, server_, OSCpath.c_str(), msg);

    // Let's free the message after (not sure if this is necessary):
    lo_message_free(msg);
}

} // end of namespace spatosc
