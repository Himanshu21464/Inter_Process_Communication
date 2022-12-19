#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define NAME_OF_SOCKET "SOCKET_1.socket"
#define SIZE 12

int main(int Argument_Count, char *Argument[]) {
	struct sockaddr_un servername;
	int FLAG = 0;
	int CONNECTION_SOCKET;
	int RESULT;
	int MAX_INDEX = 0;
	int DATA_SOCKET;
	char BUFF[SIZE];

	CONNECTION_SOCKET = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if(CONNECTION_SOCKET == -1) {
		perror("Socket couldn't be made");
		exit(EXIT_FAILURE);
	}

	memset(&servername, 0, sizeof(servername));
	servername.sun_family = AF_UNIX;
	strncpy(servername.sun_path, NAME_OF_SOCKET, sizeof(servername.sun_path) - 1);

	RESULT = bind(CONNECTION_SOCKET, (const struct sockaddr *) &servername, sizeof(servername));

	if(RESULT == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	RESULT = listen(CONNECTION_SOCKET, 100);
	if(RESULT == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	while(1) {
		DATA_SOCKET = accept(CONNECTION_SOCKET, NULL, NULL);
		if(DATA_SOCKET == -1) {
			perror("couldn't accept");
			exit(EXIT_FAILURE);
		}

		MAX_INDEX = 0;
		int CURRENT_INDEX = MAX_INDEX;
		while(1) {
			RESULT = read(DATA_SOCKET, BUFF, sizeof(BUFF));
			if(RESULT == -1) {
				perror("couldn't read");
				exit(EXIT_FAILURE);
			}

			BUFF[sizeof(BUFF) - 1] = 0;
			CURRENT_INDEX = BUFF[0];

			if(!strncmp(BUFF, "DOWN", sizeof(BUFF))) {
				FLAG = 1;
				break;
			}

			//BUFF contains the string with the 0th idx as the index of the string
			printf("STRING SENT BY CLIENT: ");		
			
			for(int i = 1; i < 11; i++) {
				printf("%c", BUFF[i]);
			}

			printf("\n");
			sprintf(BUFF, "%d", CURRENT_INDEX);
			printf("ID SENT BY CLIENT = %s\n", BUFF);		

			if(CURRENT_INDEX == MAX_INDEX + 5) {
				MAX_INDEX = CURRENT_INDEX;
				RESULT = write(DATA_SOCKET, BUFF, sizeof(BUFF));
			}	

			if(CURRENT_INDEX >= 50) break;
		}
		
		

		close(DATA_SOCKET);

		if(FLAG) {
			printf("SHUTTING SERVER");
			close(CONNECTION_SOCKET);
			unlink(NAME_OF_SOCKET);
			exit(EXIT_SUCCESS);
			break;
		}
		
		if(RESULT == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}