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

int main() {
    int F_A, F_B;

    int RESULT;
    char BUFFER[SIZE];
    char * FIFO_File_Path_A = "File_For_FIFO_1";
    char * FIFO_File_Path_B = "File_For_FIFO_2";
    
    char Random_String_Generated[51][12] = {{0}};

    Get_Random_String(Random_String_Generated);
    
	for(int i = 1; i <= 50; i++) {
		for(int j = 0; j < 12; j++) {
			if(j == 0) {
				printf("%d ", Random_String_Generated[i][j]);
			}
			else printf("%c", Random_String_Generated[i][j]);
		}
		printf("\n");
	}

	printf("\n%s\n", "The above strings are random strings created");
	
    mkfifo(FIFO_File_Path_A, 0666);
    mkfifo(FIFO_File_Path_B, 0666);


	int END_INDEX = 1;
	while(1) {
		if(F_A == -1) {
			perror("Couldn't access FIFO1");
		}

		printf("sending Strings Indexed from %d to %d\n", END_INDEX, END_INDEX + 4);
		
		for(int i = END_INDEX; i < END_INDEX + 5; i++) {
			F_A = open(FIFO_File_Path_A, O_WRONLY);
			RESULT = write(F_A, Random_String_Generated[i], sizeof(Random_String_Generated[i]) + 1);
			sleep(1);	
			close(F_A);
		}

		F_B = open(FIFO_File_Path_B, O_RDONLY);
		RESULT = read(F_B, BUFFER, sizeof(BUFFER));
		close(F_B);

		if(RESULT == -1) {
			perror("read");
			exit(EXIT_FAILURE);
		}

		int FINAL_INDEX = atoi(BUFFER);
 		printf("MAX ID SENT BACK BY SERVER = %s\n\n", BUFFER);
		END_INDEX = FINAL_INDEX + 1;
		if(END_INDEX >= 50){
			exit(EXIT_SUCCESS);
		}
	}

    return 0;
}