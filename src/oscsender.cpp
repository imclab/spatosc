#include <iostream>
#include <lo/lo_lowlevel.h>
#include "oscsender.h"

namespace spatosc
{
/*
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
*/
OscSender::OscSender(const std::string &addr) :
    fromPort_(""),
    address_(lo_address_new_from_url(addr.c_str()))//,
    //server_(lo_server_new(0, 0))
{
    if (lo_address_get_protocol(address_)==LO_TCP)
        server_ = lo_server_new_with_proto(lo_address_get_port(address_), LO_TCP, NULL);
    else
        server_ = lo_server_new(0,0);
}

OscSender::OscSender(const std::string &addr, const std::string &fromPort) :
    fromPort_(fromPort),
    address_(lo_address_new_from_url(addr.c_str()))//,
    //server_(lo_server_new(fromPort_.c_str(), 0))
{
    if (lo_address_get_protocol(address_)==LO_TCP)
        server_ = lo_server_new_with_proto(fromPort_.c_str(), LO_TCP, NULL);
    else
        server_ = lo_server_new(fromPort_.c_str(),0);
}




std::string OscSender::toString() const
{
    std::stringstream sstr;
    sstr << lo_server_get_port(server_);
    //return "host: " + host_ + ", port: " + toPort_ + " (outgoing port: "+sstr.str()+")";
    return std::string(lo_address_get_url(address_)) + " (outgoing port: "+sstr.str()+")";
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
    //lo_send_message_from(address_, NULL, OSCpath.c_str(), msg);

    // Let's free the message after (not sure if this is necessary):
    //lo_message_free(msg);
}

} // end of namespace spatosc
