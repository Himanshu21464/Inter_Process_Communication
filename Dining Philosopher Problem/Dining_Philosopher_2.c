#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_BUFFERS 10
char BUFFER [MAX_BUFFERS] [100];
int BUFF_INDEX;
int BPI;

pthread_mutex_t BUFFER_MUTEX = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t BUFFER_CONDITION = PTHREAD_COND_INITIALIZER;
pthread_cond_t SPOOL_CONDITION = PTHREAD_COND_INITIALIZER;
int BUFF_LEFT = MAX_BUFFERS;
int LINE = 0;

pthread_mutex_t fork_mutex [5] = {PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER,
                                  PTHREAD_MUTEX_INITIALIZER};


void STRING (const char *const str){
    int r;
    if ((r = pthread_mutex_lock (&BUFFER_MUTEX)) != 0) {
        printf("error");exit(EXIT_FAILURE);
    }
        while (!BUFF_LEFT) 
            pthread_cond_wait (&BUFFER_CONDITION, &BUFFER_MUTEX);
        int j = BUFF_INDEX;
        BUFF_INDEX++;
        if (BUFF_INDEX == MAX_BUFFERS)
            BUFF_INDEX = 0;
        BUFF_LEFT--;

	strcpy (BUFFER [j], str);
        LINE++;
        pthread_cond_signal (&SPOOL_CONDITION);
    if ((r = pthread_mutex_unlock (&BUFFER_MUTEX)) != 0) {
        printf("error");exit(EXIT_FAILURE);
    }
    
}

void *PHILOSOPHER (void *arg){
    int i, r, LEFT_FORK, RIGHT_FORK;
    int PHILOSOPHER_NUM = *((int *) arg);
    char P_STRING [100];
    while (1) {
        bool eat = rand () % 2;	
	if (eat) {
            sprintf (P_STRING, "Philosopher %d: Will eat!\n", PHILOSOPHER_NUM+1);
            STRING (P_STRING);
	    LEFT_FORK = (PHILOSOPHER_NUM == 4) ? 0 : PHILOSOPHER_NUM;
            if ((r = pthread_mutex_lock (&fork_mutex [LEFT_FORK])) != 0) {
                printf("error");exit(EXIT_FAILURE);
            }
	    RIGHT_FORK = (PHILOSOPHER_NUM == 4) ? 4 : PHILOSOPHER_NUM + 1;
            if ((r = pthread_mutex_lock (&fork_mutex [RIGHT_FORK])) != 0) {
                fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (EXIT_FAILURE);
            }
            sprintf (P_STRING, "Philosopher %d: Got forks: %d and %d\n", PHILOSOPHER_NUM+1, LEFT_FORK+1, RIGHT_FORK+1);
            STRING (P_STRING);
            sprintf (P_STRING, "Philosopher %d:  Eating....\n", PHILOSOPHER_NUM+1);
            STRING (P_STRING);
            sleep (rand () % 5 + 1);
            if ((r = pthread_mutex_unlock (&fork_mutex [LEFT_FORK])) != 0) {
                printf("error");exit(EXIT_FAILURE);
            }
	 
            if ((r = pthread_mutex_unlock (&fork_mutex [RIGHT_FORK])) != 0) {
                printf("error");exit(EXIT_FAILURE);
            }
	}
	else { 
            sprintf (P_STRING, "Philosopher %d: Thinking....\n", PHILOSOPHER_NUM+1);
            STRING (P_STRING);
            sleep (rand () % 5 + 1);
	}
        sprintf (P_STRING, "Philosopher %d: Finished %s", PHILOSOPHER_NUM+1, (eat) ? "eating.\n" : "thinking.\n");
        STRING (P_STRING);
    }
}


void *TEMP_FUNCTION (void *arg){
    int r;

    while (1) {  
    
        if ((r = pthread_mutex_lock (&BUFFER_MUTEX)) != 0) {
            printf("error");exit(EXIT_FAILURE);
        }
            while (!LINE) 
                pthread_cond_wait (&SPOOL_CONDITION, &BUFFER_MUTEX);
            printf ("%s", BUFFER [BPI]);
            LINE--;
            BPI++;
            if (BPI == MAX_BUFFERS)
               BPI = 0;
            BUFF_LEFT++;
            pthread_cond_signal (&BUFFER_CONDITION);
        if ((r = pthread_mutex_unlock (&BUFFER_MUTEX)) != 0) {
            printf("error");exit(EXIT_FAILURE);
        }

    }
}


int main (int argc, char **argv){
    pthread_t THREAD [5], THREAD_TEMP;
    int i, r;
    BUFF_INDEX = BPI = 0;
    time_t now = time (NULL);
    srand ((unsigned int) (now % 937));
    if ((r = pthread_create (&THREAD_TEMP, NULL, TEMP_FUNCTION, NULL)) != 0) {
        printf("error");exit(EXIT_FAILURE);
    }
    int THREAD_PH [5];
    for (i = 0; i < 5; i++) {
        THREAD_PH [i] = i;
        if ((r = pthread_create (&THREAD [i], NULL, PHILOSOPHER, (void *) &THREAD_PH [i])) != 0) {
            printf("error");exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < 5; i++)
        if ((r = pthread_join (THREAD [i], NULL)) == -1) {
            printf("error");exit(EXIT_FAILURE);
        }
    while (LINE) sleep (1);
    if ((r = pthread_cancel (THREAD_TEMP)) != 0) {
        printf("error");exit(EXIT_FAILURE);
    }
    exit (EXIT_SUCCESS);
}