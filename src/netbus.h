#include <libubox/blobmsg.h>
#include <libubox/uloop.h>

#define NETBUS_SOCKET "tcp://0.0.0.0:5060"

typedef void *handle_t; 

#include "proto.h"
