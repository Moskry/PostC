#include "actions.h"
#include "connections.h"
#include "listing.h"
#include "messaging.h"
#include <stdlib.h>
#include <string.h>

int handle_all(int cfd) {
  Mail *found;
  char *name;
  for (size_t i = 0; i < count; i++) {
    if (subscribers[i].fd == cfd) {
      name = subscribers[i].defname;
      break;
    }
  }

  int count = 0;
  for (MailingList *item = mailingList; item != NULL; item = item->front) {

    if (strcmp(item->mail->to, name) == 0) {
      count++;
      int status = pop_item(&found, item->mail->id);
      if (status == EXIT_SUCCESS) {
        return send_to_client(cfd, found, sizeof(Mail), MAIL);
      }
    }
  }
  if (count == 0) return 404;
  return EXIT_SUCCESS;
}
