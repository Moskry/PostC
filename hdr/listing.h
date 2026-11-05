#ifndef _LISTING_H_
#define _LISTING_H_

typedef struct {
  int id;
  char *from;
  char *to;
  char *payload;
} Mail;

typedef struct ml {
  Mail *mail;
  struct ml *front;  
  struct ml *back;
} MailingList;

/* Public mailing list */
extern MailingList *mailingList;

Mail *new_mail(const char *from, const char *to, const char *payload);

/* Add a new item to the list to be sent */
int new_list_item(Mail *mail);

/* Pull an item from the list and handle the rest */
int pull_item(Mail *mail);

/* Clear the contents of the mailing list */
int clear_mailing_list(void);

/* Free the mailing list object */
int free_mailing_list(void);

#endif
