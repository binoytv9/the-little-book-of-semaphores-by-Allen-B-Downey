#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#define n 10    // number of threads

int count;
sem_t mutex, barrier;

void *thread(void *a)
{
    sem_wait(&mutex);
    count += 1;
    sem_post(&mutex);

    if(count == n)
        sem_post(&barrier);

    sem_wait(&barrier);
    sem_post(&barrier);

    printf("thread%d critical section\n", *(int *)a);
}

main()
{
    int i;
    sem_init(&mutex, 0, 1);
    sem_init(&barrier, 0, 0);

    pthread_t t[n];

    for(i = 0; i < n; ++i)
        pthread_create(&t[i], 0, thread, &i);

    for(i = 0; i < n; ++i)
        pthread_join(t[i], 0);

    printf("count is %d\n", count);
}
