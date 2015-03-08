#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

int leaders, followers;
sem_t mutex, rendezvous, leaderQueue, followerQueue;

void *leader(void *a)
{
    int i = 0;
    while(++i){
        sem_wait(&mutex);
        if(followers > 0){
            followers--;
            sem_post(&followerQueue);
        }
        else{
            leaders++;
            sem_post(&mutex);
            sem_wait(&leaderQueue);
        }
        printf("leader\t\t%d\n", i);

        sem_wait(&rendezvous);
        sem_post(&mutex);
    }
}

void *follower(void *a)
{
    int i = 0;
    while(++i){
        sem_wait(&mutex);
        if(leaders > 0){
            leaders--;
            sem_post(&leaderQueue);
        }
        else{
            followers++;
            sem_post(&mutex);
            sem_wait(&followerQueue);
        }
        printf("follower\t%d\n", i);

        sem_post(&rendezvous);
    }
}

main()
{
    pthread_t t1, t2;

    sem_init(&mutex, 0, 1);
    sem_init(&rendezvous, 0, 0);
    sem_init(&leaderQueue, 0, 0);
    sem_init(&followerQueue, 0, 0);

    pthread_create(&t1, 0, &leader, 0);
    pthread_create(&t2, 0, &follower, 0);

    while(1);
}
