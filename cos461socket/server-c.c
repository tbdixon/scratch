#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include "port-utils-c.h"

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 * Return 0 on success, non-zero on failure
 */
int server(char *server_port) {
	int socketfd;
	int listeningsocketfd;
	struct addrinfo *serverInfo;
	struct sockaddr_storage clientInfo;
	socklen_t client_addr_size;
	char inboundData[RECV_BUFFER_SIZE];
	char s[INET6_ADDRSTRLEN];

	assert(atoi(server_port) >= 1024);

	serverInfo = get_addr_wrapper(server_port, "localhost");
	socketfd = get_socketfd(serverInfo);

	if(listen(socketfd, QUEUE_LENGTH) == -1) {
		perror("Error with listen");
	}

	while(1){
		memset(&inboundData, '\0', sizeof inboundData);
		/* Handle this gracefully if accept or recv fails */
		listeningsocketfd = accept(socketfd, (struct sockaddr_storage *)&clientInfo, (socklen_t *)&client_addr_size);
		inet_ntop(clientInfo.ss_family,
				get_in_addr((struct sockaddr *)&clientInfo),
				s, sizeof s);
		printf("server: got connection from %s\n", s);

		recv(listeningsocketfd, inboundData, RECV_BUFFER_SIZE, 0);
		printf("%s",inboundData);
		close(listeningsocketfd);
	}

	close(socketfd);
	freeaddrinfo(serverInfo);

	return 0;
}

/*
 * main():
 * Parse command-line arguments and call server function
 */
int main(int argc, char **argv) {
	char *server_port;

	if (argc != 2) {
		fprintf(stderr, "Usage: ./server-c [server port]\n");
		exit(EXIT_FAILURE);
	}

	server_port = argv[1];
	return server(server_port);
}
