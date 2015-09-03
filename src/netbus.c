#include <zmq.h>
#include "netbus.h"

int main (void){
    handle_t ctx = zmq_ctx_new ();
    char buff[1024 * 16]; 
    struct blob_buf bb; 
    
    int error = 0; 
    
    handle_t req = zmq_socket (ctx, ZMQ_REQ);
    error = zmq_connect(req, NETBUS_SOCKET);
    if(error){
			fprintf(stderr, "Could not setup netbus socket %s\n", NETBUS_SOCKET); 
			return -1; 
		}
    
    while(1){
			// send message
			blob_buf_init(&bb, 0); 
			blob_put_int8(&bb, NETBUS_ATTR_STATUS, NETBUS_MSG_CALL); 
			blob_put_int32(&bb, NETBUS_ATTR_OBJID, 1111); 
			blob_put_string(&bb, NETBUS_ATTR_OBJPATH, "Hello World!"); 
			
			int rc = zmq_send (req, bb.head, blob_raw_len(bb.head), 0);
			
			// receive response
			rc = zmq_recv (req, buff, sizeof(buff), 0);
			
			//parse message
			blob_buf_init(&bb, 0); 
			blob_put_raw(&bb, buff, rc); 
			blob_next(bb.head); 
			int msg_id = blob_get_int8(bb.head); 
			printf("Got %d bytes, id: %d\n", rc, msg_id); 
		}
		
    zmq_close (req);
    zmq_ctx_term (ctx);
    
    return 0 ;
}

