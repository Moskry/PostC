#ifndef _MSG_PROTOCOLS_H_
#define _MSG_PROTOCOLS_H_

#include <stddef.h>

#define MAXMSGLEN 4096

/* Messaging metadata protocol formats, will probably change slightly with time */
#define SUBSCRIBE "%04d%04d%s"
#define UNSUBSCRIBE "%00150011unsubscribe"
#define REQUEST "%ld0004from%d%s"
#define REQUEST_ALL "%ld0003all"
#define RESPONSE "%04d0012responsefrom%04d%s%04d%s" // from %s to %s
#define ACKNOWLEDGE "%04d0006served%04d%s%04d%s" // to %s payload %s
#define SEND "%04d0004send%d%s%d%s" // to %s payload %s

typedef enum { STRING, MAIL, ACKNOWLEDGEMENT } MessageType;

int read_from(int cfd, char *buffer, size_t n);

int receive_from_client(int *cfd);

int send_to_client(int cfd, void *data, size_t n, MessageType type);

#endif
