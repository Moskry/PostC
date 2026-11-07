#include "listing.h"
#include "locker.h"
#include <stdlib.h>
#include <unistd.h>

int pop_item(Mail **mail, int id) {
  pthread_mutex_lock(&lock_global);
  MailingList *item = mailingList;
  for (; item != NULL; item = item->front) {
    if (item->mail->id == id) {
      *mail = item->mail;
      if (item->back != NULL) {
        if (item->front != NULL) {
          item->back->front = item->front;
          item->front->back = item->back;
        } else
          item->back->front = NULL;
      } else if (item->front != NULL) {
        mailingList = item->front;
        mailingList->back = NULL;
      } else
        mailingList = NULL;
      free(item);
      pthread_mutex_unlock(&lock_global);
      return EXIT_SUCCESS;
    }
  }
  pthread_mutex_unlock(&lock_global);
  return EXIT_FAILURE;
}
