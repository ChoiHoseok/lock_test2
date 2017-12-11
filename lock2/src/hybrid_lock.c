#include "hybrid_lock.h"

int hybrid_lock_init(struct hybrid_lock *lock)     //락을 초기화하는 함수
{
    if(pthread_mutex_init(&lock->mLock, NULL)!=0){
        printf("init error\n");
        return -1;
    }
    if(pthread_mutex_init(&lock->pLock,NULL)!= 0){
        printf("init error\n");
        return -1;
    }
    return 0;
}

int hybrid_lock_destroy(struct hybrid_lock *lock)      //락을 사용이 끝났을 때, 메모리에서 해제해주는 함수
{
    if(pthread_mutex_destroy(&lock->mLock)!=0){
        printf("destroy error\n");
        return -1;
    }
    if(pthread_mutex_destroy(&lock->pLock)!=0){
        printf("destroy error\n");
        return -1;
    }
    return 0;
}

int hybrid_lock_lock(struct hybrid_lock *lock)     //락을 점유하는 함수
{   
    struct timeval start,end;
    long a = 0;
	long b = 0;
    
    gettimeofday(&start,NULL);
    
    while(1){
        if(pthread_mutex_trylock(&lock-> pLock) == 0){
			a = a + 80;
            if(pthread_mutex_trylock(&lock-> mLock) == 0){
                pthread_mutex_unlock(&lock-> pLock);
                return 0;
            }else{
                pthread_mutex_unlock(&lock-> pLock);
            }
        }
        if(a > 57800000){
			b++;
            gettimeofday(&end,NULL);
            if(end.tv_sec > start.tv_sec && end.tv_usec >= start.tv_usec){
                break;
            }else if(end.tv_sec - 2 == start.tv_sec){
				break;
			}
            continue;
        }
        a++;
    }
	gettimeofday(&end,NULL);
    printf("time: %ld:%ld\n", end.tv_sec - start.tv_sec,end.tv_usec - start.tv_usec);
	printf("number of gettimeofday: %ld\n\n",b);
    pthread_mutex_lock(&lock->pLock);
    pthread_mutex_lock(&lock->mLock);
    pthread_mutex_unlock(&lock->pLock);
    return 0;
}

int hybrid_lock_unlock(struct hybrid_lock *lock)       // 락을 해제하는 함수
{
    int i,j = 0;
    if(pthread_mutex_unlock(&lock->mLock)!=0){
        printf("unlock error\n");
        return -1;
    }
    return 0;
}
