#include "locker.h"
#include "threading.h"
#include <stdatomic.h>
#include <stdio.h>
#include <sys/types.h>

task pop_task(void) {
  uint task_count = atomic_load(&waiting_tasks_count);
  if (task_count == 0) {
    perror("Task array already empty");
    return NULL;
  }
  pthread_mutex_lock(&data_funcs_lock);
  task popped = tasks[0];
  for (int i = 1; i < task_count; i++) {
    tasks[i - 1] = tasks[i];
  }
  atomic_fetch_sub(&waiting_tasks_count, 1);
  pthread_mutex_unlock(&data_funcs_lock);
  return popped;
}

int pop_arg(void) {
  int args_count = atomic_load(&waiting_args_count);
  if (args_count == 0) {
    perror("Args array already empty");
    return 0;
  }
  pthread_mutex_lock(&data_funcs_lock);
  int popped = arguments[0];
  for (int i = 1; i < args_count; i++) {
    arguments[i - 1] = arguments[i];
  }
  atomic_fetch_sub(&waiting_args_count, 1);
  pthread_mutex_unlock(&data_funcs_lock);
  return popped;
}
