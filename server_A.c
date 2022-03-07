/* 
Per compilare il codice, spostarsi nella directory dove sono presenti i file sorgenti.
Per creare un eseguibile di nome "server", dove "server" e` il nome dell'eseguibile creato mentre "server_C.c" e` il nome del file sorgente:
$ gcc -o server server_5C.C

Da riga di comando eseguo il server:
$ ./server

Apro un'altra finestra e da riga di comando instauro una connessione con telnet ed invio il messaggio:
$ telnet localhost 8080

Apro un'altra finestra e da riga di comando verifico l'effettiva creazione del socket e connessione:
$ netstat -tunape | grep 8080

*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>

#define N_CONN 12
#define PORT 8080


/* Prototipi delle funzioni/procedure */
int socket (int domain, int type, int protocol);
int bind (int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen (int sockfd, int backlog);
int accept (int sockfd, struct sockaddr *addr, socklen_t * addrlen);
int close(int fd);
ssize_t recv (int sockfd, void *buf, size_t len, int flags);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
/* */

int
main (int argc, char *argv[])
{
  int sockfd, new_sockfd, clilen, n1, n2;
  struct sockaddr_in server_address, client_address;
//  int port = 8080;
  char buffer[256];
  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    {
      printf ("Error during socket creation\n");
      exit (EXIT_FAILURE);
    }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons (PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;


  if (bind (sockfd, (struct sockaddr *) &server_address, sizeof (server_address)) < 0)
    {
      printf ("Error during bind....\n");
      exit (EXIT_FAILURE);
    }
  if (listen (sockfd, N_CONN) < 0)
    {
      printf ("Error during listen....\n");
      exit (EXIT_FAILURE);
    }
  clilen = sizeof (client_address);

  new_sockfd = accept (sockfd, (struct sockaddr *) &client_address, &clilen);
  if (new_sockfd < 0)
    {
      printf ("Error during accept....\n");
      exit (EXIT_FAILURE);
    }

// Succede qualcosa....

n1 = recv(new_sockfd, buffer, 255, 0);
  if (n1 < 0)
    {
      printf ("Error during recv.... sockfd: %d\n", new_sockfd);
      exit (EXIT_FAILURE);
    }
printf("The message is: %s\n", buffer);

n2 = send(new_sockfd, "I received your message\n",24, 0);
  if (n2 < 0)
    {
      printf ("Error during send.... sockfd: %d\n", new_sockfd);
      exit (EXIT_FAILURE);
    }

  if (close (sockfd) < 0)
    {
      printf ("Error during close....\n");
      exit (EXIT_FAILURE);
    }

  return 0;
}
