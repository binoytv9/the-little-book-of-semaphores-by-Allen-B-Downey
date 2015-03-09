#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#define bufSize 100

sem_t mutex, items, spaces;

int i, j, k;
char *buffer[bufSize];
char *eventBuf[] = {    
                        "getup",
                        "brush",
                        "bath",
                        "breakfast",
                        "get ready",
                        "goto school",
                        "attnd assembly",
                        "goto class",
                        "study",
                        "lunch",
                        "again study",
                        "goto home",
                        "drink tea",
                        "play",
                        "again bath",
                        "study",
                        "dinner",
                        "sleep"
                    };
int eventNum = sizeof(eventBuf) / sizeof(char *);

void *producer(void *a);
void *consumer(void *a);
char *waitForEvent(void);
char *get(char **buffer);
void process(char *event);
void add(char **buffer, char *event);

main()
{
    pthread_t t1, t2;

    sem_init(&mutex, 0, 1);
    sem_init(&items, 0, 0);
    sem_init(&spaces, 0, bufSize);

    pthread_create(&t1, 0, consumer, 0);
    pthread_create(&t2, 0, producer, 0);

    while(1);
}

void *consumer(void *a)
{
    char *event;

    while(1){
        sem_wait(&items);
        sem_wait(&mutex);
            event = get(buffer);
        sem_post(&mutex);
        sem_post(&spaces);

        process(event);
    }
}

char *get(char **buffer)
{
    return buffer[k++ % bufSize];
}

void process(char *event)
{
    printf("%s\n", event);
}

void *producer(void *a)
{
    char *event;

    while(1){
        event = waitForEvent();

        sem_wait(&spaces);
        sem_wait(&mutex);
            add(buffer, event);
        sem_post(&mutex);
        sem_post(&items);
    }
}

char *waitForEvent(void)
{
    return eventBuf[i++ % eventNum];
}

void add(char **buffer, char *event)
{
    buffer[j++ % bufSize] = event;
}
