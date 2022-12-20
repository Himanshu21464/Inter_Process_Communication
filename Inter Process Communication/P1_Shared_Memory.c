
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
void Get_Random_String(char Random_Strings[][12]) {        
    srand (time(NULL));                            
    for (int j = 0; j <= 50; j++) {
        Random_Strings[j][0] = j;
    }
    
    for(int i = 0; i <= 50; i++) {
		for(int j = 1; j <= 10; j++) {
			Random_Strings[i][j] = rand() % 26 + 65;
		}
	}
}

int main(){
	char astr[51][12]={{0}};
	Get_Random_String(astr);
	key_t key = ftok("shmfile",65);
	int shmid = shmget(key,1024,0666|IPC_CREAT);
	char *Concat = (char*) shmat(shmid,(void*)0,0);

	int First_index=1;
	int Last_Index=5;	
	int Quit_Signal=0;



while (Quit_Signal!=1){

	for(int i = First_index; i <= Last_Index; i++) {
		for(int j = 0; j < 12; j++) {
			if(j == 0) {
				printf("%d ", astr[i][j]);
			}
			else printf("%c", astr[i][j]);
		}
		printf("\n");
		strcat(Concat,astr[i]);
	}
	//First_index+=Last_Index;
	//Last_Index+=6;

	// printf("COncatenated string: %s\n",Concat);
	// Concat=" ";
	 //if(Last_Index==51){
		Quit_Signal=1;
	//}
	}
	
	//detach from shared memory
	shmdt(Concat);

	return 0;
}

