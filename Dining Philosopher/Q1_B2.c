#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define N 5
#define NUM_SAUCE_BOWLS 2

sem_t SAUCE_BOWLS;
sem_t FORKS[N];

void* Philosopher_Function(void* num)
{
    int PH_ID = *(int*)num;
    int PREVIOUS_FORK = PH_ID;
    int NEXT_FORK = (PH_ID + 1) % N;

    while (1) {
        ///////////////////////// Think////////////////////////////////////////
        printf("Philosopher : %d is THINKING ...\n", PH_ID+1);
        sleep(1);

        /////////////////////Check for sauce bowl////////////////////////////
        sem_wait(&SAUCE_BOWLS);

        /////////////////////// Check for FORKS ////////////////////////
        sem_wait(&FORKS[PREVIOUS_FORK]);
        sem_wait(&FORKS[NEXT_FORK]);

        // Eat
        printf("Philosopher : %d is EATING ...\n", PH_ID+1);
        sleep(1);

        //forks down
        sem_post(&FORKS[PREVIOUS_FORK]);
        sem_post(&FORKS[NEXT_FORK]);

        //sauce bowl down
        sem_post(&SAUCE_BOWLS);
    }
}

int main(){
    pthread_t THREAD[N];
    int IDS[N];

    ////////////////////////   semaphores Initialization   /////////////////
    sem_init(&SAUCE_BOWLS, 0, NUM_SAUCE_BOWLS);
    for (int i = 0; i < N; i++) {
        sem_init(&FORKS[i], 0, 1);
    }

    //////////////////////   THREAD CREATION     ////////////////////////
    for (int i = 0; i < N; i++) {
        IDS[i] = i;
        pthread_create(&THREAD[i], NULL, Philosopher_Function, &IDS[i]);
    }
    
    for (int i = 0; i < N; i++) {
        pthread_join(THREAD[i], NULL);
    }

    return 0;
}

