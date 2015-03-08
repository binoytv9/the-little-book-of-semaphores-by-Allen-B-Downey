#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define num 10    // number of threads

struct barrier{
    int n;
    int count;
    sem_t mutex;
    sem_t turnstile;
    sem_t turnstile2;
};

struct barrier *Barrier(int n)
{
    struct barrier *self = (struct barrier *)malloc(sizeof(struct barrier));

    self->n = n;
    self->count = 0;
    sem_init(&self->mutex, 0, 1);
    sem_init(&self->turnstile, 0, 0);
    sem_init(&self->turnstile2, 0, 0);

    return self;
}

void phase1(struct barrier *self)
{
    int i;

    sem_wait(&self->mutex);
    self->count += 1;
    if(self->count == self->n)
        for(i = 0; i < self->n; ++i)
            sem_post(&self->turnstile);
    sem_post(&self->mutex);

    sem_wait(&self->turnstile);
}

void phase2(struct barrier *self)
{
    int i;

    sem_wait(&self->mutex);
    self->count -= 1;
    if(self->count == 0)
        for(i = 0; i < self->n; ++i)
            sem_post(&self->turnstile2);
    sem_post(&self->mutex);

    sem_wait(&self->turnstile2);
}

void wait(struct barrier *self)
{
    phase1(self);
    phase2(self);
}

void *thread(void *a)
{
    struct barrier *ba;

    ba = Barrier(num);
    wait(ba);
}

main()
{
    int i;
    pthread_t t[num];

    for(i = 0; i < num; ++i)
        pthread_create(&t[i], 0, thread, &i);

}
