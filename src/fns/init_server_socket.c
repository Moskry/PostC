#include "connections.h"
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct sockaddr_in6 server_addr;
Subscribers subscribers;
int server_fd;

static int init_subscriber_list(void) {
  subscribers.defname = calloc(1, sizeof(char *));
  subscribers.length = calloc(1, sizeof(socklen_t));
  subscribers.socket = calloc(1, sizeof(struct sockaddr_storage));
  subscribers.valid = calloc(1, sizeof(bool));
  subscribers.valid[0] = false;
  if (subscribers.defname == NULL || subscribers.socket == NULL || subscribers.length == NULL)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

/* TESTS PENDING */
int init_server_socket(void) {
  struct addrinfo hints;
  struct addrinfo *result;
  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_canonname = NULL;
  hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(NULL, PORT_NUM, &hints, &result) != 0) {
    perror("getaddrinfo");
    return EXIT_FAILURE;
  }

  struct addrinfo *r;

  int optval = 1;
  for (r = result; r != NULL; r = r->ai_next) {
    server_fd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
    if (server_fd == -1)
      continue; // Try next address

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval,
                   sizeof(optval)) == -1) {
      perror("setsockopt");
      return EXIT_FAILURE;
    }
    if (bind(server_fd, r->ai_addr, r->ai_addrlen) == 0) {
      memcpy(&server_addr, r->ai_addr, r->ai_addrlen);
      break;
    }
    close(server_fd);
  }
  if (r == NULL) {
    perror("Could not bind socket to any address");
    return EXIT_FAILURE;
  }
  
  if (listen(server_fd, 128) == -1) {
    perror("listen");
    return EXIT_FAILURE;
  }

  freeaddrinfo(result);
  return init_subscriber_list();
}
