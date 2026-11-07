#include "connections.h"
#include "locker.h"
#include "messaging.h"
#include "threading.h"
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#define MAX_EV 64

int main(int argc, char *argv[]) {

  struct epoll_event ev, events[MAX_EV];
  int epollfd, nfds;

  epollfd = epoll_create1(0);
  if (epollfd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  ev.events = EPOLLIN | EPOLLONESHOT;

  if (argc != 2) {
    printf("Number of threads ommitted, default is 5.\n");
  }

  if (init_server_socket() == EXIT_FAILURE) {
    perror("init_server_socket");
    printf("server failed to initialize!\n");
    exit(EXIT_FAILURE);
  }
  ev.data.fd = server_fd;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
    perror("epoll_ctl: server_fd");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_storage client_sockaddr;
  memset(&client_sockaddr, 0, sizeof(client_sockaddr));
  socklen_t addrlen;
  int status = EXIT_SUCCESS;

  for (;;) {
    nfds = epoll_wait(epollfd, events, MAX_EV, -1);
    if (nfds == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd == server_fd) {
        int client_fd =
            accept(server_fd, (struct sockaddr *)&client_sockaddr, &addrlen);
        if (client_fd == -1) {
          perror("accept");
          status = EXIT_FAILURE;
        }
        ev.events = EPOLLIN | EPOLLONESHOT;
        ev.data.fd = client_fd;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
          perror("epoll_ctl: client_fd");
          status = EXIT_FAILURE;
        }
        char service[NI_MAXSERV];
        int rc = getnameinfo((struct sockaddr *)&client_sockaddr, addrlen, NULL,
                             0, service, sizeof(service), NI_NUMERICSERV);
        if (rc != 0) {
          fprintf(stderr, "getnameinfo: %s\n", gai_strerror(rc));
          exit(EXIT_FAILURE);
        } else {
          subscribe(client_fd, service, &client_sockaddr, addrlen);
        }

        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
          exit(EXIT_FAILURE);
        }
      } else {
        push_task((task)receive_from_client);
        push_arg((events[i].data.fd));
        pthread_cond_signal(&data_funcs_condition);
      }
    }
  }

  if (init_locks() == EXIT_FAILURE) {
    perror("init_locks");
    return EXIT_FAILURE;
  }
  
  destroy_locks();

  return EXIT_SUCCESS;
}
