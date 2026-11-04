#ifndef _CONNECTIONS_H_
#define _CONNECTIONS_H_

#include <sys/socket.h>

/* struct of arrays pattern */
typedef struct {
  char *defname;
  struct sockaddr_storage *socket;
  socklen_t *length;
} Subscribers;

/* Struct of subscriber lists */
extern Subscribers subscribers;

/* Self explanatory */
int init_server_socket(void);

/* Subscribe a client under a name to be able to send and receive payloads */
int subscribe(const char *defname);

/* The opposite of subscribing */
int unsubscribe(const char *defname);
#endif
