#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 5
#define BOWLS 2

int ORDERING[N] = { 0, 1, 2, 3, 4 }; // Strict ordering
pthread_mutex_t bowls;
pthread_mutex_t FORKS[N];

void* Philosopher_Function(void* num){
    int PH_ID = *(int*)num;
    int PREVIOUS_FORK = PH_ID;
    int NEXT_FORK = (PH_ID + 1) % N;

    while (1) {
        /////////////////         Think          //////////////////////////
        printf("Philosopher : %d is THINKING ...\n", PH_ID+1);
        sleep(1);

        //////////////  if a sauce bowl  available   ///////////////////////////
        pthread_mutex_lock(&bowls);

        ///////////////     if FORKS available  ////////////////////////
        pthread_mutex_lock(&FORKS[PREVIOUS_FORK]);
        pthread_mutex_lock(&FORKS[NEXT_FORK]);

        // Eat
        printf("Philosopher : %d is EATING ...\n", PH_ID+1);
        sleep(1);

        // Put down FORKS
        pthread_mutex_unlock(&FORKS[PREVIOUS_FORK]);
        pthread_mutex_unlock(&FORKS[NEXT_FORK]);

        // Put down sauce bowl
        pthread_mutex_unlock(&bowls);
    }
}

int main(){
    pthread_t THREAD[N];
    int IDS[N];


    pthread_mutex_init(&bowls, NULL);
    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&FORKS[i], NULL);
    }

    ///////////////////////////   THREAD     ///////////////////////
    for (int i = 0; i < N; i++) {
        IDS[i] = ORDERING[i];
        pthread_create(&THREAD[i], NULL, Philosopher_Function, &IDS[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(THREAD[i], NULL);
    }

    return 0;
}

