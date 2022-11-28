#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<time.h>

#define Name_Of_Socket "2021464.socket"
#define SIZE 10

void Random_String_Generator(char Random_Strings_Array[][12]) {        
    srand (time(NULL));                            
    for (int y = 0; y <= 50; y++) {
        Random_Strings_Array[y][0] = y;
    }
    
    for(int x = 0; x <= 50; x++) {
		for(int y = 1; y <= 10; y++) {
			Random_Strings_Array[x][y] = rand() % 26 + 65;
		}
	}
}

int main(int argc, char* argv[]) {
	struct sockaddr_un Address;
	int RESULT;
	int Data_Socket;
	char BUFFER[SIZE];

	/////////////////////////////////////////////////////////////////////////////////////////
	Data_Socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if(Data_Socket == -1) {
		printf("Unable To Create Socket!!!!!");
		exit(-1);
	}	
	memset(&Address, 0, sizeof(Address));

	/////////////////////////////////////////////////////////////////////////////////////////
	Address.sun_family = AF_UNIX;
	strncpy(Address.sun_path, Name_Of_Socket, sizeof(Address.sun_path) - 1);
	RESULT = connect(Data_Socket, (const struct sockaddr *) &Address, sizeof(Address));

	if(RESULT == -1) {
		printf("Server is down\n");
		exit(-1);
	}	

	/////////////////////////////////////////////////////////////////////////////////////////
	char Random_String[51][12] = {{0}};
	Random_String_Generator(Random_String);
	
	for(int x = 1; x <= 50; x++) {
		for(int y = 0; y < 12; y++) {
			if(y == 0) {
				printf("%d ", Random_String[x][y]);
			}
			else printf("%c", Random_String[x][y]);
		}
		printf("\n");
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	if(argc > 1) {
		strncpy(BUFFER, "DOWN", sizeof("DOWN"));
		write(Data_Socket, BUFFER, sizeof(BUFFER));
		close(Data_Socket);
		exit(-1);
	}
	else {
		int END_INDEX = 1;
		while(1) {
			printf("sending Strings Indexed from %d to %d\n", END_INDEX, END_INDEX + 4);
			for(int x = END_INDEX; x < END_INDEX + 5; x++) {
				RESULT = write(Data_Socket, Random_String[x], strlen(Random_String[x]) + 1);
				if(RESULT == -1) {
					perror("couldn't write");
				}
			}

			////////////////////////////////////////////////////////////////////////////////
			RESULT = read(Data_Socket, BUFFER, sizeof(BUFFER));
			if(RESULT == -1) {
				printf("Read");
				exit(-1);
			}

			BUFFER[sizeof(BUFFER) - 1] = 0;
			int FINAL_INDEX = atoi(BUFFER);
			
			printf("MAX ID SENT BACK BY SERVER = %s\n\n", BUFFER);
			if(FINAL_INDEX == 50) {
				printf("===============================================\n");
				printf("|    All the strings were sent Successfully   |\n");
				printf("===============================================\n");
				
				strncpy(BUFFER, "DOWN", sizeof("DOWN"));
				write(Data_Socket, BUFFER, sizeof(BUFFER));
				close(Data_Socket);
				exit(0);
				break;
			}
			else {
				END_INDEX = FINAL_INDEX + 1;
			}
		}
	}
}