#include <zmq.h>
#include "netbus.h"

#define NETBUS_SOCKET "tcp://0.0.0.0:5060"
/*
static const struct {
	void (*handler)(struct blob_attr *msg); 
} handlers[NETBUS_MSG_MAX] = {
	[NETBUS_MSG_RESOLVE] = { .handler = netbus_resolve }, 
	[NETBUS_MSG_ADD] = { .handler = netbus_add_object }, 
	[NETBUS_MSG_CALL] = { .handler = netbus_call }, 
	[NETBUS_MSG_LIST] = { .handler = netbus_list }
}; */

static const struct blob_attr_info ubus_policy[NETBUS_ATTR_MAX] = {
	[NETBUS_ATTR_STATUS] = { .type = BLOB_ATTR_INT8 },
	[NETBUS_ATTR_OBJID] = { .type = BLOB_ATTR_INT32 },
	[NETBUS_ATTR_OBJPATH] = { .type = BLOB_ATTR_STRING },
	/*[NETBUS_ATTR_METHOD] = { .type = BLOB_ATTR_STRING },
	[NETBUS_ATTR_ACTIVE] = { .type = BLOB_ATTR_INT8 },
	[NETBUS_ATTR_NOREPLY] = { .type = BLOB_ATTR_INT8 }*/
};

static struct blob_attr *attrbuf[NETBUS_ATTR_MAX];

int main (void){
    handle_t ctx = zmq_ctx_new ();
    char buff[1024 * 16]; 
    struct blob_buf bb,rb; 
    
    blob_buf_init(&rb, 0); 
    
    int error = 0; 
    
    handle_t rep = zmq_socket (ctx, ZMQ_REP);
    error = zmq_bind (rep, NETBUS_SOCKET);
    if(error){
			fprintf(stderr, "Could not setup netbus socket %s\n", NETBUS_SOCKET); 
			return -1; 
		}
    
    while(1){
			// receive a message from a connected client
			int rc = zmq_recv (rep, buff, sizeof(buff), 0);
			
			//parse message
			blob_parse((struct blob_attr*)buff, attrbuf, ubus_policy, NETBUS_ATTR_MAX); 
			int msg_id = blob_get_int8(attrbuf[NETBUS_ATTR_STATUS]); 
			int obj_id = blob_get_int32(attrbuf[NETBUS_ATTR_OBJID]);
			const char *msg = blob_get_string(attrbuf[NETBUS_ATTR_OBJPATH]); 
			
			// send response
			blob_buf_init(&bb, 0); 
			blob_put_int8(&bb, 0, NETBUS_MSG_ACK); 
			
			printf("Sending back %d\n", blob_raw_len(bb.head)); 
			rc = zmq_send (rep, bb.head, blob_raw_len(bb.head), 0);
		}
		
    zmq_close (rep);
    zmq_ctx_term (ctx);
    
    return 0 ;
}

