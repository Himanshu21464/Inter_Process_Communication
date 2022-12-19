#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<time.h>

#define NAME_OF_SOCKET "Himanshu_2021464.socket"
#define SIZE 10

void GENERATE_RANDOM_STRING(char RANDOM_STRINGS[][12]) {        
    srand (time(NULL));                            
    for (int j = 0; j <= 50; j++) {
        RANDOM_STRINGS[j][0] = j;
    }
    
    for(int i = 0; i <= 50; i++) {
		for(int j = 1; j <= 10; j++) {
			RANDOM_STRINGS[i][j] = rand() % 26 + 65;
		}
	}
}

int main(int Argument_Count, char* Argument[]) {
	struct sockaddr_un ADDRESS;
	int RESULT;
	int DATA_SOCKET;
	char BUFF[SIZE];

	//Creating data socket
	DATA_SOCKET = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if(DATA_SOCKET == -1) {
		printf("--------------------------\n");
		perror("UNABLE To Create Socket!!!\n");
		printf("--------------------------\n");
		exit(EXIT_FAILURE);
	}	

	memset(&ADDRESS, 0, sizeof(ADDRESS));

	//For local connections
	ADDRESS.sun_family = AF_UNIX;
	strncpy(ADDRESS.sun_path, NAME_OF_SOCKET, sizeof(ADDRESS.sun_path) - 1);
	RESULT = connect(DATA_SOCKET, (const struct sockaddr *) &ADDRESS, sizeof(ADDRESS));

	if(RESULT == -1) {
		printf("---------------\n");
		perror("| SERVER DOWN |\n");
		printf("---------------\n");
		exit(EXIT_FAILURE);
	}	

	//Writing data
	char RANDOM_STRING[51][12] = {{0}};
	GENERATE_RANDOM_STRING(RANDOM_STRING);
	
	for(int i = 1; i <= 50; i++) {
		for(int j = 0; j < 12; j++) {
			if(j == 0) {
				printf("%d ", RANDOM_STRING[i][j]);
			}
			else printf("%c", RANDOM_STRING[i][j]);
		}
		printf("\n");
	}

	if(Argument_Count > 1) {
		strncpy(BUFF, "DOWN", sizeof("DOWN"));
		write(DATA_SOCKET, BUFF, sizeof(BUFF));
		close(DATA_SOCKET);
		exit(EXIT_SUCCESS);
	}
	else {
		int LAST_INDEX = 1;
		while(1) {
			printf("Sending Strings Indexed from %d to %d\n", LAST_INDEX, LAST_INDEX + 4);
			for(int i = LAST_INDEX; i < LAST_INDEX + 5; i++) {
				RESULT = write(DATA_SOCKET, RANDOM_STRING[i], strlen(RANDOM_STRING[i]) + 1);
				if(RESULT == -1) {
					perror("UNABLE TO WRITE!!!");
				}
			}

			//Reading Acknowledgements
			RESULT = read(DATA_SOCKET, BUFF, sizeof(BUFF));
			if(RESULT == -1) {
				perror("read");
				exit(EXIT_FAILURE);
			}

			BUFF[sizeof(BUFF) - 1] = 0;
			int FINAL_INDEX = atoi(BUFF);
			printf("--------------------------------------\n");
			printf("MAX ID RECIEVED BY SERVER = %s\n\n", BUFF);
			printf("--------------------------------------\n");
			if(FINAL_INDEX == 50) {
				printf("--------------------------------------------\n");
				printf("ALL STRINGS SENT TO CONSUMER SUCCESSFULLY!!!\n");
				printf("--------------------------------------------\n");
				strncpy(BUFF, "DOWN", sizeof("DOWN"));
				write(DATA_SOCKET, BUFF, sizeof(BUFF));
				close(DATA_SOCKET);
				exit(EXIT_SUCCESS);
				break;
			}
			else {
				LAST_INDEX = FINAL_INDEX + 1;
			}
		}
	}
}