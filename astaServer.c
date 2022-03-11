/*
Esempio di socket per gestire un'asta. Famiglia AF_INET, protocollo di trasporto: UDP.

Per testare il codice:
1) compialare i file sorgenti: 
- $ gcc -o server astaServer.c 
- $ gcc -o client astaClient.c

2) da shell lanciare il server:
$ ./server

3) da shell lanciare il client:
$ ./client 127.0.0.1 (o $ ./client localhost) 

4) Lato client inserire nome e offerta
(4bis) Testare il server con socat...) 

5) Aggiornare il server in maniera tale che resti attivo per ricevere piu` offerte e sia interattivo... ongoing!

*/

/* Librerie usate */
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

/* Costanti usate */
#define MAXOFFERTE   9		// Numero massimo di offerte per sessione
#define MAXLUNGO    40		// Lunghezza nominativo offerente
#define FILENAME "asta.txt"	// File che memorizza i dati del miglior offerente
#define IPADDRESS "127.0.0.1"	// Indirizzo del server
#define PORTA 1313		// Numero di porta del server
struct sockaddr_in offerente;
int socketlen;

  /* Creazione socket sul server, esterna dal main -- include sia la 'socket()' che la 'bind()' */
int
creaServer (char *ip_address, int porta)
{
  int socketfd;
  struct sockaddr_in server;

  /* Creo il descrittore del socket */
  if ((socketfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      printf ("Errore nella creazione della socket");
      return 0;
    }

  /* Configuro i parametri */
  bzero ((char *) &server, sizeof (server));        /* Azzera il record        */
  server.sin_family = AF_INET;	                    /* Internet                */
  server.sin_addr.s_addr = inet_addr (ip_address);  /* Indirizzo IP del server */
  server.sin_port = htons (porta);                  /* Porta del server        */

  /* Collegamento dela socket sul server   */
  if (bind (socketfd, (struct sockaddr *) &server, sizeof (server)) < 0)
    {
      printf ("Errore nell'operazione di bind");
      return 0;
    }
  return socketfd;        /* Ritorna il descrittore del socket: sockfd*/
}


/*  Funzione, esterna al main, che trasmette un messaggio al client con la 'sendto() */
int
invia_msg (int socketfd, char *buffer)
{
  int msg_len = strlen (buffer);
  if (sendto (socketfd, buffer, msg_len, 0, (struct sockaddr *) &offerente, socketlen) < 0)
    {
      printf ("Errore nella trasmissione del messaggio di risposta");
      return 0;
    }
  exit (EXIT_SUCCESS);
}


/* Funzione, esterna al main, che riceve un messaggio dal client */
int
ricevi_msg (int socketfd, char *buffer)
{
  int numCaratteri;

  /* attesa della ricezione di un datagramma */
  socketlen = sizeof (offerente);
  if ((numCaratteri = recvfrom (socketfd, buffer, BUFSIZ, 0, (struct sockaddr *) &offerente, (int *) &socketlen)) < 0)
    {
      printf ("Errore nella ricezione del messaggio con l'offerta");
      exit (EXIT_FAILURE);
    }
  buffer[numCaratteri] = '\0';
  return numCaratteri;
}

int
main (int argc, char *argv[])
{
  int socketfd, n_offerte;	
  char buffer[BUFSIZ + 1];
  char nomeOfferente[MAXLUNGO], nome_vincente[MAXLUNGO];
  int nuova_offerta, offerta_vincente;
  FILE *fileOfferte;
  n_offerte = 0;

  /* Creazione del socket, richiamando la funzione esterna */
  if ((socketfd = creaServer (IPADDRESS, PORTA)) == 0)
    {
      fprintf (stderr, "impossibile creare la socket sul server\n");
      exit (EXIT_FAILURE);
    }

  if ((fileOfferte = fopen (FILENAME, "r")) == NULL)
    {
      nome_vincente[0] = '\0';
      offerta_vincente = 0;
      printf ("L'asta e' aperta: nessuna offerta pervenuta\n");
    }

  else
    {
      fscanf (fileOfferte, "%s %d", nome_vincente, &offerta_vincente);
      fclose (fileOfferte);
      printf ("L'asta e' aperta: fate le vostre offerte\n");
      printf ("Migliore offerta pervenuta: %s %d\n", nome_vincente, offerta_vincente);
    }

  /* Ricezione di un'offerta */ 
  while (ricevi_msg (socketfd, buffer) > 0)
    {
      nuova_offerta = 0;
      sscanf (buffer, "%s %d", nomeOfferente, &nuova_offerta);
      if (nuova_offerta <= 0)
	invia_msg (socketfd, "Offerta non valida\n");

      else
	{
	  n_offerte++;
	  printf ("Nuova offerta # %d di Euro  %d\n", n_offerte, nuova_offerta);
	  if (nuova_offerta > offerta_vincente)
	    {			/* La nuova offerta ricevuta diventa vincente */
	      offerta_vincente = nuova_offerta;
	      strcpy (nome_vincente, nomeOfferente);
	      if ((fileOfferte = fopen (FILENAME, "w")) == NULL)
		{		/* Aggiorno il file */
		  invia_msg (socketfd, "Errore di apertura del file\n");
		  printf ("Errore di apertura del file\n");
		}

	      else
		{
		  fprintf (fileOfferte, "%s %d", nome_vincente, offerta_vincente);
		  fclose (fileOfferte);
		}
	      invia_msg (socketfd, "Offerta accettata\n");
	      printf ("Nuova offerta da superare: %s %d\n", nome_vincente, offerta_vincente);
	    }

	  else
	    {			
	      invia_msg (socketfd, "Spiacente, e` stata fatta un'offerta superiore\n");
	    }
	}
      if (n_offerte > MAXOFFERTE)
	{		
	  printf ("Termine asta : si aggiudica il sig. %s con l'offerta di Euro %d \n", nome_vincente, offerta_vincente);
	  exit (0);
	}
    }

  /* Chiusura del socket */
  if (close (socketfd) != 0)
    {
      printf ("Errore nella chiusura della socket");
      return 0;
    }
  return 0;
}
