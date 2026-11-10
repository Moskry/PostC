#include "listing.h"
#include "locker.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

MailingList *mailingList;

static MailingList *new_list_item_default(void) {
  MailingList *new;
  char cnt = 0;
  for (; cnt < 5; cnt++) {
    new = calloc(1, sizeof(MailingList));
    if (new != NULL)
      break;
  }
  if (new == NULL) {
    perror("calloc");
    return NULL;
  }

  new->front = NULL;

  return new;
}
static inline void assign_to_list(MailingList *new) {
  pthread_mutex_lock(&lock_global);
  if (mailingList == NULL)
    mailingList = new;
  else {
    MailingList *item = mailingList;
    for (; item->front != NULL; item = item->front)
      ;
    item->front = new;
    item->front->back = item;
  }
  pthread_mutex_unlock(&lock_global);
}

int new_list_item_clone(Mail *mail) {
  MailingList *new = new_list_item_default();
  if (new == NULL)
    return EXIT_FAILURE;
  char cnt = 0;
  for (; cnt < 5; cnt++) {
    new->mail = calloc(1, sizeof(Mail));
    if (new->mail != NULL)
      break;
  }
  if (new->mail == NULL) {
    free(new);
    perror("calloc");
    return EXIT_FAILURE;
  }

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

  assign_to_list(new);

  return EXIT_SUCCESS;
}

int new_list_item(Mail *mail) {
  MailingList *new = new_list_item_default();
  if (new == NULL)
    return EXIT_FAILURE;
  new->mail = mail;
  mail = NULL;

  assign_to_list(new);
  fprintf(stderr, "new->mail->payload: %s\n", new->mail->payload);
  return EXIT_SUCCESS;
}
