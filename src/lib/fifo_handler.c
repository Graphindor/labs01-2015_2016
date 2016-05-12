#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include "fifo_handler.h"

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

/*
/Scrive su fifo l'intero s che rappresenta il numero di match trovati
*/
void write_fifo(int s)
{
    printf("scrivo su fifo\n");
	int writing, n;
    //Apertura fifo
    writing = open("myFIFO", O_RDWR | O_NONBLOCK);
    //Scrittura da fifo
    n = write(writing, &s, sizeof(s));
    printf("Ho scritto %d di %i byte.\n", s, n);
}

/*
/Legge da fifo il numero di match trovati per un eventuale aggiornamento
*
/Ritorna il valore letto da myFIFO
*/
int read_fifo()
{
    printf("read_fifo\n");
    
	int lettura, n, s;
    //Apertura fifo
    lettura = open("myFIFO", O_RDWR | O_NONBLOCK);
    n = read(lettura, &s, sizeof(s));
    //Lettura da fifo
    printf("Ho letto %i byte: %d\n", n, s);
    
    return s;
}

/*
/Rimuove il file "myFIFO" se esiste
*/
void remove_fifo()
{
    remove("myFIFO");
}

/*
/Questa funzione si occupa della rimozione della fifo e della creazione della sua "istanza"
*/
void create_fifo()
{
    //cancella la fifo precedente per non far andare in errore la creazione di una nuova fifo
    remove_fifo();

    //Creazione della fifo 
    if (mkfifo("myFIFO", FILE_MODE) == -1)
    {
        perror("myFIFO");
        exit(1);
    }
    
    //Inizializzazione della variabile condivisa con i figli
    write_fifo(0);
}
