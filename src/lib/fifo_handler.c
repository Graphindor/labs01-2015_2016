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

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

/*
/Scrive su fifo l'intero s che rappresenta il numero di match trovati
*/
void write_fifo(int s)
{
    //Inizializzazione varibili di scrittura
    int writing, n;
    //Apertura fifo
    writing = open("myFIFO", O_RDWR | O_NONBLOCK);
    //Scrittura da fifo
    n = write(writing, &s, sizeof(s));
}

/*
/Legge da fifo il numero di match trovati per un eventuale aggiornamento
*
/Ritorna il valore letto da myFIFO
*/
int read_fifo()
{
    //Inizializzazione variabili di lettura
    int lettura, n, s;
    //Apertura fifo
    lettura = open("myFIFO", O_RDWR | O_NONBLOCK);
    //Lettura da fifo
    n = read(lettura, &s, sizeof(s));
    //Ritorno del valore letto da myFIFO
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
        //Seganalazione di errore se la creazione della fifo non è andata a buon fine
        print_error("Error: creazione di myFIFO");

    //Inizializzazione della variabile condivisa con i figli
    write_fifo(0);
	//Seganalazione dell'avvenuto comlpetamento della fifo senza errori
    print_message("FIFO correctly generated.");
}
