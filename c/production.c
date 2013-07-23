#include<pthread.h>
#include <unistd.h>
#include<semaphore.h>
#include<stdio.h>
#include<time.h>
#define BUFFSIZE 10
#define START 6
#define sem_signal(sem)  sem_post(sem)
pthread_mutex_t allmutex, consumutex, produmutex;
sem_t product, slot;
void* producer(void* arg)
{
	pthread_mutex_lock(&produmutex);
	sem_wait(&slot);
	pthread_mutex_lock(&allmutex);
	sem_signal(&product);
	pthread_mutex_unlock(&allmutex);
	pthread_mutex_unlock(&produmutex);
}
void* consumer(void* arg)
{
	int val;
	pthread_mutex_lock(&consumutex);
	sem_wait(&product);
	pthread_mutex_lock(&allmutex);
	sem_getvalue(&product,&val);
	printf("%d\n",val);
	pthread_mutex_unlock(&allmutex);
	sem_signal(&slot);
	pthread_mutex_unlock(&consumutex);
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
	pthread_t l1, l2, l3, l4;
        sem_init(&product,0,START);
        sem_init(&slot,0,BUFFSIZE-START);
        pthread_mutex_init(&produmutex,0);
        pthread_mutex_init(&consumutex,0);
	pthread_create(&l1, 0, loop, consumer);
	pthread_create(&l2, 0, loop, consumer);
	pthread_create(&l3, 0, loop, producer);
	pthread_create(&l4, 0, loop, producer);
	while(1);
}
