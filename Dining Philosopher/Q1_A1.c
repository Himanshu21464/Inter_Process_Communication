#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>

#define N 5  // Number of philosophers

int FORKS[N];  // Array to represent the FORKS

int main()
{
  int i;

  while (1) {
    for (i = 0; i < N; i++) {
      // Think
      printf("Philosopher : %d is THINKING\n", i);
      sleep(1);

      // Acquire FORKS in a predetermined order
      if (i % 2 == 0) {
        // Pick left FORK
        FORKS[i] = 1;
        printf("Philosopher : %d picked up left FORK\n", i);
        // Pick right FORK
        FORKS[(i + 1) % N] = 1;
        printf("Philosopher : %d picked up right FORK\n", i);
      } else {
        // Pick right FORK
        FORKS[(i + 1) % N] = 1;
        printf("Philosopher : %d picked up right FORK\n", i);
        // Pick left FORK
        FORKS[i] = 1;
        printf("Philosopher : %d picked up left FORK\n", i);
      }

      // Eat
      printf("Philosopher : %d is EATING\n", i);
      sleep(1);
      printf("Philosopher : %d stopped EATING\n", i);


      // Put down FORKS
      FORKS[i] = 0;
      FORKS[(i + 1) % N] = 0;
    }
  }

////-----------THIS PROGRAM IS RUNNING BY ONLY ONE THREAD( MAIN THREAD)-----------------
  return 0;
}
