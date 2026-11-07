#include "connections.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// INCOMPLETE
int subscribe(int fd, const char *defname, struct sockaddr_storage *socket,
              socklen_t length) {
  void *buffer = NULL;
  count++;
  char cnt = 0;
  for (; cnt < 5; cnt++) {
    buffer = realloc(subscribers, sizeof(Subscribers) * count);
    if (buffer != NULL)
      break;
  }
  if (buffer == NULL) {
    count--;
    return EXIT_FAILURE;
  }
  subscribers = buffer;

  for (cnt = 0; cnt < 5; cnt++) {
    subscribers[count - 1].defname = strdup(defname);
    if (subscribers[count - 1].defname != NULL)
      break;
  }
  if (subscribers[count - 1].defname == NULL) {

    count--;
    if (count == 0) {
      free(subscribers);
      subscribers = NULL;
      return EXIT_FAILURE;
    }

    buffer = NULL;
    for (cnt = 0; cnt < 5; cnt++) {
      buffer = realloc(subscribers, sizeof(Subscribers) * count);
      if (buffer != NULL)
        break;
    }
    if (buffer == NULL) {
      perror("realloc");
      return EXIT_FAILURE;
    }
    subscribers = buffer;
    return EXIT_FAILURE;
  }

  memcpy(&subscribers[count - 1].socketAddress, socket,
         sizeof(subscribers[count - 1].socketAddress));
  subscribers[count - 1].length = length;
  subscribers[count - 1].fd = fd;

  return EXIT_SUCCESS;
}

// INCOMPLETE
int unsubscribe(const char *defname) {
  bool found = false;
  for (size_t i = 0; i < count; i++) {
    if (strcmp(subscribers[i].defname, defname) != 0)
      continue;
    found = true;
    close(subscribers[i].fd);
    free(subscribers[i].defname);
    for (size_t j = i; j < count - 1; j++) {
      subscribers[j] = subscribers[j + 1];
    }

    count--;
    if (count == 0) {
      free(subscribers);
      subscribers = NULL;
      return EXIT_SUCCESS;
    }

    void *buffer;
    for (char cnt = 0; cnt < 5; cnt++) {
      buffer = NULL;
      buffer = realloc(subscribers, sizeof(Subscribers) * count);
      if (buffer != NULL)
        break;
    }
    if (buffer == NULL) {
      perror("realloc");
      return EXIT_FAILURE;
    }
    subscribers = buffer;
    break;
  }
  if (found)
    return EXIT_SUCCESS;
  return EXIT_FAILURE;
}
