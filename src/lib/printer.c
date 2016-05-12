#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "printer.h"
#include "fifo_handler.h"
#include "file_manager.h"
#include "split_search.h"
#include "input_parser.h"

/*
/Scrive messagio di help che spiega il corretto input da dare al programma
/Non importa l'ordine di immissione
*/
void print_help ()
{
	printf("\nUsage: \n" );
	printf("splitsearch -i <input file> -o <output file> -v <value> [-m <max limit>] \n\n");
	exit(0);
}

/*
/Scrive un messaggio di errore (in rosso) e dopo esce dall'esecuzione
*/
void print_error(char * message)
{
	printf("# ");
	//Cambio colore in [01;31m (rosso acceso)
	printf("\033[01;31m");
	//Stampa di <message>
	printf("%s\n", message);
	//Rispristino del colore di default [0m (bianco)
	printf("\033[0m");
	exit(0);
}

/*
/Scrive un messaggio (in verde), questa funzione è usata per segnalare il completamento
/di alcuni passaggi del programma
*/
void print_message(char * message)
{
	printf("# ");
	//Cambio colore in [01;32 (verde acceso)
	printf("\033[01;32m");
	//Stampa di <message>
	printf("%s\n", message);
	//Rispristino del colore di default [0m (bianco)
	printf("\033[0m");
}

/*
/Scrive un alert (in giallo)
*/
void print_alert(char * message)
{
	//Cambio colore in [01;33m (rosso acceso)
	printf("# ");
	printf("\033[01;33m");
	//Stampa a video del messaggio
	printf("%s\n", message);
	//Ripristinare il colore in [0m (bianco)
	printf("\033[0m");
}

/*
/Segnala il ritrovamento di una corrispondenza ad un certo indice
*/
void print_found(int index)
{
	//Riposizionamento del cursore
	printf("\033[1A");
	printf("\033[1A");
	//Stampa dell'indice della corrispondenza
	printf("# Elemento trovato alla riga %d\n", index);
	//Cancellamento della riga dove si trova il cursore
	printf("\033[2K");
	printf("#\n");
	printf("\033[2K");
	printf("#\n");
}

/*
/Aggiorna la progressbar
*/
void print_progressbar(int index, int total_length, int progressbar_lenght)
{
	//Riposizionamento del cursore e pulizia della riga
	printf("\r");
	printf("\033[1A");
	printf("\033[2K");

	//Stampa punto di processamento
	printf("# Processati %d su %d elementi\n", (index), total_length);
	printf("# ");

	//Inizializzazione delle varibili per il calcolo del punto di "progresso"
	int progressbar_value = 0;
	double percent = 0;
	int period = 0;

	//Calcolo delle variabili per il completamento e girandola
	percent = index * 100 / total_length;
	progressbar_value = percent * progressbar_lenght / 100;
	period = total_length / 120;

	//Ripristino variabile di <period> se fuori dai limiti
	if (period < 1)
		period = 1;

	//Creazione della girandola
	char * waiter;

	//Inizializzazione della girandola
	int waiter_class = 0;
	//Calcolo della posizione della girandola
	waiter_class = (((index - waiter_class) / period) + 1) % 4;

	switch(waiter_class)
	{
		case 0:

			waiter = "\\";
			break;

		case 1:

			waiter = "|";
			break;

		case 2:

			waiter = "/";
			break;

		case 3:

			waiter = "-";
			break;

		default:
			break;
	}

	//Creazione della progressbar
	char progressbar [progressbar_value];
	//Inizializzazione della progressbar
	strcpy(progressbar, "");

	//Concatenamento di "=" sulla progressbar
	int i;
	for (i = 0; i < progressbar_value; i++)
		strcat(progressbar, "=");

	//Stampa della girandola, progressbar e la percentuale raggiunta
	printf("%s - %s - %d%%\r", waiter, progressbar, (int)percent);
}
