#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "port-utils-c.h"

#define SEND_BUFFER_SIZE 2048


/* TODO: client()
 * Open socket and send message from stdin.
 * Return 0 on success, non-zero on failure
 */
int client(char *server_ip, char *server_port) {
	int socketfd, inputLen;
	struct addrinfo *serverInfo;
	char s[INET6_ADDRSTRLEN];
	char input[SEND_BUFFER_SIZE];

	serverInfo = get_addr_wrapper(server_port, server_ip);
	socketfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if(socketfd == -1){
		perror("Issue calling socket");
		exit(1);
	}
	if(connect(socketfd, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1 ){
		perror("Error connecting to server");
		close(socketfd);
		exit(1);
	}

	inet_ntop(serverInfo->ai_family, get_in_addr((struct sockaddr *)serverInfo->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);
	freeaddrinfo(serverInfo); 
	fgets(input, sizeof input, stdin);
	inputLen = strlen(input);
	printf("sending string of length %d: %s", inputLen, input);

	send(socketfd, input, inputLen, 0);
	return 0;
}

/*
 * main()
 * Parse command-line arguments and call client function
 */
int main(int argc, char **argv) {
	char *server_ip;
	char *server_port;

	if (argc != 3) {
		fprintf(stderr, "Usage: ./client-c [server IP] [server port] < [message]\n");
		exit(EXIT_FAILURE);
	}

	server_ip = argv[1];
	server_port = argv[2];
	return client(server_ip, server_port);
}
