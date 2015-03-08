#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#define n 10    // number of threads

int count;
sem_t mutex, turnstile, turnstile2;

void *thread(void *a)
{
    int i;

    sem_wait(&mutex);
    count += 1;
    if(count == n)
        for(i = 0; i < n; ++i)
            sem_post(&turnstile);   // unlock the first
    sem_post(&mutex);

    sem_wait(&turnstile);           // first turnstile

    sem_wait(&mutex);
    count -= 1;
    if(count == 0)
        for(i = 0; i < n; ++i)
            sem_post(&turnstile2);  // unlock the second
    sem_post(&mutex);

    sem_wait(&turnstile2);          // second turnstile
}

main()
{
    int i;
    sem_init(&mutex, 0, 1);
    sem_init(&turnstile, 0, 0);
    sem_init(&turnstile2, 0, 1);

    pthread_t t[n];

    for(i = 0; i < n; ++i)
        pthread_create(&t[i], 0, thread, &i);

    for(i = 0; i < n; ++i)
        pthread_join(t[i], 0);

}
