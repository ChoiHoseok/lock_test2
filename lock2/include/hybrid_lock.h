#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

struct hybrid_lock {
	struct {
    	pthread_mutex_t mLock;
    };
    struct{
    	pthread_mutex_t pLock;
    };
};

int hybrid_lock_init(struct hybrid_lock *lock);
int hybrid_lock_destroy(struct hybrid_lock *lock);
int hybrid_lock_lock(struct hybrid_lock *lock);
int hybrid_lock_unlock(struct hybrid_lock *lock);
