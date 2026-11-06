#ifndef _THREADING_H_
#define _THREADING_H_

#include <pthread.h>
#include <stdatomic.h>
#define WAITING_MAX 128
/* List of threads */
extern pthread_t *thread_descriptors;

/* Array of waiting tasks */
extern void *(*tasks[WAITING_MAX])(void *);

/* Current number of waiting tasks */
extern atomic_uint waiting_tasks_count;

typedef void *task_args;
/* Array of task arguments */
extern task_args task_arguments[WAITING_MAX];

/* Create a thread pool and set a number of threads */
int thread_pool_init(int num);

/* Close the thread pool */
void thread_pool_closing(void);

#endif
