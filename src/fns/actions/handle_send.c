#include "actions.h"
#include "connections.h"
#include "listing.h"
#include "locker.h"
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
  return EXIT_SUCCESS;
}

int handle_send(int cfd, char *buffer) {

  char lenstr[5] = {0};
  memcpy(lenstr, buffer, 4);

  int length = strtol(lenstr, NULL, 10);
  if (length > 1024 || length <= 0) {
    perror("incorrect length input @ handle_send");
    return EXIT_FAILURE;
  }

  buffer += 4;

  char *name_to = NULL;
  if (string_duplicate(&name_to, buffer, length) == EXIT_FAILURE) {
    perror("string_duplicate @ handle_send");
    return EXIT_FAILURE;
  }

  buffer += length;

  memset(lenstr, 0, sizeof(lenstr));
  memcpy(lenstr, buffer, 4);

  length = strtol(lenstr, NULL, 10);
  if (length > 1024 || length <= 0) {
    perror("incorrect length input @ handle_send");
    free(name_to);
    return EXIT_FAILURE;
  }

  buffer += 4;
  char *payload = NULL;
  if (string_duplicate(&payload, buffer, length) == EXIT_FAILURE) {
    free(name_to);
    perror("string_duplicate @ handle_send");
    return EXIT_FAILURE;
  }

  char *name_from = NULL;

  for (size_t i = 0; i < count; i++) {

    pthread_mutex_lock(&lock_global);
    if (subscribers[i].fd == cfd) {
      if (string_duplicate(&name_from, subscribers[i].defname,
                           strlen(subscribers[i].defname)) == EXIT_FAILURE) {
        perror("string_duplicate @ handle_send");
        free(name_to);
        free(payload);
        pthread_mutex_unlock(&lock_global);
        return EXIT_FAILURE;
      }
    }
    pthread_mutex_unlock(&lock_global);
  }

  Mail *new = new_mail(name_from, name_to, payload);
  free(name_from);
  free(name_to);
  free(payload);
  return new_list_item(new);
}
