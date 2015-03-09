#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t forks[5], footman;

int left(int i);
int right(int i);
void *get_forks(void *a);
void *put_forks(void *a);

void main(void)
{
    int i;
    pthread_t t[5];

    sem_init(&footman, 0, 4);
    for(i = 0; i < 5; ++i)
        sem_init(&forks[i], 0, 1);


    for(i = 0; i < 5; ++i){
        printf("i = %d\n", i);
        pthread_create(&t[i], 0, get_forks, &i);
        pthread_create(&t[i], 0, put_forks, &i);
    }

    while(1);
}

void *get_forks(void *a)
{
    int i = *(int *)a;
    printf("get_forks[%d]\n", i);
    sem_wait(&footman);
    sem_wait(&forks[right(i)]);
    sem_wait(&forks[left(i)]);
}

void *put_forks(void *a)
{
    int i = *(int *)a;
    printf("put_forks[%d]\n", i);
    sem_wait(&footman);
    sem_post(&forks[right(i)]);
    sem_post(&forks[left(i)]);
    sem_post(&footman);
}

int left(int i)
{
    return i;
}

int right(int i)
{
    return (i + 1) % 5;
}
