#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "lib/file_manager.h"
#include "lib/fifo_handler.h"
#include "lib/split_search.h"
#include "lib/input_parser.h"

// MAXLEN of strings:
#define MAXLEN 100
typedef char string[MAXLEN];

int main(int argc, char **argv)
{

	string output="";
	string input="";
	string cerca="";
	int limit = -1;

	get_input(&argc, argv, &output, &input, &cerca, &limit);

	printf(">>>>>>>>>>>>>>>Cominciato<<<<<<<<<<<<<<<<");
	//Registrazione process ID del padre "più grande"
	pid_t general_master_id = getpid();

	//Creazione file vuoto di output
	FILE *scrivi = fopen(output, "w");
	fclose(scrivi);
	printf("\n");

	//Creazione della fifo
	create_fifo();

	printf("creazione offset\n");
	//Calcolo righe totali (#insieme di dati)
	//e creazione del file temporaneo di offset
	int lines = create_offsets(input, 1);

	//Chiamata della funzione splt_search
	split(general_master_id, cerca, 0, lines - 1, 0, input, output, limit, 0);

	int found_count = read_fifo();
	//Riussanto dei valori trovati depositati nella fifo
	printf("valori trovati %d su %d elementi...\n", found_count, lines);

	if(found_count == 0)
		addFound(output, found_count);

	//Rimozione del file temporaneo di offsets
	remove_offsets();

	//Fase finale di uscita del padre unico superstite
	printf("Sono il padre e sto terminando\n");
	printf("\n");
	return 0;
}
