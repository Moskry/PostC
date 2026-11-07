#include "actions.h"
#include "listing.h"
#include "locker.h"
#include "messaging.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int handle_from(int cfd, char *buffer) {

  char lenstr[5] = {0};
  memcpy(lenstr, buffer, 4);

  int length = strtol(lenstr, NULL, 10);
  if (length <= 0 || length > 1024) {
    perror("length @ handle_from");
    return EXIT_FAILURE;
  }

  buffer += 4;
  char *fromstr = calloc(length + 1, sizeof(char));
  memcpy(fromstr, buffer, length);

  Mail **found = NULL;
  int found_count = 0;
  pthread_mutex_lock(&lock_global);
  for (MailingList *item = mailingList; item != NULL; item = item->front) {
    if (strncmp(item->mail->from, fromstr, length) != 0)
      continue;
    found_count++;

    void *temp = NULL;
    for (char cnt = 0; cnt < 5; cnt++) {
      temp = realloc(found, sizeof(Mail *) * found_count);
      if (temp != NULL)
        break;
    }
    if (temp == NULL) {
      perror("realloc @ handle_from");
      for (int i = 0; i < found_count - 1; i++) {
        if (new_list_item(found[i]) == EXIT_FAILURE) {
          perror("new_list_item @ handle_from");
          free_mail(found[i]);
        }
      }
      free(found);
      pthread_mutex_unlock(&lock_global);
      return EXIT_FAILURE;
    }

    int status = pop_item(&found[found_count - 1], item->mail->id);
    if (status == EXIT_FAILURE) {
      perror("pop_item @ handle_from");
      for (int i = 0; i < found_count - 1; i++) {
        if (new_list_item(found[i]) == EXIT_FAILURE) {
          perror("new_list_item @ handle_from");
          free_mail(found[i]);
        }
      }
      free(found);
      pthread_mutex_unlock(&lock_global);
      return EXIT_FAILURE;
    }
  }
  pthread_mutex_unlock(&lock_global);
  if (found_count == 0)
    return 404;

  for (int i = 0; i < found_count; i++) {
    send_to_client(cfd, found[i], sizeof(Mail), MAIL);
    free_mail(found[i]);
  }
  free(found);

  // if (send_to_client(cfd, found, sizeof(Mail), MAIL) == EXIT_FAILURE)
  return EXIT_SUCCESS;
}
