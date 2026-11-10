#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void) {
  int sfd, j, status;
  size_t len;
  ssize_t numRead;

  struct addrinfo hints;
  struct addrinfo *result, *rp;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_family = AF_UNSPEC;
  hints.ai_next = NULL;
  hints.ai_socktype = SOCK_STREAM;

  status = getaddrinfo("::1", "3513", &hints, &result);
  if (status != 0) {
    errno = ENOSYS;
    return -1;
  }

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1)
      continue;

    if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
      break;
    close(sfd);
  }
  freeaddrinfo(result);

  char buffer[1024] = {0};

  read(sfd, buffer, 4);
  int length = strtol(buffer, NULL, 10);
  char *spot = buffer + 4;
  read(sfd, spot, length);

  printf("received: %s\n", spot);
  char format[512] = {0};
  int lengtha = length + 16 + 5;
  sprintf(format, "%04d0004send%s%04d%s", lengtha, spot, 4,
          "hello");
  send(sfd, format, strlen(format), MSG_NOSIGNAL);
  printf("sent: format: %s\n", format);

  char received[512] = {0};

  send(sfd, "00070003all", strlen("00070003all"), MSG_NOSIGNAL);
  read(sfd, received, sizeof(received));
  printf("received: %s\n", received);
  exit(EXIT_SUCCESS);
}
