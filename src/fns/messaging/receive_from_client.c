#include "actions.h"
#include "connections.h"
#include "locker.h"
#include "messaging.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* NOT UNIT TESTED */

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
  char buffer[MAXMSGLEN] = {0};
  char req[20] = {0};

  pthread_mutex_lock(&lock_global);
  if (read_from(cfd, buffer, 4) != EXIT_SUCCESS) {
    perror("read_from");
    pthread_mutex_unlock(&lock_global);
    return EXIT_FAILURE;
  }

  long length = strtol(buffer, NULL, 10);
  if (length <= 0 || length > MAXMSGLEN) {
    if (length <= 0) {
      perror("length @ receive_from_client");
      pthread_mutex_unlock(&lock_global);
      return EXIT_FAILURE;
    }
    for (; length > MAXMSGLEN; length -= MAXMSGLEN)
      read_from(cfd, buffer, MAXMSGLEN);
    read_from(cfd, buffer, length);
    pthread_mutex_unlock(&lock_global);
    perror("length @ receive_from_client");
  }
  char *buffer_position = &buffer[4];

  if (read_from(cfd, buffer_position, length) != EXIT_SUCCESS) {
    perror("read_from");
    pthread_mutex_unlock(&lock_global);
    return EXIT_FAILURE;
  }
  pthread_mutex_unlock(&lock_global);

  char lenstr[5] = {0};
  memcpy(lenstr, buffer_position, 4 * sizeof(char));
  length = strtol(lenstr, NULL, 10);

  memcpy(req, buffer_position, length);

  return dispatch_to_handlers(cfd, req, buffer_position);
}
