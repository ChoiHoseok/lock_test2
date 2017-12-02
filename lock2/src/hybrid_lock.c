#include "hybrid_lock.h"

int hybrid_lock_init(struct hybrid_lock *lock)     //락을 초기화하는 함수
{
    lock->g_count = 0;
    if(pthread_mutex_init(&lock->mLock, NULL)!=0){
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
    return 0;
}

int hybrid_lock_lock(struct hybrid_lock *lock)     //락을 점유하는 함수
{
    printf("lock\n");
    struct timeval start2, end2;
    gettimeofday(&start2,NULL);
    gettimeofday(&end2, NULL);
    //clock_t start=clock();
    //clock_t end= CLOCKS_PER_SEC;
    while(!(end2.tv_usec > start2.tv_usec && start2.tv_sec < end2.tv_sec)){
        if(pthread_mutex_trylock(&lock-> mLock) == 0){        //락이 획득됐을때.
            return 0;
        }
        gettimeofday(&end2,NULL);
    }

    printf("%ld:%ld\n",end2.tv_sec - start2.tv_sec, end2.tv_usec - start2.tv_usec);
    //락이 1초동안 획득 되지 못했을때 mutex락으로 전환.
    pthread_mutex_lock(&lock->mLock);
    return 0;
}

int hybrid_lock_unlock(struct hybrid_lock *lock)       // 락을 해제하는 함수
{
    if(pthread_mutex_unlock(&lock->mLock)!=0){
        printf("unlock error\n");
        return -1;
    }
    return 0;
}
