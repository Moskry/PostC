#include "listing.h"
#include "messaging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int send_to_client(int cfd, void *data, size_t n, MessageType type) {
  Mail *mail;
  char *payload;
  switch (type) {

  case STRING:
    send(cfd, data, n, 0);
    break;

  case MAIL:
    mail = data;
    payload = calloc(strlen(mail->from) + strlen(mail->payload) + 32, sizeof(char));
    sprintf(payload, RESPONSE, mail->from, mail->payload);
    send(cfd, payload, strlen(payload), MSG_NOSIGNAL);
    free(payload);
    break;

  case ACKNOWLEDGEMENT:
    mail = data;
    payload = calloc(strlen(mail->to) + strlen(mail->payload) + 32, sizeof(char));
    sprintf(payload, ACKNOWLEDGE, mail->to, mail->payload);
    send(cfd, payload, strlen(payload), MSG_NOSIGNAL);
    free(payload);
    break;
  default:
    perror("Entered default state on send to client. Shouldn't happen");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
