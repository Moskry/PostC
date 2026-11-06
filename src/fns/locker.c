#include "locker.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static int create_mutex(pthread_mutex_t *mtx) {
  for (char cnt = 0; cnt < 5; cnt++)
    if (pthread_mutex_init(mtx, NULL) == 0)
      return EXIT_SUCCESS;
  perror("pthread_mutex_init");
  return EXIT_FAILURE;
}

int init_locks(void) {
  if (create_mutex(&assign_lock) == EXIT_FAILURE) {
    perror("create_mutex");
    return EXIT_FAILURE;
  }
  if (create_mutex(&data_funcs_lock) == EXIT_FAILURE) {
    pthread_mutex_destroy(&assign_lock);
    perror("create_mutex");
    return EXIT_FAILURE;
  }
  if (create_mutex(&lock_global) == EXIT_FAILURE) {
    pthread_mutex_destroy(&assign_lock);
    pthread_mutex_destroy(&data_funcs_lock);
    perror("create_mutex");
    return EXIT_FAILURE;
  }
  for (char cnt = 0; cnt < 5; cnt++)
    if (pthread_cond_init(&data_funcs_condition, NULL) == 0)
      return EXIT_SUCCESS;

  if (pthread_cond_init(&data_funcs_condition, NULL) != 0) {
    pthread_mutex_destroy(&assign_lock);
    pthread_mutex_destroy(&data_funcs_lock);
    pthread_mutex_destroy(&lock_global);
    perror("pthread_cond_init");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void destroy_locks(void) {
    pthread_mutex_destroy(&assign_lock);
    pthread_mutex_destroy(&data_funcs_lock);
    pthread_mutex_destroy(&lock_global);
    pthread_cond_destroy(&data_funcs_condition);
}
