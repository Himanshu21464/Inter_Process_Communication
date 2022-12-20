
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main(){
	// ftok to generate unique key
	key_t key = ftok("shmfile",65);

	// shmget returns an identifier in shmid
	int shmid = shmget(key,1024,0666|IPC_CREAT);

	// shmat to attach to shared memory
	char *str = (char*) shmat(shmid,(void*)0,0);

	printf("Data read from memory: %s\n",str);
	int a=1;
	int b=12;
	for(int x=1;x<=5;x++){
		for(int z=a;z<b;z++){
			printf("%c",str[z]);
		}
		printf("\n");
		a=b;
	    b=b+11;	
	}
	

	
	
	//detach from shared memory
	shmdt(str);
	
	// destroy the shared memory
	shmctl(shmid,IPC_RMID,NULL);
	
	return 0;
}

