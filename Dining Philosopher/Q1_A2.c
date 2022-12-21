#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define N 5

sem_t BOWLS[N];
sem_t FORKS[N];

void* Philosopher_Function(void* num)
{
    int PH_ID = *(int*)num;
    int PREVIOUS_FORK = PH_ID;
    int NEXT_FORK = (PH_ID + 1) % N;

    while (1) {
        // Think
        printf("Philosopher : %d is THINKING ...\n", PH_ID+1);
        sleep(1);

        // Check if bowl is available
        sem_wait(&BOWLS[PH_ID]);

        // Check if FORKS are available
        sem_wait(&FORKS[PREVIOUS_FORK]);
        sem_wait(&FORKS[NEXT_FORK]);

        // Eat
        printf("Philosopher : %d is EATING ...\n", PH_ID+1);
        sleep(1);

        // Put down FORKS
        sem_post(&FORKS[PREVIOUS_FORK]);
        sem_post(&FORKS[NEXT_FORK]);

        // Put down bowl
        sem_post(&BOWLS[PH_ID]);
    }
}

int main(int argc, char* argv[])
{
    pthread_t THREAD[N];
    int IDS[N];

    // Initialize semaphores
    for (int i = 0; i < N; i++) {
        sem_init(&BOWLS[i], 0, 1);
        sem_init(&FORKS[i], 0, 1);
    }

    // Create Philosopher_Function threads
    for (int i = 0; i < N; i++) {
        IDS[i] = i;
        pthread_create(&THREAD[i], NULL, Philosopher_Function, &IDS[i]);
    }

    // Wait for Philosopher_Function threads to finish
    for (int i = 0; i < N; i++) {
        pthread_join(THREAD[i], NULL);
    }

    return 0;
}

