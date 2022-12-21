
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<unistd.h>

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
	

	int a=1;
	int b=12;
	for(int x=1;x<=50;x++){
		strcat(Concat,astr[x]);
	}
	sleep(5);
	long Start=clock();
	for(int x=1;x<=50;x++){
		for(int z=a;z<b;z++){
			printf("%c",Concat[z]);
		}
		printf("\n");

		if(x%5==0){
			printf("MAX ID recieved from P2 : %d\n",x);
		}
		
		a=b;
	    b=b+11;	
	}
	printf("All STRINGS send successfully!!\n");
	long end=clock();
	long final=end-Start;
	printf("Time : %ld",final);
	//detach from shared memory
	shmdt(Concat);

	return 0;
}

