#ifndef _THREADING_H_
#define _THREADING_H_

#include <pthread.h>
#include <stdatomic.h>

#define WAITING_MAX 128

typedef void *(*task)(void *);

/* List of threads */
extern pthread_t *thread_descriptors;

/* Array of waiting tasks */
extern task tasks[WAITING_MAX];
/* Array of task arguments */
extern int arguments[WAITING_MAX];

/* Current number of waiting tasks */
extern atomic_uint waiting_tasks_count;
/* Current number of waiting arguments */
extern atomic_uint waiting_args_count;

/* Create a thread pool and set a number of threads */
int thread_pool_init(int num);

/* Close the thread pool */
void thread_pool_closing(void);

task pop_task(void);
int pop_arg(void);

int push_task(task t);
int push_arg(int fd);
#endif
