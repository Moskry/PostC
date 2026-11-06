#include "listing.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static atomic_uint id = 1;

static int clone(char **dest, const char *src) {
  for (char cnt = 0; cnt < 5; cnt++) {
    *dest = strdup(src);
    if (*dest != NULL)
      break;
  }
  if (*dest == NULL) {
    perror("strdup");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

Mail *new_mail(const char *from, const char *to, const char *payload) {
  Mail *new = NULL;
  for (char cnt = 0; cnt < 5; cnt++) {
    Mail *new = malloc(sizeof(Mail));
    if (new != NULL)
      break;
  }
  if (new == NULL) {
    perror("malloc");
    return NULL;
  }
  if (clone(&new->from, from) == EXIT_FAILURE) {
    free(new);
    perror("clone");
    return NULL;
  }
  if (clone(&new->to, to) == EXIT_FAILURE) {
    free(new->from);
    free(new);
    perror("clone");
    return NULL;
  }
  if (clone(&new->payload, payload) == EXIT_FAILURE) {
    free(new->from);
    free(new->to);
    free(new);
    perror("clone");
    return NULL;
  }
  new->id = atomic_load(&id);
  atomic_fetch_add(&id, 1);
  return new;
}
