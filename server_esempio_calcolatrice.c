#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>

#define N_CON 5

void
error (char *msg)
{
  perror (msg);
  exit (1);
}

void
main (int argc, char *argv[])
{
  int b, sockfd, connfd, sin_size, l, n, len;
  char operator;
  int op1 = 0, op2 = 0, result = 0;
  int portno;

  if (argc < 2)
    {
      fprintf (stderr, "ERROR, check and add the port number\n");
      exit (1);
    }

  portno = atoi (argv[1]);

  /* socket() */
  if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    error ("ERROR opening socket\n");
  printf ("%d\n", sockfd);

  struct sockaddr_in servaddr;
  struct sockaddr_in clientaddr;

  servaddr.sin_family = AF_INET;
  // servaddr.sin_addr.s_addr = inet_addr ("127.0.0.1");
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons (portno);

  /* bind() */
  if ((bind (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr))) < 0)
    error ("ERROR on binding");

  /* listen() */
  listen (sockfd, N_CON);

  sin_size = sizeof (struct sockaddr_in);
  /* accept() */
  connfd = accept (sockfd, (struct sockaddr *) &clientaddr, &sin_size);
  if (connfd < 0)
    error ("ERROR on accept");

  int read1, read2, read3;

  //read1 =  read (connfd, &operator, 10);
  read1 = recv (connfd, &operator, 10, 0);
  // printf ("read1: %d\n", read1);
  read2 = recv (connfd, &op1, sizeof (op1), 0);
  // read2 = read (connfd, &op1, sizeof (op1));
  // printf ("read2: %d\n", read2);
  read3 = recv (connfd, &op2, sizeof (op2), 0);
  // read3 =  read (connfd, &op2, sizeof (op2));
  // printf ("read3: %d\n", read3);
  printf ("op1: %d, op2: %d\n", op1, op2);

  switch (operator)
    {
    case '+':
      result = op1 + op2;
      printf ("Result sum is: %d + %d = %d\n", op1, op2, result);
      break;
    case '-':
      result = op1 - op2;
      printf ("Result differnce is: %d - %d = %d\n", op1, op2, result);
      break;
    case '*':
      result = op1 * op2;
      printf ("Result multiplication is: %d * %d = %d\n", op1, op2, result);
      break;
    case '/':
      result = op1 / op2;
      printf ("Result division is: %d / %d = %d\n", op1, op2, result);
      break;
    default:
      printf ("ERROR: Unsupported Operation");
    }

  send (connfd, &result, sizeof (result), 0);
  // write (connfd, &result, sizeof (result));

  /* close() */
  close (sockfd);
}
