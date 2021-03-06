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
/Funzione di controllo della raggiunta dal limite dei risultati da ritornare
*/
void check_limit(int limit, pid_t general_master_id)
{
	//Check valore di risultati già trovati e riscrittura per tenerlo dentro la fifo
	int val = read_fifo();
	write_fifo(val);

	//Termina programma risalendo la gerarchia dei processi
	if(val >= limit)
	{
		int id = getpid();
		char idc[100];

		sprintf(idc, "Terminazione processo figlio - id: %d", id);

		if(general_master_id != id)
		{

			printf("\033[1A");
			printf("\033[1A");

			print_alert(idc);
			printf("\033[2K");
			printf("#\n");
			printf("\033[2K");
			printf("#\n");
			exit(0);
			//print_error("Limite risultati trovati raggiunto...\n Esco e risalgo le gerarchie verso il padre per terminare il preogramma");
		}
	}
}

/*
/Funzione "ricorsiva" di split tra un indice di inizio e di fine
/Quando mi rimane un solo elemento lo controllo
*/
void split(pid_t general_master_id, char * found, int inizio, int fine, int depth, char * input, char * output, int limit, int total_length)
{
	//Controllo superamento limite risultati ottenuti se settato
	if(limit > 0)
		check_limit(limit, general_master_id);
	//Controllo se ho più di un indice tra le mani
	//in tal caso taglio a metà la mia fetta di indici
	if(inizio < fine)
	{
		//Calcolo centro
		int centro = (inizio + fine) / 2;
		pid_t result_fork = fork();

		//Il figlio va sempre a sinistra <---
		if(result_fork == 0)
		{
			//Incremento della varibile profondità
			depth++;
			split(general_master_id, found, inizio, centro, depth, input, output, limit, total_length);
		}
		//Il padre aspetta il figlio poi va a destra --->
		else if(result_fork > 0)
		{
			int retval_child;
			wait(&retval_child);
			//Incremento della varibile profondità
			depth++;
			// Vado in profondità e/o continuo la ricerca se il figlio non da errori
			if(retval_child != -1)
				split(general_master_id, found, centro + 1, fine, depth, input, output, limit, total_length);
		}
		//Errore creazione fork() molto improbabile che accada
		//Dovrebbe accadere quando si finisce la memoria o posti per i figli di un processo
		//
		//Il numero di fork possibili aumenta esponanzialmente in base alla profondità
		//dell'albero che si crea seguendo la seguente formula depth = log(#elements)/log(2)
		else
		{
			perror("Troppi fork!!!");
			exit(-1);
		}
	}
	else
	{
		//Valore che verrà letto usando gli offset
		char * item = get_value(input, inizio);

		//Controllo se il valore cercato e quello letto sono identici
		if(strcmp(found, item) == 0)
		{
			//Aggiunge l'indice al file di output in fondo
			add_found(output, inizio + 1);
			//Lettura del valore di match già trovati
			int val = read_fifo();
			//Scrittura del valore di match incrementato di uno
			write_fifo(val + 1);
			//Trovato un match
			print_found(inizio + 1);
		}

		//Aggiorna la progressbar
		print_progressbar(inizio + 1, total_length, 50);

		//Se non sono il padre ritorno 0(zero) per comunicare
		//che è tutto andato a finire bene
		if(general_master_id != getpid())
		{
			//printf("Non sono il padre group id => %d invece il mio id => %d\n",getpgrp(),getpid());
			exit(0);
		}
	}
}
