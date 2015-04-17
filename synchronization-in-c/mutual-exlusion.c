#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define NUM_CHILDREN 10

typedef sem_t Semaphore;

typedef struct{
	int counter;
	int end;
	int *array;
	Semaphore *mutex;
} Shared;

void errExit(char *msg);
void *entry(void *shared);
Shared *make_shared(int end);
void *check_malloc(int size);
void child_code(Shared *shared);
Semaphore *make_semaphore(int n);
void check_array(Shared *shared);
void join_thread(pthread_t thread);
pthread_t make_thread(void *(*entry)(void *), Shared *shared);

int main()
{
	int i;
	pthread_t child[NUM_CHILDREN];

	Shared *shared = make_shared(100000);

	for(i = 0; i < NUM_CHILDREN; i++)
		child[i] = make_thread(entry, shared);

	for(i = 0; i < NUM_CHILDREN; ++i)
		join_thread(child[i]);

	check_array(shared);

	return 0;
}

Shared *make_shared(int end)
{
	int i;
	Shared *shared = check_malloc(sizeof(Shared));

	shared->counter = 0;
	shared->end = end;
	shared->mutex = make_semaphore(1);;

	shared->array = check_malloc(shared->end * sizeof(int));
	for(i = 0; i < shared->end; ++i)
		shared->array[i] = 0;

	return shared;
}

Semaphore *make_semaphore(int n)
{
	Semaphore *sem = check_malloc(sizeof(Semaphore));
	int ret = sem_init(sem, 0, n);
	if(ret == -1)
		errExit("sem_init");

	return sem;
}

void child_code(Shared *shared)
{
	while(1){
		sem_wait(shared->mutex);

		if(shared->counter >= shared->end){
			sem_post(shared->mutex);
			return;
		}
		shared->array[shared->counter]++;
		shared->counter++;

		sem_post(shared->mutex);
	}
}

void check_array(Shared *shared)
{
	int i, errors = 0;

	for(i = 0; i < shared->end; ++i)
		if(shared->array[i] != 1)
			errors++;

	printf("%d errors\n", errors);
}

void *check_malloc(int size)
{
	void *p = malloc(size);
	if(p == NULL)
		errExit("malloc");

	return p;
}

pthread_t make_thread(void *(*entry)(void *), Shared *shared)
{
	int n;
	pthread_t thread;

	n = pthread_create(&thread, NULL, entry, shared);
	if(n != 0)
		errExit("pthread_create");

	return thread;
}

void *entry(void *shared)
{
	child_code(shared);
	pthread_exit(NULL);
}

void join_thread(pthread_t thread)
{
	int ret = pthread_join(thread, NULL);
	if(ret == -1)
		errExit("pthread_join");
}

void errExit(char *msg)
{
	perror(msg);
	exit(1);
}
