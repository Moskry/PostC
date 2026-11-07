#include "listing.h"
#include "locker.h"
#include <stdlib.h>
#include <unistd.h>

void free_mail(Mail *mail) {
  free(mail->from);
  free(mail->to);
  free(mail->payload);
  free(mail);
}

int free_mailing_list(void) {
  pthread_mutex_lock(&lock_global);
  MailingList *a, *b;
  a = mailingList;
  b = a;

  while(b != NULL) {
    b = a->front;
    free_mail(a->mail);
    free(a);
    a = b;
  }
  mailingList = NULL;
  pthread_mutex_unlock(&lock_global);
  return EXIT_SUCCESS;
}
