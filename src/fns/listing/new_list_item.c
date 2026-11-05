#include "listing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

MailingList *mailingList;

/* COULD BE OPTIMIZED BASED ON MAIL */
int new_list_item(Mail *mail) {
  MailingList *new;
  char cnt = 0;
  for (; cnt < 5; cnt++) {
    new = calloc(1, sizeof(MailingList));
    if (new != NULL)
      break;
  }
  if (new == NULL) {
    perror("calloc");
    return EXIT_FAILURE;
  }

  for (cnt = 0; cnt < 5; cnt++) {
    new->mail = calloc(1, sizeof(Mail));
    if (new->mail != NULL)
      break;
  }
  if (new->mail == NULL) {
    free(new);
    perror("calloc");
    return EXIT_FAILURE;
  }
  new->front = NULL;

  // memcpy(new->mail, mail, sizeof(Mail)); // Doesn't work properly
  for (cnt = 0; cnt < 5; cnt++) {
    if (new->mail->from == NULL)
      new->mail->from = strdup(mail->from);
    if (new->mail->to == NULL)
      new->mail->to = strdup(mail->to);
    if (new->mail->payload == NULL)
      new->mail->payload = strdup(mail->payload);

    if (new->mail->from != NULL && new->mail->to != NULL &&
        new->mail->payload != NULL)
      break;
  }
  if (new->mail->from == NULL || new->mail->to == NULL ||
      new->mail->payload == NULL) {
    if (new->mail->from != NULL)
      free(new->mail->from);
    if (new->mail->to != NULL)
      free(new->mail->to);
    if (new->mail->payload != NULL)
      free(new->mail->payload);
    free(new->mail);
    free(new);
    return EXIT_FAILURE;
  }

    if (mailingList == NULL)
      mailingList = new;
    else {
      MailingList *item = mailingList;
      for (; item->front != NULL; item = item->front)
        ;
      item->front = new;
      item->front->back = item;
    }
  return EXIT_SUCCESS;
}
