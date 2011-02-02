#include "oscreceiver.h"
#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstdlib> // for free
#include <lo/lo.h>

#include "soundsource.h"

namespace spatosc
{

OscReceiver::OscReceiver(const std::string &port) :
    port_(port),
    server_(lo_server_new(port_.c_str(), error))
{
#ifdef CONFIG_DEBUG
    /* add method that will match any path and args */
    lo_server_add_method(server_, NULL, NULL, genericHandler, this);
#endif
}

OscReceiver::~OscReceiver()
{
//    std::cout << "Freeing OSC server\n";
    lo_server_free(server_);
}

int OscReceiver::receive()
{
    return lo_server_recv_noblock(server_, 0);
}

void OscReceiver::addHandler(const char *path, const char *types, lo_method_handler handler, void *userData)
{
    lo_server_add_method(server_, path, types, handler, userData);
}

void OscReceiver::error(int num, const char *msg, const char *path)
{
    std::cerr << "liblo server error " << num << " in path " << path
        << ": " << msg << std::endl;
}

#ifdef CONFIG_DEBUG
/* catch any incoming messages and display them. returning 1 means that the
 *  * message has not been fully handled and the server should try other methods */
int OscReceiver::genericHandler(const char *path,
        const char *types, lo_arg **argv,
        int argc, void * /*data*/, void * /*user_data*/)
{
    //OscReceiver *context = static_cast<OscReceiver*>(user_data);
    printf("path: <%s>\n", path);
    for (int i = 0; i < argc; ++i)
    {
        printf("arg %d '%c' ", i, types[i]);
        lo_arg_pp(static_cast<lo_type>(types[i]), argv[i]);
        printf("\n");
    }
    printf("\n");
    fflush(stdout);
    return 1;
}
#endif // CONFIG_DEBUG


std::string OscReceiver::toString() const
{
    return "port:" + port_;
}

// The following 2 structs are needed by the remove_generic_handler() method:
typedef struct _lo_method {
        const char *path;
        const char *typespec;
        lo_method_handler handler;
        char *user_data;
        struct _lo_method *next;
} *internal_lo_method;

#ifdef LO_VERSION_25
typedef struct _lo_server {
        int socket;
        struct addrinfo *ai;
        lo_method first;
        lo_err_handler err_h;
        int port;
        char *hostname;
        char *path;
        int protocol;
        void *queued;
        struct sockaddr_storage addr;
        socklen_t addr_len;
} *internal_lo_server;
#else
typedef struct _lo_server {
        struct addrinfo *ai;
        lo_method first;
        lo_err_handler err_h;
        int port;
        char *hostname;
        char *path;
        int protocol;
        void *queued;
        struct sockaddr_storage addr;
        socklen_t addr_len;
        int sockets_len;
        int sockets_alloc;
#ifdef HAVE_POLL
        struct pollfd *sockets;
#else
        struct { int fd; } *sockets;
#endif
} *internal_lo_server;
#endif

void OscReceiver::removeGenericHandler(void *data)
{
    // Mikewoz says:
    // Here, we need to free methods from the liblo server if one Pd object
    // is destroyed but others still exist. When no other remaining instances
    // require the liblo server, then we can destroy it as well.
    //
    // Unfortunately, the liblo library doesn't provide the hooks to do this;
    // A method can only be removed like this:
    // lo_server_del_method(lo_server s, const char *path, const char *typespec)
    //
    // For us, path and typespec are both NULL, so ALL methods will be deleted!
    //
    // Thus, we've copied the lo_server struct (and named it internal_lo_server)
    // so that we can remove methods if their user_data matches

    internal_lo_server s = reinterpret_cast<internal_lo_server>(server_);

    if (!s->first)
    {
        std::cerr << "OSCrx error: Could not free method! The server has no registered callbacks.";
        return;
    }

    internal_lo_method it, prev, next;
    it = (internal_lo_method) s->first;
    prev = it;

    while (it)
    {
        // in case we free it:
        next = it->next;

        // if the user_data points to our data pointer:
        if (it->user_data == static_cast<char*>(data))
        {
            // Take care when removing the head:
            if (it == s->first)
                s->first = it->next;
            else
                prev->next = it->next;

            next = it->next;
            free((char *) it->path);
            free((char *) it->typespec);
            free(it);
            it = prev;
        }
        prev = it;
        if (it)
            it = next;
    }
}

} // end namespace spatosc
