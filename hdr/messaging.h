#ifndef _MSG_PROTOCOLS_H_
#define _MSG_PROTOCOLS_H_

#include <stddef.h>

/* Messaging metadata protocol formats */
#define SUBSCRIBE "subscribe %s\n"
#define UNSUBSCRIBE "unsubscribe %s\n"
#define REQUEST "from %s\n"
#define REQUEST_ALL "all \n"
#define RESPONSE "response from %s#payload#%s\n"
#define ACKNOWLEDGE "served %s#payload#%s\n"
#define SEND "send %s#payload#%s\n"

typedef enum { STRING, MAIL, ACKNOWLEDGEMENT } MessageType;

int receive_from_client(int cfd, void *buffer, size_t n);

int send_to_client(int cfd, void *data, size_t n, MessageType type);

#endif
