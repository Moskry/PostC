#ifndef _THREADING_H_
#define _THREADING_H_

#include <pthread.h>

/* List of threads */
extern pthread_t *thread_descriptors;

/* Create a thread pool and set a number of threads */
int thread_pool_init(int num);

/* Close the thread pool */
int thread_pool_close(void);

#endif
