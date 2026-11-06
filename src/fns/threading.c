#include "threading.h"
#include "actions.h"
#include "locker.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>

atomic_bool closing_condition;
pthread_t *thread_descriptors;
void *(*functions[WAITING_MAX])(void *);
atomic_uint waiting_functions_count;

static void *waiter(void *data) {
  pthread_mutex_lock(&assign_lock);
  pthread_cond_wait(&data_funcs_condition, &assign_lock);
  if (atomic_load(&closing_condition) == true)
    goto closing;

  pthread_mutex_lock(&data_funcs_lock);
  if (atomic_load(&waiting_functions_count) > 0) {
    functions[atomic_load(&waiting_functions_count - 1)](data);
    pool_take();
  }
  pthread_mutex_unlock(&data_funcs_lock);

  pthread_mutex_unlock(&assign_lock);

closing:
  return NULL;
}

/* Just a prototype.
 * Heavily depends on the thread pool implementation.
 * Can be a deadlock spinner otherwise. */
int thread_pool_init(int num) {
  if (num <= 0)
    return EXIT_FAILURE;

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

  atomic_store(&waiting_functions_count, 0);
  atomic_store(&closing_condition, false);

  return EXIT_SUCCESS;
}

void thread_pool_closing(void) { atomic_store(&closing_condition, true); }
