#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include<unistd.h>
#define NUMBER 5
int SOCHRA_HAI=2;
int BHOOKA_HAI=1;
int KHA_RAHA_HAI=0;
#define LEFT (PHILOSOPHER_NUMBER + 4) % NUMBER
#define RIGHT (PHILOSOPHER_NUMBER + 1) % NUMBER

int MODE[NUMBER];
int PHILOSOPHER[NUMBER] = { 0, 1, 2, 3, 4 };

sem_t mutex;
sem_t S[NUMBER];

void CHECK(int PHILOSOPHER_NUMBER){
	if (MODE[PHILOSOPHER_NUMBER] == BHOOKA_HAI
		&& MODE[LEFT] != KHA_RAHA_HAI
		&& MODE[RIGHT] != KHA_RAHA_HAI) {
		MODE[PHILOSOPHER_NUMBER] = KHA_RAHA_HAI;
		sleep(1);
		printf("Philosopher %d takes fork %d and %d\n",
					PHILOSOPHER_NUMBER + 1, LEFT + 1, PHILOSOPHER_NUMBER + 1);
		printf("Philosopher %d is KHA_RAHA_HAI\n", PHILOSOPHER_NUMBER + 1);
		sem_post(&S[PHILOSOPHER_NUMBER]);
	}
}

void PUT_FORK_UP(int PHILOSOPHER_NUMBER){
	sem_wait(&mutex);
	MODE[PHILOSOPHER_NUMBER] = BHOOKA_HAI;
	printf("Philosopher %d is BHOOKA\n", PHILOSOPHER_NUMBER + 1);
	CHECK(PHILOSOPHER_NUMBER);
	sem_post(&mutex);
	sem_wait(&S[PHILOSOPHER_NUMBER]);
	sleep(1);
}

void PUT_FORK_DOWN(int PHILOSOPHER_NUMBER){
	sem_wait(&mutex);
	MODE[PHILOSOPHER_NUMBER] = SOCHRA_HAI;
	printf("Philosopher %d putting fork %d and %d down\n",PHILOSOPHER_NUMBER + 1, LEFT + 1, PHILOSOPHER_NUMBER + 1);
	printf("Philosopher %d is thinking\n", PHILOSOPHER_NUMBER + 1);
	CHECK(LEFT);
	CHECK(RIGHT);
	sem_post(&mutex);
}

void* PHILOSOPHER_N(void* num){
	while (1) {
		int* i = num;
		sleep(1);
		PUT_FORK_UP(*i);
		sleep(0);
		PUT_FORK_DOWN(*i);
	}
}
int main(){
	int i;
	pthread_t thread_id[NUMBER];
	sem_init(&mutex, 0, 1);
	for (i = 0; i < NUMBER; i++)
		sem_init(&S[i], 0, 0);
	for (i = 0; i < NUMBER; i++) {
		pthread_create(&thread_id[i], NULL,
					PHILOSOPHER_N, &PHILOSOPHER[i]);
		printf("Philosopher %d is SOCHRA_HAI\n", i + 1);
	}
	for (i = 0; i < NUMBER; i++)
		pthread_join(thread_id[i], NULL);
}
