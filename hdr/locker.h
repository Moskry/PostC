#ifndef _LOCKER_H_
#define _LOCKER_H_

#include <pthread.h>

extern pthread_mutex_t assign_lock;
extern pthread_mutex_t data_funcs_lock;
extern pthread_mutex_t lock_global;
extern pthread_cond_t data_funcs_condition;

int init_locks(void);

void destroy_locks(void);

#endif
