#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define NAME_OF_SOCKET "Himanshu_2021464.socket"
#define SIZE 12

int main(int Argument_Count, char *Argument[]) {
	struct sockaddr_un NAME_OF_SERVER;
	int FLAG = 0;
	int CONNECTION_SOCKET;
	int RESULT;
	int MAX_INDEX = 0;
	int DATA_SOCKET;
	char BUFF[SIZE];

	CONNECTION_SOCKET = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if(CONNECTION_SOCKET == -1) {
		printf("------------------------------------------\n");
		perror("UNABLE TO CREATE CONNECTION WITH SOCKET!!!");
		printf("------------------------------------------\n");
		exit(EXIT_FAILURE);
	}

	memset(&NAME_OF_SERVER, 0, sizeof(NAME_OF_SERVER));
	NAME_OF_SERVER.sun_family = AF_UNIX;
	strncpy(NAME_OF_SERVER.sun_path, NAME_OF_SOCKET, sizeof(NAME_OF_SERVER.sun_path) - 1);

	RESULT = bind(CONNECTION_SOCKET, (const struct sockaddr *) &NAME_OF_SERVER, sizeof(NAME_OF_SERVER));

	if(RESULT == -1) {
		perror("----BIND----");
		exit(EXIT_FAILURE);
	}

	RESULT = listen(CONNECTION_SOCKET, 100);
	if(RESULT == -1) {
		perror("---LISTEN----");
		exit(EXIT_FAILURE);
	}

	while(1) {
		DATA_SOCKET = accept(CONNECTION_SOCKET, NULL, NULL);
		if(DATA_SOCKET == -1) {
			printf("-------------------\n");
			perror("UNABLE TO ACCEPT!!!\n");
			printf("-------------------\n");
			exit(EXIT_FAILURE);
		}

		MAX_INDEX = 0;
		int CURRENT_INDEX = MAX_INDEX;
		while(1) {
			RESULT = read(DATA_SOCKET, BUFF, sizeof(BUFF));
			if(RESULT == -1) {
				printf("-----------------\n");
				perror("UNABLE TO read!!!\n");
				printf("-----------------\n");
				exit(EXIT_FAILURE);
			}

			BUFF[sizeof(BUFF) - 1] = 0;
			CURRENT_INDEX = BUFF[0];

			if(!strncmp(BUFF, "DOWN", sizeof(BUFF))) {
				FLAG = 1;
				break;
			}

			//BUFF contains the string with the 0th idx as the index of the string
			printf("STRING SENT BY CLIENT     :  ");		
			
			for(int i = 1; i < 11; i++) {
				printf("%c", BUFF[i]);
			}

			printf("\n");
			sprintf(BUFF, "%d", CURRENT_INDEX);
			printf("ID SENT BY CLIENT         :  %s\n", BUFF);		

			if(CURRENT_INDEX == MAX_INDEX + 5) {
				MAX_INDEX = CURRENT_INDEX;
				RESULT = write(DATA_SOCKET, BUFF, sizeof(BUFF));
			}	

			if(CURRENT_INDEX >= 50) {
				exit(EXIT_SUCCESS);
				break;
		
			}
		}
		
		

		close(DATA_SOCKET);

		if(FLAG) {
			printf("-------SHUTTING SERVER-------");
			close(CONNECTION_SOCKET);
			unlink(NAME_OF_SOCKET);
			exit(EXIT_SUCCESS);
			break;
		}
		
		if(RESULT == -1) {
			perror("---WRITE---");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}