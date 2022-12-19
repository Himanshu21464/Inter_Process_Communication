#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<time.h>

#define SOCKET "socket1.socket"
#define BUFFER_SIZE 10

void Generate_Random_String(char Random_Strings[][12]) {        
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

int main(int Argument_Count, char* Argument[]) {
	struct sockaddr_un Socket_Address;
	int RESULT;
	int DATA_SOCKETS;
	char BUFF[BUFFER_SIZE];

	//Creating data socket
	DATA_SOCKETS = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if(DATA_SOCKETS == -1) {
		printf("--------------------------\n");
		perror("UNABLE to create socket!!!");
		printf("--------------------------\n");
		exit(EXIT_FAILURE);
	}	

	memset(&Socket_Address, 0, sizeof(Socket_Address));

	//For local connections
	Socket_Address.sun_family = AF_UNIX;
	strncpy(Socket_Address.sun_path, SOCKET, sizeof(Socket_Address.sun_path) - 1);
	RESULT = connect(DATA_SOCKETS, (const struct sockaddr *) &Socket_Address, sizeof(Socket_Address));

	if(RESULT == -1) {
		printf("----------------------------------\n");
		perror("Server is not working currently!!!\n");
		printf("----------------------------------\n");
		exit(EXIT_FAILURE);
	}	

	//Writing data
	char Random_String[51][12] = {{0}};
	Generate_Random_String(Random_String);
	
	for(int i = 1; i <= 50; i++) {
		for(int j = 0; j < 12; j++) {
			if(j == 0) {
				printf("%d ", Random_String[i][j]);
			}
			else printf("%c", Random_String[i][j]);
		}
		printf("\n");
	}

	if(Argument_Count > 1) {
		strncpy(BUFF, "DOWN", sizeof("DOWN"));
		write(DATA_SOCKETS, BUFF, sizeof(BUFF));
		close(DATA_SOCKETS);
		exit(EXIT_SUCCESS);
	}
	else {
		int LAST_INDEX = 1;
		while(1) {
			printf("Sending Strings Indexed from %d to %d\n", LAST_INDEX, LAST_INDEX + 4);
			for(int i = LAST_INDEX; i < LAST_INDEX + 5; i++) {
				RESULT = write(DATA_SOCKETS, Random_String[i], strlen(Random_String[i]) + 1);
				if(RESULT == -1) {
					perror("couldn't write");
				}
			}

			//Reading Acknowledgements
			RESULT = read(DATA_SOCKETS, BUFF, sizeof(BUFF));
			if(RESULT == -1) {
				perror("read");
				exit(EXIT_FAILURE);
			}

			BUFF[sizeof(BUFF) - 1] = 0;
			int FINAL_INDEX = atoi(BUFF);
			
			printf("MAX ID SENT BACK BY SERVER = %s\n\n", BUFF);
			if(FINAL_INDEX == 50) {
				printf("--------------------------------------------------\n");
				printf("| ALL STRINGS ARE SUCCESSSFULLY SENT BY PRODUCER |\n");
				printf("--------------------------------------------------\n");
				
				strncpy(BUFF, "DOWN", sizeof("DOWN"));
				write(DATA_SOCKETS, BUFF, sizeof(BUFF));
				close(DATA_SOCKETS);
				exit(EXIT_SUCCESS);
				break;
			}
			else {
				LAST_INDEX = FINAL_INDEX + 1;
			}
		}
	}
}