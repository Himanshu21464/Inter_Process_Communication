#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 5

int ORDERING[N] = { 0, 1, 2, 3, 4 };
pthread_mutex_t BOWLS[N];
pthread_mutex_t FORKS[N];

void* Philosopher_Function(void* num)
{
    int PH_ID = *(int*)num;
    int PREVIOUS_FORK = PH_ID;
    int NEXT_FORK = (PH_ID + 1) % N;

    while (1) {
        // Think
        printf("Philosopher : %d is THINKING ...\n", PH_ID+1);
        sleep(1);

        // if bowl available
        while (pthread_mutex_trylock(&BOWLS[PH_ID]) != 0) {
            sleep(1);
        }

        // if FORKS available
        while (pthread_mutex_trylock(&FORKS[PREVIOUS_FORK]) != 0 ||
               pthread_mutex_trylock(&FORKS[NEXT_FORK]) != 0) {
            pthread_mutex_unlock(&FORKS[PREVIOUS_FORK]);
            pthread_mutex_unlock(&FORKS[NEXT_FORK]);
            sleep(1);
        }

        ///////////    EatING     /////////////////////////////
        printf("Philosopher : %d is EATING ...\n", PH_ID+1);
        sleep(1);

        // Put down FORKS
        pthread_mutex_unlock(&FORKS[PREVIOUS_FORK]);
        pthread_mutex_unlock(&FORKS[NEXT_FORK]);

        // Put down bowl
        pthread_mutex_unlock(&BOWLS[PH_ID]);
    }
}

int main()
{
    pthread_t THREAD[N];
    int ids[N];

    // InitialIZE MUTEX ////////////////////
    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&BOWLS[i], NULL);
        pthread_mutex_init(&FORKS[i], NULL);
    }

    //          THREAD        ////
    for (int i = 0; i < N; i++) {
        ids[i] = ORDERING[i];
        pthread_create(&THREAD[i], NULL, Philosopher_Function, &ids[i]);
    }

    
    for (int i = 0; i < N; i++) {
        pthread_join(THREAD[i], NULL);
    }

    return 0;
}
