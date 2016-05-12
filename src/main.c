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

	char * output="";
	char * input="";
	char * cerca="";
	int limit = -1;

	printf("\n" );
	printf("####################Inizio####################\n");
	printf("#\n");

	get_input(argc, argv, &output, &input, &cerca, &limit);

	printf("#\n");
	printf("# Arguments:\n");
	printf("# \tinput:\t\033[36m%s\x1B[0m\n", input);
	printf("# \toutput:\t\033[36m%s\x1B[0m\n", output);
	printf("# \tvalue:\t\033[36m%s\x1B[0m\n", cerca);
	if (limit != -1) {
		printf("# \tlimit:\t\033[36m%d\x1B[0m\n", limit);
	}
	printf("#\n");
	//Registrazione process ID del padre "più grande"
	pid_t general_master_id = getpid();

	//Creazione file vuoto di output
	FILE *scrivi = fopen(output, "w");
	fclose(scrivi);

	print_alert("Creazione FIFO");
	//Creazione della fifo
	create_fifo();

	printf("#\n");

	print_alert("Creazione file offset");
	//Calcolo righe totali (#insieme di dati)
	//e creazione del file temporaneo di offset
	int lines = create_offsets(input);

	printf("#\n");
	printf("# Numero elementi: %d\n", lines);
	printf("#\n");
	printf("#\n");
	printf("#\n");

	//Chiamata della funzione split_search
	split(general_master_id, cerca, 0, lines - 1, 0, input, output, limit, lines);

	int found_count = read_fifo();
	//Riussanto dei valori trovati depositati nella fifo

	printf("#\n");
	printf("#\n");
	print_message("Ricerca terminata.");
	printf("# Sono state trovate %d corrispondenze su %d elementi\n", found_count, lines);
	printf("#\n");
	print_message("Output correctly saved on:");
	printf("# \t\033[36m%s\x1B[0m\n", output);

	if(found_count == 0)
		add_found(output, found_count);

	//Rimozione del file temporaneo di offsets
	remove_offsets();
	
	printf("#\n");
	//Fase finale di uscita del padre unico superstite
	printf("#####################Fine#####################\n");

	printf("\n");
	return 0;
}
