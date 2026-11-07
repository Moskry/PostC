#ifndef _MSG_PROTOCOLS_H_
#define _MSG_PROTOCOLS_H_

#include <stddef.h>

#define MAXMSGLEN 4096

/* Messaging metadata protocol formats */
#define SUBSCRIBE "%ld0009subscribe%d%s"
#define UNSUBSCRIBE "%ld0011unsubscribe"
#define REQUEST "%ld0004from%d%s"
#define REQUEST_ALL "%ld0003all"
#define RESPONSE "%ld0012responsefrom%d%s%d%s" // from %s to %s
#define ACKNOWLEDGE "%ld0006served%d%s%d%s" // to %s payload %s
#define SEND "%ld0004send%d%s%d%s" // to %s payload %s

typedef enum { STRING, MAIL, ACKNOWLEDGEMENT } MessageType;

int read_from(int cfd, char *buffer, size_t n);

int receive_from_client(int cfd);

int send_to_client(int cfd, void *data, size_t n, MessageType type);

#endif
