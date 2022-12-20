#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t Plate;
sem_t FORKS[5];
int P_Count[5];
char MODE[5];
char Souce_Bowl[2];
int Bowl_Count[2];
int Eaten_Status[5];

int PREVIOUS_FORK_OR_OPHILOSOPHER(int p){
    return (p+4)%5;
}

int NEXT_FORK_OR_OPHILOSOPHER(int p){
    return (p+1)%5;
}

int Get_Bowl(){
    for(int i=0;i<2;i++){
        if(Souce_Bowl[i]=='V'){
            return i;
        }
    }
    return -1;
}

int Get_Philosopher_Number(int inde){
    for(int i=0;i<4;i++){
        if(Bowl_Count[i]==inde){
            return i;
        }
    }
    return -1;
}

void*PHILOSOPHER(void*i){
    do{  
        int*j=i;
        int k = *j;

        if(sleep(1)<0){
            printf("error: wasn't able to call sleep function");
        }
        
        if(sem_wait(&Plate)<0){
            printf("error: sem_wait did not work\n");
        }
        MODE[k]='H';
        int RESULT=Get_Bowl();
        printf("philosopher %d is Hungry\n\n",k+1);
        if(MODE[k]=='H'&&MODE[PREVIOUS_FORK_OR_OPHILOSOPHER(k)]!='E'&&MODE[NEXT_FORK_OR_OPHILOSOPHER(k)]!='E'&&RESULT!=-1){
            MODE[k]='E';
            Souce_Bowl[RESULT]='E';
            Bowl_Count[RESULT]=k;
            Eaten_Status[k]++;
            if(sleep(1)<0){
                printf("error: wasn't able to use sleep function");
            }
            printf("philosopher %d has picked up the FORKS %d and %d \n\nHe is EATING from Souce_Bowl %d\n\n",k+1,k+1,NEXT_FORK_OR_OPHILOSOPHER(k)+1,RESULT+1);
            if(sem_post(&FORKS[k])<0){
                printf("error: sem_post did not work\n");
            }
        }
        if(sem_post(&Plate)<0){
            printf("error: sem_post did not work\n");
        }
        if(sem_wait(&FORKS[k])<0){
            printf("error: sem_wait did not work\n");
        }

        if(sleep(1)<0){
            printf("error: wasn't able to call sleep function");
        }

        if(sem_wait(&Plate)<0){
            printf("error: sem_wait did not work\n");
        }
        MODE[k]='T';
        int checker=Get_Philosopher_Number(k);
        if(checker!=-1){
            Souce_Bowl[checker]='V';
            Bowl_Count[checker]=-1;
        }
        printf("philosopher %d is Putting down the FORKS %d and %d and his Souce_Bowl\n\n",k+1,k+1,NEXT_FORK_OR_OPHILOSOPHER(k)+1);
        printf("Philosopher %d Is THINKING\n\n",k+1);
        int l=PREVIOUS_FORK_OR_OPHILOSOPHER(k);
        int r=NEXT_FORK_OR_OPHILOSOPHER(k);
        RESULT=Get_Bowl();
        if(MODE[l]=='H'&&MODE[PREVIOUS_FORK_OR_OPHILOSOPHER(l)]!='E'&&MODE[NEXT_FORK_OR_OPHILOSOPHER(l)]!='E'&&RESULT!=-1){
            MODE[l]='E';
            Souce_Bowl[RESULT]='E';
            Bowl_Count[RESULT]=l;
            Eaten_Status[l]++;
            if(sleep(1)<0){
                printf("error: wasn't able to use sleep function");
            }
            printf("philosopher %d picked up the FORKS %d and %d \n\nHe is EATING from Souce_Bowl %d\n\n",l+1,l+1,NEXT_FORK_OR_OPHILOSOPHER(l)+1,RESULT+1);
            if(sem_post(&FORKS[l])<0){
                printf("error: sem_post did not work\n");
            }
        }
        RESULT=Get_Bowl();
        if(MODE[r]=='H'&&MODE[PREVIOUS_FORK_OR_OPHILOSOPHER(r)]!='E'&&MODE[NEXT_FORK_OR_OPHILOSOPHER(r)]!='E'&&RESULT!=-1){
            MODE[r]='E';
            Souce_Bowl[RESULT]='E';
            Bowl_Count[RESULT]=r;
            Eaten_Status[r]++;
            if(sleep(1)<0){
                printf("error: wasn't able to use sleep function");
            }
            printf("philosopher %d picked up the FORKS %d and %d \n\nHe is EATING from Souce_Bowl %d\n\n",r+1,r+1,NEXT_FORK_OR_OPHILOSOPHER(r)+1,RESULT+1);
            if(sem_post(&FORKS[r])<0){
                printf("error: sem_post did not work\n");
            }
        }
        if(sem_post(&Plate)<0){
            printf("error: sem_post did not work\n");
        }
    }while(1);
}

int main(){
    pthread_t Thread[5];
    for(int i=0;i<5;i++){
        P_Count[i]=i;
        MODE[i]='H';
        Eaten_Status[i]=0;
    }
    for(int i=0;i<2;i++){
        Souce_Bowl[i]='V';
        Bowl_Count[i]=-1;
    }
    if(sem_init(&Plate,0,4)<0){
        printf("error: wasn't able to initialise the main semaphore\n\n");
    }
    for(int i=0;i<5;i++){
        if(sem_init(&FORKS[i],0,0)<0){
            printf("error: wasn't able to initialise semaphore number %d\n",i+1);
        }
    }
    for(int i=0;i<5;i++){
        if(pthread_create(&Thread[i],NULL,PHILOSOPHER,&P_Count[i])<0){
            printf("error: wasn't able to create separate thread for philosopher number %d\n",i+1);
        }
    }
    for(int i=0;i<5;i++){
        if(pthread_join(Thread[i],NULL)<0){
            printf("error: wasn't able to join the thread for philosopher number %d back into the main thread\n",i+1);
        }
    }
    return 0;
}
