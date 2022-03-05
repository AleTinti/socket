#include<stdio.h>
#include<stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<netinet/in.h>
#include <unistd.h>

#define N_CONN 12

/* Prototipi delle funzioni/procedue */

int
main ()
{
  int sockfd, new_sockfd, clilen, n1, n2;
  struct sockaddr_in server_address, client_address;
  int port = 8080;
  char buffer[256];
  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    {
      printf ("Error during socket creation\n");
      exit (EXIT_FAILURE);
    }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons (port);
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
      printf ("Error during recv.... sockfd: %d\n", new_sockfd);
      exit (EXIT_FAILURE);
    }

  if (close (sockfd) < 0)
    {
      printf ("Error during close....\n");
      exit (EXIT_FAILURE);
    }


  return 0;
}
