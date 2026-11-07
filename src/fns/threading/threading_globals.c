#include "threading.h"

pthread_t *thread_descriptors;
task tasks[WAITING_MAX];
int arguments[WAITING_MAX];
atomic_uint waiting_tasks_count;
atomic_uint waiting_args_count;
