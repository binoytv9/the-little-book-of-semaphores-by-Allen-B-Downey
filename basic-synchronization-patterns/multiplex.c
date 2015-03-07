#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#define N 50 // maximum number of threads
#define n 10 // maximum number of concurrent threads

int count;
sem_t multiplex;

void *threadN(void *a)
{
    sem_wait(&multiplex);
    // critical section
    int sval;
    sem_getvalue(&multiplex, &sval);

    printf("thread%d semVal : %d\n", *(int *)a, sval);
    count += 1;
    sem_post(&multiplex);
}

main()
{
    int i;
    pthread_t t[N];
    sem_init(&multiplex, 0, n);

    printf("count before : %d\n", count);

    for(i = 0; i < N; ++i)
        pthread_create(&t[i], 0, threadN, &i);
    
    for(i = 0; i < N; ++i)
        pthread_join(t[i], 0);


    printf("count after : %d\n", count);
}
