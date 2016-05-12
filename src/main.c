#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#include "lib/printer.h"
#include "lib/fifo_handler.h"
#include "lib/file_manager.h"
#include "lib/split_search.h"
#include "lib/input_parser.h"

int main(int argc, char **argv)
{
	//Inizializzazione delle varibili che conterranno gli argomenti se correttamente inseriti
	char * output = "";
	char * input = "";
	char * cerca = "";
	int limit = -1;

	//Segnalazione dell'inizio del programma
	printf("\n" );
	printf("####################Inizio####################\n");
	printf("#\n");

	//parsing degli argomenti
	get_input(argc, argv, &output, &input, &cerca, &limit);

	//Stampa delle variabili ricavate dal parsing degli argomenti (in blu)
	printf("#\n");
	printf("# Argomenti:\n");
	printf("# \t\tinput:\t\033[01;36m%s\x1B[0m\n", input);
	printf("# \t\toutput:\t\033[01;36m%s\x1B[0m\n", output);
	printf("# \t\tvalue:\t\033[01;36m%s\x1B[0m\n", cerca);

	if (limit != -1)
		printf("# \t\tlimite:\t\033[01;36m%d\x1B[0m\n", limit);

	printf("#\n");
	//Registrazione process ID del padre "più grande"
	pid_t general_master_id = getpid();

	//Creazione file vuoto di output
	FILE *scrivi = fopen(output, "w");
	fclose(scrivi);

	//Segnalazione della creazione della fifo
	print_alert("Creazione myFIFO");
	//Creazione della fifo
	create_fifo();

	printf("#\n");

	//Segnalazione della creazione degli offset
	print_alert("Creazione file offset");
	//Calcolo righe totali (#insieme di dati)
	//e creazione del file temporaneo di offset
	int lines = create_offsets(input);

	//Stampa numero di elementi trovati
	printf("#\n");
	printf("# Numero elementi: %d\n", lines);
	printf("#\n");
	printf("#\n");
	printf("#\n");

	//Chiamata della funzione split_search
	split(general_master_id, cerca, 0, lines - 1, 0, input, output, limit, lines);

	//Ripescaggio del numero di valori trovati
	int found_count = read_fifo();
	//Riussanto dei valori trovati depositati nella fifo

	printf("#\n");
	printf("#\n");

	if(found_count >= limit)
	{
		print_message("Limite raggiunto.");
	}

	//Stampa delle corrispondenze trovate
	print_message("Ricerca terminata.");
	printf("# Sono state trovate %d corrispondenze su %d elementi\n", found_count, lines);
	printf("#\n");
	print_message("Output salvato correttamente");
	printf("# \t\033[01;36m%s\x1B[0m\n", output);

	if(found_count == 0)
		add_found(output, found_count);

	//Rimozione del file temporaneo di offsets
	remove_offsets();

	//Fase finale di uscita del padre unico superstite
	printf("#\n");
	printf("#####################Fine#####################\n");
	printf("\n");

	return 0;
}
