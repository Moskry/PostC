#include "actions.h"
#include "listing.h"
#include "messaging.h"
#include <stdlib.h>
#include <string.h>

int handle_from(int cfd, char *buffer) {
  char *newline = strstr(buffer, "\n");
  char *name = strndup(buffer, newline - buffer);

  int count = 0;
  int status;
  Mail *found;
  for (MailingList *item = mailingList; item != NULL; item = item->front) {
    if (strcmp(item->mail->from, name) != 0)
      continue;
    count++;
    int status = pop_item(&found, item->mail->id);
    if (status == EXIT_SUCCESS)
      if (send_to_client(cfd, found, sizeof(Mail), MAIL) == EXIT_FAILURE)
        return EXIT_FAILURE;
  }

  if (count == 0)
    return 404;

  free(name);
  return EXIT_SUCCESS;
}
