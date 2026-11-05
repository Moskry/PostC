#include "messaging.h"
#include "actions.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "connections.h"

/* REQUIRES FURTHER TESTING */

static int dispatch_to_handlers(int cfd, const char *request, char *buffer) {
  if (strcmp(request, "unsubscribe") == 0) {
    return unsubscribe(buffer);
  }
  else if (strcmp(request, "from") == 0) {
    return handle_from(cfd, buffer);
  }
  else if (strcmp(request, "all") == 0) {
    return handle_all(cfd);
  }
  else if (strcmp(request, "send") == 0) {
    return handle_send(cfd, buffer);
  }
  fprintf(stderr, "Incorrect request format on socket %d!\n", cfd);
  return EXIT_FAILURE;
}

int receive_from_client(int cfd, void *buffer, size_t n) {
  for (;;) {
    int num_read = read(cfd, buffer, n);
    if (num_read == -1) {

      if (errno == EINTR)
        continue;
      else
        return EXIT_FAILURE;
    } else
      break;
  }

  if (strlen((char *)buffer) <= 3)
    return 2; // Ignore the request

  char *whitespace = strstr(buffer, " ");
  char req[20] = {0};
  memcpy(req, buffer, whitespace - (char *)buffer);

  return dispatch_to_handlers(cfd, req, ++whitespace); 
}
