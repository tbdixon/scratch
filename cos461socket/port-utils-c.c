#include <stdio.h>
#include <string.h>
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

/* Wrapper to manage the setup for getaddrinfo call */
struct addrinfo *get_addr_wrapper(char *portNum, char *serverIP){
	struct addrinfo hints;
	struct addrinfo *p, *res;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(serverIP, portNum, &hints, &res) != 0){
		perror("Error getting address info");
		exit(1);
	}
	/* Use the first valid IPv4 entry */
	for(p = res; p != NULL; p = p->ai_next){
		/* IPv4 */
		if (p->ai_family == AF_INET) {
			res = p;
			break;
		}
	}
	/* We must have run through the entire list without breaking so nothing valid was found*/
	if(p == NULL) {
		printf("No valid IPv4 address found");
		exit(1);
	}

	return p;
}

/* Get *and bind* a socket and return the file descriptor */
int get_socketfd(struct addrinfo *res){
	int socketfd;

	socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(socketfd == -1){
		perror("Issue calling socket");
		exit(1);
	}

	if(bind(socketfd, res->ai_addr, res->ai_addrlen) == -1){
		perror("Issue binding socket");
		exit(1);
	}
	return socketfd;
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
