#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "fifo_handler.h"
#include "file_manager.h"
#include "split_search.h"
#include "input_parser.h"

/*
/Aggiunta del nuovo indice in fondo al file di <output>
*/
void addFound(char * output, int index)
{
	//Apertura del file di output in modalità append
	FILE * writer = fopen(output, "a+");

	//Scrittura nuovo indice
	fprintf(writer, "%d\n", index);
	//Chiusura e conseguente salvataggio del nuovo indice
	fclose(writer);
}

/*
/ Lettura del valore all'ennesima posizione
*/
char * get_value(char * input, int index, int show)
{
	FILE * reader_offset = fopen("offsets.txt", "r");
	// Inizializzazione delle variabili riga attuale e somma offset
	int jumping_bytes = 0;
	int lines = 0;
	int value, reading_bytes;

	// printf("\n\n\n\nsearching at index => %d\n", index);

	while(lines < index + 1 && fscanf(reader_offset, "%d", &value) != -1)
	{
		//Incremento il numero di riga e sommo l'offset delle righe da saltare
		lines++;

		jumping_bytes += value;
	}

	//Lettura della quantità di byte da leggere
	fscanf(reader_offset, "%d", &reading_bytes);

	//Apertura file "reale" degli input
	FILE * reader = fopen(input, "r");

	//Spostamento dell'indicatore della posizione da leggere del file contenuto in reader
	fseek(reader, jumping_bytes, SEEK_SET);

	//Allocazione della memoria necessaria per contenere il valore di input
	char * retval = (char *) malloc(sizeof(char) * reading_bytes);

	//Lettura di <reading_bytes> da reader in <retval>
	fgets(retval, reading_bytes, reader);

	if(show == 1)
		printf("\t\tHo letto %s saltando %d bytes\n", retval, jumping_bytes);

	//Chiusura degli stream
	fclose(reader);
	fclose(reader_offset);

	return retval;
}

/*
/Crea offsets.txt per potere sapere quanti byte saltare
/per leggere l'input e quanti byte leggere
*
/Ritorna anche la cardinalità dell'insieme dei dati
*/
int create_offsets(char * input, int show)
{
	//Apertura <input>
	if( access( input, F_OK ) == -1 ) {
    print_error("Error: input file does not exist.");
		exit(0);
  }

	FILE *reader = fopen(input, "r");
	//Creazione del file temporaneo di offsets
	FILE *writer = fopen("offsets.txt", "w");
	//Creazione variabile che conterrà la lunghezza di una riga
	int size;
	//Contatore dimensione totale del file
	int total_size = 0;

	//Inizializzazione contatore di righe e aggiunta primo offset 0(zero)
	int count = 0;
	fprintf(writer, "%d\n", 0);

	//Creazione variabili per contenere e misurare la riga appena letta
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	//Lettura del file di input fino a eof
	while(((read = getline(&line, &len, reader)) != -1))
	{
		//Calcolo lunghezza della stringa incluso '\n'
		//di cui si deve tenere conto se si vuole
		//leggere da input saltando tot bytes
		int size = (int)strlen(line);
		//Aggiornamento variabili riga e  dimensione totale
		total_size += size;
		count++;
		if(show == 1)
			//printf("Ho letto %s, with size %d\n", line, size);
		//Aggiunta al file di offsets
		fprintf(writer, "%d\n", size);
	}

	if(show == 1)
		//printf("\n\nIl file pesa %d\tE ha %d righe\n", total_size, count);

	//Chiusura stream e conseguente salvataggio del file di offsets
	fclose(reader);
	fclose(writer);

	return count;
}

/*
/ Cancello file temporaneo di offsets.txt
*/
void remove_offsets()
{
	remove("offsets.txt");
}
