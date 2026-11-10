#include "actions.h"
#include "connections.h"
#include "listing.h"
#include "locker.h"
#include <asm-generic/errno-base.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int string_duplicate(char **dest, char *src, int num) {
  for (char cnt = 0; cnt < 5; cnt++) {
    *dest = strndup(src, num);
    if (*dest != NULL)
      break;
  }
  if (*dest == NULL) {
    perror("strdup @ string_duplicate");
    return EXIT_FAILURE;
  }
  fprintf(stderr, "string_duplicate succeeded with value %s\n", *dest);
  return EXIT_SUCCESS;
}

int handle_send(int cfd, char *buffer) {

  fprintf(stderr, "handle_send\n");
  fprintf(stderr, "buffer value: %s\n", buffer);
  char lenstr[5] = {0};
  memcpy(lenstr, buffer, 4);
  buffer += 4;

  int length = strtol(lenstr, NULL, 10);
  if (length > 1024 || length <= 0) {
    perror("incorrect length input @ handle_send");
    return EXIT_FAILURE;
  }

  fprintf(stderr, "length: %d\n", length);

  char *name_to = NULL;
  if (string_duplicate(&name_to, buffer, length) == EXIT_FAILURE) {
    perror("string_duplicate @ handle_send");
    return EXIT_FAILURE;
  }

  buffer += length;
  fprintf(stderr, "buffer: %s\n", buffer);
  fprintf(stderr, "name_to: %s\n", name_to);

  memset(lenstr, 0, 5 * sizeof(char));
  memcpy(lenstr, buffer, 4);

  length = strtol(lenstr, NULL, 10);
  if (length > 1024 || length <= 0) {
    perror("incorrect length input @ handle_send");
    free(name_to);
    return EXIT_FAILURE;
  }

  buffer += 4;

  fprintf(stderr, "buffer: %s\n", buffer);
  fprintf(stderr, "length: %d\n", length);

  char *payload = NULL;
  if (string_duplicate(&payload, buffer, length + 1) == EXIT_FAILURE) {
    free(name_to);
    perror("string_duplicate @ handle_send");
    return EXIT_FAILURE;
  }

  fprintf(stderr, "fd: %d - cfd: %d\n", subscribers[0].fd, cfd);
  fprintf(stderr, "service: %s - from: %s\n", subscribers[0].defname, name_to);
  fprintf(stderr, "count: %d", (int)count);

  char *name_from = NULL;

  for (size_t i = 0; i < count; i++) {
    fprintf(stderr, "before locks\n");
    pthread_mutex_lock(&lock_global);
    fprintf(stderr, "locks\n");
    if (subscribers[i].fd == cfd) {
      if (string_duplicate(&name_from, subscribers[i].defname,
                           strlen(subscribers[i].defname)) == EXIT_FAILURE) {
        perror("string_duplicate @ handle_send");
        free(name_to);
        free(payload);
        pthread_mutex_unlock(&lock_global);
        fprintf(stderr, "unlocks\n");
        return EXIT_FAILURE;
      }
      fprintf(stderr, "name_from: %s\n", name_from);
    }
    fprintf(stderr, "before unlocks\n");
    pthread_mutex_unlock(&lock_global);
    fprintf(stderr, "unlocks\n");
  }

  fprintf(stderr, "name_from: %s\n", name_from);
  fprintf(stderr, "name_to: %s\n", name_to);
  fprintf(stderr, "payload: %s\n", payload);
  Mail *new = new_mail(name_from, name_to, payload);

  free(name_from);
  free(name_to);
  free(payload);

  return new_list_item(new);
}
