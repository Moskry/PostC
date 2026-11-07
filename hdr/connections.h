#ifndef _CONNECTIONS_H_
#define _CONNECTIONS_H_

#include <netinet/in.h>
#include <stdbool.h>
#include <sys/socket.h>

#define PORT_NUM "3513"

// REJECTED
/* struct of arrays pattern
typedef struct {
  char **defname;
  struct sockaddr_storage *socket;
  socklen_t *length;
  bool *valid;
  size_t count;
} Subscribers; */

typedef struct {
  int fd;
  char *defname;
  struct sockaddr_storage socketAddress;
  socklen_t length;
} Subscribers;

/* Struct of subscriber lists */
extern Subscribers *subscribers;
/* Length of the list */
extern size_t count;

/* Server socket address data */
extern struct sockaddr_in6 server_addr;
extern int server_fd;

/* Self explanatory */
int init_server_socket(void);

/* Subscribe a client under a name to be able to send and receive payloads */
int subscribe(int fd, const char *defname, struct sockaddr_storage *socket,
              socklen_t length);

/* The opposite of subscribing */
int unsubscribe(const char *defname);
#endif
