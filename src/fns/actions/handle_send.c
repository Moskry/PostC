#include "actions.h"
#include "connections.h"
#include "listing.h"
#include <stdlib.h>
#include <string.h>

int handle_send(int cfd, char *buffer) {
  char *delimiter = strstr(buffer, "#payload#");
  char *name_to = NULL;
  char *name_from = NULL;
  char *payload = NULL;
  char cnt = 0;
  for (; cnt < 5; cnt++) {
    name_to = strndup(buffer, delimiter - buffer);
    if (name_to != NULL)
      break;
  }
  if (name_to == NULL)
    return EXIT_FAILURE;

  for (cnt = 0; cnt < 5; cnt++) {
    payload = strstr(delimiter + 1, "#");
    if (payload != NULL)
      break;
  }
  if (payload == NULL)
    return EXIT_FAILURE;
  payload++;

  char *newline = NULL;
  for (cnt = 0; cnt < 5; cnt++) {
    newline = strstr(payload, "\n");
    if (newline != NULL)
      break;
  }
  if (newline == NULL)
    return EXIT_FAILURE;
  *newline = 0;

  for (size_t i = 0; i < count; i++) {
    if (subscribers[i].fd == cfd) {
      for (cnt = 0; cnt < 5; cnt++) {
        name_from = strdup(subscribers[i].defname);
        if (name_from != NULL)
          break;
      }
      if (name_from != NULL)
        break;
      return EXIT_FAILURE;
    }
  }

  Mail *new = new_mail(name_from, name_to, payload);
  return new_list_item(new);

  return EXIT_SUCCESS;
}
