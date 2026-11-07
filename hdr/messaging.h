#ifndef _MSG_PROTOCOLS_H_
#define _MSG_PROTOCOLS_H_

#include <stddef.h>

#define MAXMSGLEN 1024

/* Messaging metadata protocol formats */
#define SUBSCRIBE "0009subscribe%d%s"
#define UNSUBSCRIBE "0011unsubscribe"
#define REQUEST "0004from%d%s"
#define REQUEST_ALL "0003all"
#define RESPONSE "0012responsefrom%d%s%d%s" // from %s to %s
#define ACKNOWLEDGE "0006served%d%s%d%s" // to %s payload %s
#define SEND "0004send%d%s%d%s" // to %s payload %s

typedef enum { STRING, MAIL, ACKNOWLEDGEMENT } MessageType;

int read_from(int cfd, char *buffer, size_t n);

int receive_from_client(int cfd);

int send_to_client(int cfd, void *data, size_t n, MessageType type);

#endif
