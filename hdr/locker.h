#ifndef _LOCKER_H_
#define _LOCKER_H_

#include <pthread.h>

/* locks on task setup */
extern pthread_mutex_t assign_lock;

/* locks on use of task array */
extern pthread_mutex_t data_funcs_lock;

/* locks on use of any global variables, excluding atomics */
extern pthread_mutex_t lock_global;

/* signals when one thread is allowed to enter the critical zone for task
 * selection */
extern pthread_cond_t data_funcs_condition;

int init_locks(void);

void destroy_locks(void);

#endif
