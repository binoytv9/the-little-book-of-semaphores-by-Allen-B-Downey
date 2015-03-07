#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t aArrived, bArrived;

void *thread1(void *a)
{
    printf("statement a1\n");
    sem_post(&aArrived);
    sem_wait(&bArrived);
    printf("statement a2\n");
}

void *thread2(void *a)
{
    printf("statement b1\n");
    sem_post(&bArrived);
    sem_wait(&aArrived);
    printf("statement b2\n");
}

main()
{
    pthread_t t1, t2;

    sem_init(&aArrived, 0, 0);
    sem_init(&bArrived, 0, 0);

    pthread_create(&t1, 0, thread1, 0);
    pthread_create(&t2, 0, thread2, 0);

    pthread_join(t1, 0);
    pthread_join(t2, 0);
}
