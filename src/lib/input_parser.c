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
/Funzione di processamento della stringa di input
*/
void get_input(int argc, char ** argv, char ** output_file, char ** input_file, char ** value, int * max)
{
    //Inizializzazione della varibile di indice
    int i;

    //Iterazione degli argomenti
    for (i = 1; i < argc; i++)
    {
        //In caso il marcatore sia diverso da quelli processati
        if(strcmp(argv[i], "-m") != 0 && strcmp(argv[i], "-i") != 0 && strcmp(argv[i], "-o") != 0 && strcmp(argv[i], "-v") != 0 && strcmp(argv[i], "-h") != 0)
            //Segnalazione di errore e uscita dal programma
            print_error("Errore: Argomenti invalidi");

        //In caso il marcatore sia quello di input
        if(strcmp(argv[i], "-i") == 0)
        {
            //In caso la variabile di percorso di input sia vuota
            if (strcmp(*input_file, "") == 0)
            {
                //Incremento della varibile di indice per andare a vedere cosa c'è dopo -i
                i++;

                //In caso non cia siano altri argomenti es: -i senza niente dopo
                if (i >= argc)
                    //Segnalazione di errore e uscita dal programma
                    print_error("Errore: input file\n");

                //In caso il primo carattere sia "-" (trattino) è un percorso inconsistente
                if (strncmp(argv[i], "-", 1) == 0)
                    //Segnalazione di errore e uscita dal programma
                    print_error("Errore: input file\n");

                //Allocazione dei byte che servono per contenere il percorso di input
                *input_file = (char *)malloc(sizeof(char) * strlen(argv[i]));
                //Copia dell'argomento nella variabile di input
                strcpy(*input_file, argv[i]);
            }
            else
                //Segnalazione di errore e uscita dal programma
                print_error("Errore: input file");
        }

        //In caso il marcatore sia quello di output
        if(strcmp(argv[i], "-o") == 0)
        {
            //In caso la variabile di percorso di output sia vuota
            if (strcmp(*output_file, "") == 0)
            {
                //Incremento della varibile di indice per andare a vedere cosa c'è dopo -o
                i++;

                //In caso non cia siano altri argomenti es: -o senza niente dopo
                if (i >= argc)
                    //Segnalazione di errore e uscita dal programma
                    print_error("Errore: output file\n");

                //In caso il primo carattere sia "-" (trattino) è un percorso inconsistente
                if (strncmp(argv[i], "-", 1) == 0)
                    //Segnalazione di errore e uscita dal programma
                    print_error("Errore: output file\n");

                //Allocazione dei byte che servono per contenere il percorso di output
                *output_file = (char *)malloc(sizeof(char) * strlen(argv[i]));
                //Copia dell'argomento nella variabile di output
                strcpy(*output_file, argv[i]);
            }
            else
                //Segnalazione di errore e uscita dal programma
                print_error("Errore: output file\n");
        }

        //In caso il marcatore sia quello del valore da cercare
        if(strcmp(argv[i], "-v") == 0)
        {
            if (strcmp(*value, "") == 0)
            {
                //Incremento della varibile di indice per andare a vedere cosa c'è dopo -v
                i++;

                //In caso non cia siano altri argomenti es: -v senza niente dopo
                if (i >= argc)
                    //Segnalazione di errore e uscita dal programma
                    print_error("Errore: value\n");

                //Allocazione dei byte che servono per contenere il percorso di output
                *value = (char *)malloc(sizeof(char) * strlen(argv[i]));
                //Copia dell'argomento nella variabile del valore da cercare
                strcpy(*value, argv[i]);
            }
            else
                //Segnalazione di errore e uscita dal programma
                print_error("Error: value\n");
        }

        if(strcmp(argv[i], "-m") == 0)
        {
            i++;
            //In caso la variabile di max non sia ancora stata settata
            if (*max == -1)
            {
                //In caso non cia siano altri argomenti es: -v senza niente dopo
                if (i >= argc)
                    //Segnalazione di errore e uscita dal programma
                    print_error("Error: amax result\n");

                //In caso il primo carattere sia "-" (trattino) cioè un limite negativo
                if (strncmp(argv[i], "-", 1) == 0)
                    //Segnalazione di errore e uscita dal programma
                    print_error("Error: cmax result\n");

                //Conversione da stringa a intero della variabile di massimo #risultati
                *max = atoi(argv[i]);

                if(*max == 0)
                    //Segnalazione di errore e uscita dal programma
                    print_error("Error: invalid max result\n");
            }
            else
                //Segnalazione di errore e uscita dal programma
                print_error("Error: vmax result\n");
        }

        //In caso il marcatore sia quello di aiuto (help)
        if(strcmp(argv[i], "-h") == 0)
            //Stampa del pattern degli argomenti del programma
            print_help();
    }

    //In caso un marcatore obbligatorio sia vuoto
    if (strcmp(* input_file, "") == 0 || strcmp(* output_file, "") == 0 || strcmp(* value, "") == 0)
    {
        printf("Error: missing argument\n");
        //Stampa del pattern degli argomenti del programma
        print_help();
    }
}
