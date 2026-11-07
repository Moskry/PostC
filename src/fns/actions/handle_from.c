#include "actions.h"
#include "listing.h"
#include "messaging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int handle_from(int cfd, char *buffer) {
  if (read_from(cfd, buffer, 4) != EXIT_SUCCESS) {
    perror("read_from @ handle_from");
    return EXIT_FAILURE;
  }

  int count = strtol(buffer, NULL, 10);
  if (count <= 0 || count > 1024){
    perror("count @ handle_from");
    return EXIT_FAILURE;
  }

  buffer += 4;
  if (read_from(cfd, buffer, count) != EXIT_SUCCESS) {
    perror("read_from @ handle_from");
    return EXIT_FAILURE;
  }

  Mail *found;
  for (MailingList *item = mailingList; item != NULL; item = item->front) {
    if (strncmp(item->mail->from, buffer, count) != 0)
      continue;
    count++;
    int status = pop_item(&found, item->mail->id);
    if (status == EXIT_SUCCESS)
      if (send_to_client(cfd, found, sizeof(Mail), MAIL) == EXIT_FAILURE)
        return EXIT_FAILURE;
  }

  if (count == 0)
    return 404;

  return EXIT_SUCCESS;
}
