#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include <unistd.h>
#include<string.h>
#include<strings.h>
#include <arpa/inet.h>

void
error (char *msg)
{
  perror (msg);
  exit (0);
}

void
main (int argc, char *argv[])
{
  int b, sockfd, sin_size, con, n, len;
  char operator;
  int op1, op2, result;
  int portno;


  /* Only port number passed as argument -- es. $ ./my_file portno 
     Otherwise, if you want to pass also the server name use "gethostbyname"
  */
  if (argc < 2)
    {
      fprintf (stderr, "missing hostname port\n");
      exit (0);
    }
  portno = atoi (argv[1]);

  /* socket() */
  if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    error ("ERROR opening socket\n");
  printf ("sockfd: %d\n", sockfd);

  struct sockaddr_in servaddr;

  servaddr.sin_family = AF_INET;
  //servaddr.sin_addr.s_addr = inet_addr ("127.0.0.1");
  servaddr.sin_addr.s_addr = inet_addr ("4.4.2.0");
  servaddr.sin_port = htons (portno);

  sin_size = sizeof (struct sockaddr_in);
  /* connect() */
  if ((connect (sockfd, (struct sockaddr *) &servaddr, sin_size)) < 0)
    error ("ERROR connection\n");

  printf ("Enter the operation:\n +\n-:\n/:\n*:\n");
  scanf ("%c", &operator);
  printf ("Enter first operand:\n");
  scanf ("%d", &op1);
  printf ("Enter second operand:\n");
  scanf ("%d", &op2);

  printf ("op1: %d, op2: %d\n", op1, op2);

  send (sockfd, &operator, 10, 0);
  // write (sockfd, &operator, 10);
  send (sockfd, &op1, sizeof (op1), 0);
  // write (sockfd, &op1, sizeof (op1));
  send (sockfd, &op2, sizeof (op2), 0);
  // write (sockfd, &op2, sizeof (op2));

  recv (sockfd, &result, sizeof (result), 0);
  // read (sockfd, &result, sizeof (result));
  printf ("Result from server= %d\n", result);

  /* close() */
  close (sockfd);
}
