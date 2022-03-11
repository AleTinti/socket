/* Librerie */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

/* Costanti */
#define PORTA 1313

struct sockaddr_in client;
int socketlen; 

/* Funzione, esterna al main, che riceve un messaggio dal server */
int ricevi_msg (int socketfd, char *buffer){
  int numCaratteri;
  socketlen = sizeof (client);
  if ((numCaratteri = recvfrom (socketfd, buffer, BUFSIZ, 0,
                       (struct sockaddr *) &client,(int *) &socketlen)) < 0){
    printf ("Errore nella ricezione del messaggio");
    exit(EXIT_FAILURE);
  }
  buffer [numCaratteri] = '\0';
  return numCaratteri;
}

/* Funzione che trasmette un messaggio al server */
int invia_msg (int socketfd, char *buffer, char *ip_address, int porta){
  int numCaratteri = strlen(buffer);
  /* Configurazione parametri porta locale */
  bzero ((char *) &client, sizeof (client));
  client.sin_family = AF_INET;
  client.sin_addr.s_addr = inet_addr (ip_address);
  client.sin_port = htons ((unsigned short) porta);

  if (sendto (socketfd, buffer, numCaratteri, 0,(struct sockaddr *) &client, 
                                                sizeof(client)) != numCaratteri){
    printf ("Errore nella trasmissione del messaggio");
    exit(EXIT_FAILURE);
  }
  return 1;
}

void  error(char *msg){
  perror(msg);
  exit(EXIT_FAILURE);
}

int main (int argc, char *argv[]){
   // variabili locali  
   char buffer[BUFSIZ + 1];
   int socketfd;

   if (argc != 2){                     // IP del server come parametro 
      fprintf (stderr, "inserire indirizzo IP del server\n");
      exit (EXIT_FAILURE);
   }

   /* Creazione della socket del client */
   if ((socketfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0){
      printf ("Errore durante la creazione del socket!");
      exit(EXIT_FAILURE);
   } 
   
   printf ("Inserire nome e offerta: "); 
   if (fgets (buffer, BUFSIZ, stdin) != NULL){
     invia_msg (socketfd, buffer, argv[1], PORTA);     // Invio del messaggio 
     if (ricevi_msg (socketfd, buffer) > 0)           // Attesa della risposta 
       fputs (buffer, stdout);
   }

   /* Chiusura socket del client */
   if (close (socketfd) != 0){
     printf ("Errore durante la chiusura del socket!");
     exit(EXIT_FAILURE);
   }

   return 0;
}
