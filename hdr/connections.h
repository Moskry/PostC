#ifndef _CONNECTIONS_H_
#define _CONNECTIONS_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

#define PORT_NUM "3513"

/* struct of arrays pattern */
typedef struct {
  char **defname;
  struct sockaddr_storage *socket;
  socklen_t *length;
  bool *valid;
  size_t count;
} Subscribers;

/* Struct of subscriber lists */
extern Subscribers subscribers;

/* Server socket address data */
extern struct sockaddr_in6 server_addr;
extern int server_fd;

/* Self explanatory */
int init_server_socket(void);

/* Subscribe a client under a name to be able to send and receive payloads */
int subscribe(const char *defname, struct sockaddr_storage *socket, socklen_t length);

/* The opposite of subscribing */
int unsubscribe(const char *defname);
#endif
