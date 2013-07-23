#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<stdio.h>
#include<time.h>
#define sem_signal(sem) sem_post(sem)

pthread_mutex_t roomempty, noreader, writers;
int BUFF = 0,numreader = 0;

void lockwriter()
{
	pthread_mutex_lock(&noreader);
	numreader += 1;
	if (numreader == 1)
		pthread_mutex_lock(&roomempty);
	pthread_mutex_unlock(&noreader);
}

void unlockwriterifempty()
{
	pthread_mutex_lock(&noreader);
	numreader -= 1;
	if(numreader == 0)
		pthread_mutex_unlock(&roomempty);
	pthread_mutex_unlock(&noreader);
}
	
void writer()
{
	pthread_mutex_lock(&writers);
	pthread_mutex_lock(&roomempty);
	printf("\n w");
	BUFF += 1;
	pthread_mutex_unlock(&writers);
	pthread_mutex_unlock(&roomempty);
}

void reader()
{
	pthread_mutex_lock(&writers);
	pthread_mutex_unlock(&writers);
	lockwriter();
	printf("%d ",BUFF);
	unlockwriterifempty();
}

void* loop(void* (*arg)(void*))
{
	int r;
	while(1){
		r = random()%100000+100000;
		usleep((useconds_t)r);
		arg(0);
	}
}

int main()
{
	pthread_t l1, l2, l3, l4, l5;
	pthread_mutex_init(&writers, 0);
	pthread_mutex_init(&roomempty, 0);
	pthread_mutex_init(&noreader, 0);
	pthread_create(&l1, 0, loop, reader);
	pthread_create(&l2, 0, loop, reader);
	pthread_create(&l3, 0, loop, reader);
	pthread_create(&l4, 0, loop, writer);
	pthread_create(&l5, 0, loop, writer);
	while(1);
}
