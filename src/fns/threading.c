#include "threading.h"
#include <stdlib.h>

pthread_t *thread_descriptors;

static void *waiter(void *data) {
  // ToDo!
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
  return EXIT_SUCCESS;
}

int thread_pool_close(void) {
  // ToDo!
  return EXIT_SUCCESS;
}
