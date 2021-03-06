#include<stdio.h>

typedef pthread_mutex_t Mutex;
typedef pthread_cond_t Cond;

typedef struct{
	int value, wakeups;
	Mutex *mutex;
	Cond *cond;
} Semaphore;


Mutex *make_mutex()
{
	Mutex *mutex = check_malloc(sizeof(Mutex));
	int n = pthread_mutex_init(mutex, NULL);
	if(n != 0)
		perror_exit("make_lock failed");

	return mutex;
}

void mutex_lock(Mutex *mutex)
{
	int n = pthread_mutex_lock(mutex);
	if(n != 0)
		perror_exit("lockfailed");
}

void mutex_unlock(Mutex *mutex)
{
	int n = pthread_mutex_unlock(Mutex *mutex);
	if(n != 0)
		perror_exit("unlock failed");
}

Cond *make_cond()
{
	Cond *cond = check_malloc(sizeof(Cond));
	int n = pthread_cond_init(cond, NULL);
	if(n != 0)
		perror_exit("make_cond failed");

	return cond;
}

void cond_wait(Cond *cond, Mutex *mutex)
{
	int n = pthread_cond_wait(cond, mutex);
	if(n != 0)
		perror_exit("cond_wait failed");
}

void cond_signal(Cond *cond)
{
	int n = pthread_cond_signal(cond);
	if(n != 0)
		perror_exit("cond_signal failed");
}

Semaphore *make_semaphore(int value)
{
	Semaphore *semaphore = check_malloc(sizeof(Semaphore));
	semaphore->value = value;
	semaphore->wakeups = 0;
	semaphore->mutex = make_mutex();
	semaphore->cond = make_cond();

	return semaphore;
}

void sem_wait(Semaphore *semaphore)
{
	mutex_lock(semaphore->mutex);
	semaphore->value--;

	if(semaphore->value < 0){
		do{
			cond_wait(semaphore->cond, semaphore->mutex);
		} while(semaphore->wakeups < 1);
		semaphore->wakeups--;
	}
	mutex_unlock(semaphore->mutex);
}

void sem_signal(Semaphore *semaphore)
{
	mutex_lock(semaphore->mutex);
	semaphore->value++;

	if(semaphore->value <= 0){
		semaphore->wakeups++;
		cond_signal(semaphore->cond);
	}
	mutex_unlock(semaphore->mutex);
}
