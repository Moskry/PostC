#include "actions.h"
#include "connections.h"
#include "listing.h"
#include "messaging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int string_duplicate(char **dest, char *src) {
  for (char cnt = 0; cnt < 5; cnt++) {
    *dest = strdup(src);
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
  if (read_from(cfd, buffer, 4) != EXIT_SUCCESS) {
    perror("read_from @ handle_send");
    return EXIT_FAILURE;
  }

  int length = strtol(buffer, NULL, 10);
  if (length > 1024 || length <= 0) {
    perror("incorrect length input @ handle_send");
    return EXIT_FAILURE;
  }
  buffer += 4;

  if (read_from(cfd, buffer, length) != EXIT_SUCCESS) {
    perror("read_from @ handle_send");
    return EXIT_FAILURE;
  }
  char *name_to = NULL;
  if (string_duplicate(&name_to, buffer) == EXIT_FAILURE) {
    perror("string_duplicate @ handle_send");
    return EXIT_FAILURE;
  }

  buffer += length;

  if (read_from(cfd, buffer, 4) != EXIT_SUCCESS) {
    perror("read_from @ handle_send");
    free(name_to);
    return EXIT_FAILURE;
  }
  length = strtol(buffer, NULL, 10);
  if (length > 1024 || length <= 0) {
    perror("incorrect length input @ handle_send");
    free(name_to);
    return EXIT_FAILURE;
  }

  buffer += 4;
  char *payload = NULL;
  if (read_from(cfd, buffer, length) != EXIT_SUCCESS) {
    perror("read_from @ handle_send");
    free(name_to);
    return EXIT_FAILURE;
  }
  if (string_duplicate(&payload, buffer) == EXIT_FAILURE) {
    free(name_to);
    perror("string_duplicate @ handle_send");
    return EXIT_FAILURE;
  }

  char *name_from = NULL;

  for (size_t i = 0; i < count; i++) {
    if (subscribers[i].fd == cfd) {
      if (string_duplicate(&name_from, subscribers[i].defname) ==
          EXIT_FAILURE) {
        perror("string_duplicate @ handle_send");
        free(name_to);
        free(payload);
        return EXIT_FAILURE;
      }
    }
  }

  Mail *new = new_mail(name_from, name_to, payload);
  return new_list_item(new);
}
