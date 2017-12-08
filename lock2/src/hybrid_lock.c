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
    struct timeval start,end;
    long a = 0;
    gettimeofday(&start,NULL);
    while(1){
        if(pthread_mutex_trylock(&lock-> mLock) == 0){
            gettimeofday(&end,NULL);
            //printf("trylock catch!! %ld:%ld\n",end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
            return 0;
        }
        if(a > 10000000){
            gettimeofday(&end,NULL);
            //printf("%ld:%ld\n",end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
            if(end.tv_sec > start.tv_sec && end.tv_usec > start.tv_usec){
                //printf("%ld:%ld\n",end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
                break;
            }
            continue;
        }
        a++;
    }
    //gettimeofday(&end,NULL);
    //printf("%ld:%ld\n",end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
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
    usleep(10);
    return 0;
}
