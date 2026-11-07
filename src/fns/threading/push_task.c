#include "locker.h"
#include "threading.h"
#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <sys/types.h>

int push_task(task t) {
  uint count = atomic_load(&waiting_tasks_count);
  if (count == WAITING_MAX) {
    return EXIT_FAILURE;
  }
  pthread_mutex_lock(&assign_lock);
  tasks[count] = t;
  atomic_fetch_add(&waiting_tasks_count, 1);
  pthread_mutex_unlock(&assign_lock);
  return EXIT_SUCCESS;
}
int push_arg(int fd) {
  uint count = atomic_load(&waiting_args_count);
  if (count == WAITING_MAX) {
    return EXIT_FAILURE;
  }
  pthread_mutex_lock(&assign_lock);
  arguments[count] = fd;
  atomic_fetch_add(&waiting_args_count, 1);
  pthread_mutex_unlock(&assign_lock);
  return EXIT_SUCCESS;
}
