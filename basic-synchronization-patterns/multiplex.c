#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#define N 10 // maximum number of concurrent threads

int count;
sem_t multiplex;

void *thread1(void *a)
{
    sem_wait(&multiplex);
    // critical section
    count += 1;
    sem_post(&multiplex);
}

void *thread2(void *a)
{
    sem_wait(&multiplex);
    // critical section
    count += 1;
    sem_post(&multiplex);
}
.
.
.
void *threadN(void *a)
{
    sem_wait(&multiplex);
    // critical section
    count += 1;
    sem_post(&multiplex);
}


main()
{
    pthread_t t1, t2, ... tN;
    sem_init(&multiplex, 0, N);

    printf("count before : %d\n", count);


    pthread_create(&t1, 0, thread1, 0);
    pthread_create(&t2, 0, thread2, 0);
    .
    .
    .
    pthread_create(&tN, 0, threadN, 0);


    pthread_join(t1, 0);
    pthread_join(t2, 0);
    .
    .
    .
    pthread_join(tN, 0);


    printf("count after : %d\n", count);
}
