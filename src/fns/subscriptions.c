#include "connections.h"
#include <stdlib.h>
#include <sys/socket.h>

// INCOMPLETE
static int subscribe_on_invalid(const char *defname,
                                struct sockaddr_storage *socket,
                                socklen_t length, int i) {
  
}
// INCOMPLETE
static int subscribe_on_last(const char *defname,
                             struct sockaddr_storage *socket,
                             socklen_t length) {
  subscribers.count++;

  void *buffer = NULL;
  char cnt = 0;
  for (; cnt < 5; cnt++) {
    void *buffer =
        realloc(subscribers.defname, sizeof(char *) * subscribers.count);
    if (buffer != NULL)
      break;
  }
  if (buffer == NULL) {
    subscribers.count--;
    return EXIT_FAILURE;
  }
  subscribers.defname = buffer;

  buffer = NULL;
  for (cnt = 0; cnt < 5; cnt++) {
    buffer = realloc(subscribers.socket,
                     sizeof(struct sockaddr_storage) * subscribers.count);
    if (buffer != NULL)
      break;
  }
  if (buffer == NULL) {
    subscribers.count--;
    subscribers.defname =
        realloc(subscribers.defname, sizeof(char *) * subscribers.count);
    return EXIT_FAILURE;
  }
  subscribers.socket = buffer;

  buffer = NULL;
  for (cnt = 0; cnt < 5; cnt++) {
    buffer = realloc(subscribers.length, sizeof(socklen_t) * subscribers.count);
    if (buffer != NULL)
      break;
    if (buffer == NULL) {
      subscribers.count--;
      subscribers.defname =
          realloc(subscribers.defname, sizeof(char *) * subscribers.count);
      subscribers.socket =
          realloc(subscribers.socket,
                  sizeof(struct sockaddr_storage) * subscribers.count);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

// INCOMPLETE
int subscribe(const char *defname, struct sockaddr_storage *socket,
              socklen_t length) {
  for (int i = 0; i < subscribers.count; i++) {
    if (subscribers.valid[i] == true)
      continue;
    else
      return subscribe_on_invalid(defname, socket, length, i);

    if (subscribers.count == (i + 1))
      return subscribe_on_last(defname, socket, length);
  }
  return EXIT_SUCCESS;
}

// INCOMPLETE
int unsubscribe(const char *defname) {}
