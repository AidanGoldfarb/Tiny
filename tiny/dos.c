#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv){
	if(argc != 2){
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	int port = atoi(argv[1]);
	int socket_id;
	struct sockaddr_in server;
	ssize_t payload_id;
	char payload[900];

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

	strcpy(payload, "GET\n");
	payload_id = send(socket_id,payload,strlen(payload),0);
	if(payload_id < 0){
		perror("Failed to send payload\n");
		exit(0);
	}
}