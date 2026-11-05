#include "listing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void free_mail(Mail *mail) {
  free(mail->from);
  free(mail->to);
  free(mail->payload);
  free(mail);
}

int free_mailing_list(void) {
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
  return EXIT_SUCCESS;
}
