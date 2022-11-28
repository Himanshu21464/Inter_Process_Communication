#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<sys/un.h>
#define SIZE 100


int main() {
    int F_A, F_B;

    int RESULT;
    char BUFFER[SIZE];
	int MAXIMUM_INDEX = 0;
    // FIFO file path
    char * FIFO_File_Path_A = "FIFO1";
    char * FIFO_File_Path_B = "FIFO2";

    
    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    
    mkfifo(FIFO_File_Path_A, 0666);
    mkfifo(FIFO_File_Path_B, 0666);


	MAXIMUM_INDEX = 0;
	int CURRENT_INDEX = MAXIMUM_INDEX;
	while(1) {
		while(CURRENT_INDEX < MAXIMUM_INDEX + 5) {
			sleep(2);
			F_A = open(FIFO_File_Path_A, O_RDONLY);
			RESULT = read(F_A, BUFFER, sizeof(BUFFER));
			close(F_A);
			if(RESULT == -1) {	
				perror("cannot read the strings!!!");
				exit(EXIT_FAILURE);
			}
			CURRENT_INDEX = BUFFER[0];
			printf("String sent by P1FIFO         :    ");		
			
			for(int i = 1; i < 12; i++) {
				printf("%c", BUFFER[i]);
			}
			printf("\n");
			
			sprintf(BUFFER, "%d", CURRENT_INDEX);
			printf("String Index sent by P1FIFO   :    %s\n", BUFFER);	
		}
		//BUFFER contains the string with the 0th idx as the index of the string
		
		MAXIMUM_INDEX = CURRENT_INDEX;
		F_B = open(FIFO_File_Path_B, O_WRONLY);
		RESULT = write(F_B, BUFFER, sizeof(BUFFER));
		close(F_B);
		// break;
		if(MAXIMUM_INDEX >= 50){
			 exit(EXIT_SUCCESS);
		}
	}
    return 0;
}