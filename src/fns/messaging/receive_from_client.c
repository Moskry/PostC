#include "actions.h"
#include "connections.h"
#include "messaging.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* REQUIRES FURTHER TESTING */

static int dispatch_to_handlers(int cfd, const char *request, char *buffer) {
  if (strcmp(request, "unsubscribe") == 0) {
    return unsubscribe(buffer);
  } else if (strcmp(request, "from") == 0) {
    return handle_from(cfd, buffer);
  } else if (strcmp(request, "all") == 0) {
    return handle_all(cfd);
  } else if (strcmp(request, "send") == 0) {
    return handle_send(cfd, buffer);
  }
  fprintf(stderr, "Incorrect request format on socket %d!\n", cfd);
  return EXIT_FAILURE;
}

int receive_from_client(int cfd) {
  char buffer[2048] = {0};

  if (read_from(cfd, buffer, 4) != EXIT_SUCCESS) {
    perror("read_from");
    return EXIT_FAILURE;
  }

  size_t length = strtol(buffer, NULL, 10);
  char *buffer_position = &buffer[4];

  char req[20] = {0};
  if (read_from(cfd, buffer_position, length) != EXIT_SUCCESS) {
    perror("read_from");
    return EXIT_FAILURE;
  }

  memcpy(req, buffer_position, length);

  return dispatch_to_handlers(cfd, req, buffer_position + length);
}
