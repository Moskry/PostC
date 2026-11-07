#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "locker.h"

int main(int argc, char *argv[]) {
  printf("Hello world!\n");

  if (init_locks() == EXIT_FAILURE) {
    perror("init_locks");
    return EXIT_FAILURE;
  }

  // Initialize thread pool
  //
  // initialize socket
  //
  // initialize epoll
  //
  // initialize lists
  //
  // set sub list to epoll
  //
  // wait on epoll
  
  destroy_locks();

  return EXIT_SUCCESS;
} 
