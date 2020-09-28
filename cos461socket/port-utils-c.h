#ifndef PORTTOOLC_H
#define PORTTOOLC_H

/* Wrapper to manage the setup for getaddrinfo call */
struct addrinfo *get_addr_wrapper(char *portNum, char *serverIP);

/* Get *and bind* a socket and return the file descriptor */
int get_socketfd(struct addrinfo *addr_info);


/* Helper for printing info from Beej's guide*/
void *get_in_addr(struct sockaddr *sa);

#endif
