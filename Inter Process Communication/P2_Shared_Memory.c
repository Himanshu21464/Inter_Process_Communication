
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main(){
	key_t key = ftok("shmfile",65);
	int shmid = shmget(key,1024,0666|IPC_CREAT);
	char *str = (char*) shmat(shmid,(void*)0,0);

	//printf("Data read from memory: %s\n",str);
	int a=1;
	int b=12;
	for(int x=1;x<=50;x++){
		for(int z=a;z<b;z++){
			printf("%c",str[z]);
		}
		printf("\n");
		if(x%5==0){
			printf("MAX ID sent to P1 : %d\n",x);
		}
		
		a=b;
	    b=b+11;	
	}
	shmdt(str);
	shmctl(shmid,IPC_RMID,NULL);
	
	return 0;
}