#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "hybrid_lock.h"
struct hybrid_lock g_mutex;
//pthread_spinlock_t slock;
//pthread_mutex_t mlock;


long g_count;

void *thread_func(void *arg)
{
	//sets* setset = (sets*)arg;
	long i, j, k, count =(long)arg;
	long long l;

	/*
	 * Increase the global variable, g_count.
	 * This code should be protected by
	 * some locks, e.g. spin lock, and the lock that 
	 * you implemented for assignment,
	 * because g_count is shared by other threads.
	 */
	for (i = 0; i<count; i++) {
		/************ Critical Section ************/

		hybrid_lock_lock(&g_mutex);
		//pthread_spin_lock(&slock);
		//pthread_mutex_lock(&mlock);
		for (j = 0; j<100000; j++)
			for (k = 0; k<3000; k++)
				l += j * k;
		//g_mutex.g_count++;
		//printf("%d\n",setset->tid);
		g_count++;
		//pthread_mutex_unlock(&mlock);
		//pthread_spin_unlock(&slock);
		hybrid_lock_unlock(&g_mutex);
		/******************************************/
	}
}

int main(int argc, char *argv[])
{
	pthread_t *tid;
	long i, thread_count, value;
	int rc;
	//sets setset[4];
	hybrid_lock_init(&g_mutex);
	//pthread_spin_init(&slock, PTHREAD_PROCESS_PRIVATE);
	//pthread_mutex_init(&mlock,NULL);

	/*
	 * Check that the program has three arguments
	 * If the number of arguments is not 3, terminate the process.
	 */
	if (3 != argc) {
		fprintf(stderr, "usage: %s <thread count> <value>\n", argv[0]);
		exit(0);
	}

	/*
	 * Get the values of the each arguments as a long type.
	 * It is better that use long type instead of int type,
	 * because sizeof(long) is same with sizeof(void*).
	 * It will be better when we pass argument to thread
	 * that will be created by this thread.
	 */
	thread_count = atol(argv[1]);
	value = atol(argv[2]);
	/*
	 * Create array to get tids of each threads that will
	 * be created by this thread.
	 */
	tid = (pthread_t*)malloc(sizeof(pthread_t)*thread_count);
	if (!tid) {
		fprintf(stderr, "malloc() error\n");
		exit(0);
	}

	/*
	 * Create a threads by the thread_count value received as
	 * an argument. Each threads will increase g_count for
	 * value times.
	 */
	for (i = 0; i<thread_count; i++) {
		//setset[i].count = value;
		//setset[i].tid = i;
		rc = pthread_create(&tid[i], NULL, thread_func, (void*)value);
		if (rc) {
			fprintf(stderr, "pthread_create() error\n");
			free(tid);
			hybrid_lock_destroy(&g_mutex);
			//pthread_spin_destroy(&slock);
			//pthread_mutex_destroy(&mlock);
			exit(0);
		}
	}

	/*
	 * Wait until all the threads you created above are finished.
	 */
	for (i = 0; i<thread_count; i++) {
		rc = pthread_join(tid[i], NULL);
		if (rc) {
			fprintf(stderr, "pthread_join() error\n");
			free(tid);
			hybrid_lock_destroy(&g_mutex);
			//pthread_spin_destroy(&slock);
			//pthread_mutex_destroy(&mlock);
			exit(0);
		}
	}

	hybrid_lock_destroy(&g_mutex);
	//pthread_spin_destroy(&slock);
	//pthread_mutex_destroy(&mlock);


	/*
	 * Print the value of g_count.
	 * It must be (thread_count * value)
	 */ 
	//printf("value: %ld\n", g_mutex.g_count);
	printf("value: %ld\n", g_count);
	free(tid);
}
