#include "actions.h"
#include "connections.h"
#include "listing.h"
#include "locker.h"
#include "messaging.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int handle_all(int cfd) {
  Mail **found = NULL;
  char *name = NULL;
  pthread_mutex_lock(&lock_global);
  for (size_t i = 0; i < count; i++) {
    if (subscribers[i].fd == cfd) {
      for (char cnt = 0; cnt < 5; cnt++) {
        name = strdup(subscribers[i].defname);
        if (name != NULL)
          break;
      }
      if (name == NULL) {
        perror("strdup @ handle_all");
        pthread_mutex_unlock(&lock_global);
        return EXIT_FAILURE;
      }
      pthread_mutex_unlock(&lock_global);
      break;
    }
  }

  int count_found = 0;

  pthread_mutex_lock(&lock_global);

  for (MailingList *item = mailingList; item != NULL; item = item->front) {

    if (strcmp(item->mail->to, name) == 0) {
      count_found++;
      void *temp = NULL;
      for (char cnt = 0; cnt < 5; cnt++) {
        temp = realloc(found, count_found * sizeof(Mail *));
        if (temp != NULL)
          break;
      }
      if (temp == NULL) {
        perror("realloc @ handle_all");
        free(name);
        if (count_found != 1) {
          for (int i = 0; i < count_found - 1; i++) {
            if (new_list_item(found[i]) == EXIT_FAILURE) {
              perror("new_list_item @ handle_all");
              free_mail(found[i]);
            }
          }
          free(found);
          pthread_mutex_unlock(&lock_global);
          return EXIT_FAILURE;
        }
        if (found != NULL)
          free(found);
        pthread_mutex_unlock(&lock_global);
        return EXIT_FAILURE;
      }
      found = temp;
      int status = pop_item(&found[count_found - 1], item->mail->id);

      if (status == EXIT_FAILURE) {
        perror("pop_item @ handle_all");
        free(name);
        for (int i = 0; i < count_found; i++) {
          if (new_list_item(found[i]) == EXIT_FAILURE) {
            perror("new_list_item @ handle_all");
            free_mail(found[i]);
          }
        }
        free(found);
        pthread_mutex_unlock(&lock_global);
        return EXIT_FAILURE;
      }
    }
  }

  pthread_mutex_unlock(&lock_global);
  if (count_found == 0)
    return 404;

  for (int i = 0; i < count_found; i++) {
    send_to_client(cfd, found[i], sizeof(Mail), MAIL);
    free_mail(found[i]);
  }
  free(name);
  free(found);

  return EXIT_SUCCESS;
}
