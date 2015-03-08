#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t leaderQueue, followerQueue;

void *leader(void *a)
{
    int i = 0;
    while(++i){
        sem_post(&followerQueue);
        sem_wait(&leaderQueue);
        printf("leader%d\n", i);
    }
}

void *follower(void *a)
{
    int i = 0;
    while(++i){
        sem_post(&leaderQueue);
        sem_wait(&followerQueue);
        printf("follower%d\n", i);
    }
}

main()
{
    pthread_t t1, t2;

    sem_init(&leaderQueue, 0, 0);
    sem_init(&followerQueue, 0, 0);

    pthread_create(&t1, 0, &leader, 0);
    pthread_create(&t2, 0, &follower, 0);

    while(1);
}
