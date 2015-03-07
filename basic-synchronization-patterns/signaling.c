#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>

#define MAXLEN 10

sem_t sem;
char line[MAXLEN];

void *read_line(void *a)
{
    printf("\n%s", (char *)a);

    fgets(line, MAXLEN, stdin);     // reading line
    line[strlen(line)-1] = '\0';    // removing the trailing '\n'

    sem_post(&sem);
}

void *print_line(void *a)
{
    sem_wait(&sem);

    printf("%s%s\n", (char *)a, line);
}

main()
{
    pthread_t t1, t2;

    sem_init(&sem, 0, 0);

    pthread_create(&t1, 0, read_line, "enter a line : ");
    pthread_create(&t2, 0, print_line, "line is : ");

    pthread_join(t1, 0);
    pthread_join(t2, 0);
}
