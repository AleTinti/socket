/* 
gcc -o server server_5C.c -- Per compilare il codice, crea un eseguibile di nome "server"
./server -- Eseguo il server
telnet localhost 52000 -- Instauro una connessione con telnet ed invio il messaggio
netstat -tunape | grep 52000 -- Guardo l'effettiva creazione del socket e connessione
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT_NO 52000

/* Prototipi delle funzioni/procedure */
int socket (int domain, int type, int protocol);
int bind (int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen (int sockfd, int backlog);
int accept (int sockfd, struct sockaddr *addr, socklen_t * addrlen);
int close(fd);
ssize_t recv (int sockfd, void *buf, size_t len, int flags);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
/* */

//struct studente{
//char nome[256];
//int eta;
//int ID;
//}

int
main ()
{
//struct studente stud1, stud2;
//stud1.eta = 20;
//stud2.eta = 20;

  int sockfd, backlog = 6, new_sockfd, client_l;
  char pippo[256];
  ssize_t mess_recv, mess_send;
  struct sockaddr_in server_addr, client_addr;

  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    {
      printf ("Error during socket creation...\n");
      exit (EXIT_FAILURE);
    }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons (PORT_NO);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind (sockfd, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0)
    {
      printf ("Error during bind...\n");
      exit (EXIT_FAILURE);
    }

  if (listen (sockfd, backlog) < 0)
    {
      printf ("Error during listen...\n");
      exit (EXIT_FAILURE);
    }
//int stud_l;
//stud_l = sizeof(stud2);

  client_l = sizeof (client_addr);
  new_sockfd = accept (sockfd, (struct sockaddr *) &client_addr, &client_l);
  if (new_sockfd < 0)
    {
      printf ("Error during accept...\n");
      exit (EXIT_FAILURE);
    }

  mess_recv = recv (new_sockfd, pippo, 255, 0);
  if (mess_recv < 0)
    {
      printf ("Error during recv...\n");
      exit (EXIT_FAILURE);
    }
  printf ("The message is: %s\n", pippo);

  mess_send = send (new_sockfd, "Message received!", 18, 0);
  if (mess_send < 0)
    {
      printf ("Error during recv...\n");
      exit (EXIT_FAILURE);
    }

  if (close (sockfd) < 0)
    {
      printf ("Error during close...\n");
      exit (EXIT_FAILURE);
    }

  return 0;
}
