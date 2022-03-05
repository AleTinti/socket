#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#define PORT_NO 52000

/* Prototipi delle funzioni/procedure */
int socket(int domain, int type, int protocol);
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int main (){

int sockfd;
struct sockaddr_in server_addr;

sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0){
 printf("Error during socket creation...\n");
 exit(EXIT_FAILURE);
}

server_addr.sin_family = AF_INET;
server_addr.sinport = hton(PORT_NO);
server_addr.sin_addr.s_addr = INADDR_ANY;

if(bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
 printf("Error during bind...\n");
 exit(EXIT_FAILURE);
}
 

return 0;
}










ZZZZZZ


