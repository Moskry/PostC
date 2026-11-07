#include "threading.h"
#include "locker.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

atomic_bool closing_condition;

static void *waiter(void *data) {
  task task_next;
  void *arg;
  int fd;
  for (;;) {
    pthread_mutex_lock(&assign_lock);
    while (atomic_load(&waiting_tasks_count) == 0 &&
           !atomic_load(&closing_condition))
      pthread_cond_wait(&data_funcs_condition, &assign_lock);
    if (atomic_load(&closing_condition) == true) {
      pthread_mutex_unlock(&assign_lock);
      goto closing;
    }

    if (atomic_load(&waiting_tasks_count) > 0) {
      task_next = pop_task();
      fd = pop_arg();
    }

    pthread_mutex_unlock(&assign_lock);
    arg = &fd;
    pthread_cond_signal(&data_funcs_condition);
    task_next(arg);
  }

closing:
  return NULL;
}

/* Just a prototype.
 * Heavily depends on the thread pool implementation.
 * Can be a deadlock spinner otherwise. */
int thread_pool_init(int num) {
  if (num <= 0) {
    perror("incorrect number set in thread_pool_init");
    return EXIT_FAILURE;
  }

  thread_descriptors = calloc(num, sizeof(pthread_t));
  for (int i = 0; i < num; i++) {
    if (pthread_create(&thread_descriptors[i], NULL, waiter, NULL) != 0) {
      for (int j = 0; j < i; j++) {
        pthread_join(thread_descriptors[j], NULL);
      }
      free(thread_descriptors);
      return EXIT_FAILURE;
    }
  }

  atomic_store(&waiting_tasks_count, 0);
  atomic_store(&waiting_args_count, 0);
  atomic_store(&closing_condition, false);

  return EXIT_SUCCESS;
}

void thread_pool_closing(void) { atomic_store(&closing_condition, true); }
