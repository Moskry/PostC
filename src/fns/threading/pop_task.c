#include "locker.h"
#include "threading.h"
#include <stdatomic.h>
#include <stdio.h>
#include <sys/types.h>

task pop_task(void) {
  uint count = atomic_load(&waiting_tasks_count);
  if (count == 0) {
    perror("Task array already empty");
    return NULL;
  }
  pthread_mutex_lock(&assign_lock);
  task popped = tasks[0];
  for (int i = 1; i < count; i++) {
    tasks[i - 1] = tasks[i];
  }
  atomic_fetch_sub(&waiting_tasks_count, 1);
  pthread_mutex_unlock(&assign_lock);
  return popped;
}

int pop_arg(void) {
  int count = atomic_load(&waiting_args_count);
  if (count == 0) {
    perror("Args array already empty");
    return 0;
  }
  pthread_mutex_lock(&assign_lock);
  int popped = arguments[0];
  for (int i = 1; i < count; i++) {
    arguments[i - 1] = arguments[i];
  }
  atomic_fetch_sub(&waiting_args_count, 1);
  pthread_mutex_unlock(&assign_lock);
  return popped;
}
