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

void get_input(int argc, char ** argv, char ** output_file, char ** input_file, char ** value, int * max)
{
    int i;

    for (i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-m") != 0 && strcmp(argv[i], "-i") != 0 && strcmp(argv[i], "-o") != 0 && strcmp(argv[i], "-v") != 0 && strcmp(argv[i], "-h") != 0)
            print_error("Errore: Argomenti invalidi");

        if(strcmp(argv[i], "-i") == 0)
        {
            if (strcmp(*input_file, "") == 0)
            {
                i++;

                if (i >= argc)
                    print_error("Errore: input file\n");

                if (strncmp(argv[i], "-", 1) == 0)
                    print_error("Errore: input file\n");

                *input_file = (char *)malloc(sizeof(char) * strlen(argv[i]));
                strcpy(*input_file, argv[i]);
            }
            else
                print_error("Errore: input file");
        }

        if(strcmp(argv[i], "-o") == 0)
        {
            if (strcmp(*output_file, "") == 0)
            {
                i++;
                if (i >= argc)
                    print_error("Errore: output file\n");

                if (strncmp(argv[i], "-", 1) == 0)
                    print_error("Errore: output file\n");

                *output_file = (char *)malloc(sizeof(char) * strlen(argv[i]));
                strcpy(*output_file, argv[i]);
            }
            else
                print_error("Errore: output file\n");
        }

        if(strcmp(argv[i], "-v") == 0)
        {
            if (strcmp(*value, "") == 0)
            {
                i++;

                if (i >= argc)
                    print_error("Errore: value\n");

                *value = (char *)malloc(sizeof(char) * strlen(argv[i]));
                strcpy(*value, argv[i]);
            }
            else
                print_error("Error: value\n");
        }

        if(strcmp(argv[i], "-m") == 0)
        {
            if (*max == -1)
            {
                if (i >= argc)
                    print_error("Error: max result\n");

                if (strncmp(argv[i], "-", 1) == 0)
                    print_error("Error: max result\n");

                *max = atoi(argv[i]);

                if(*max == 0)
                    print_error("Error: invalid max result\n");
            }
            else
                print_error("Error: max result\n");
        }

        if(strcmp(argv[i], "-h") == 0)
            print_help();
    }

    if (strcmp(* input_file, "") == 0 || strcmp(* output_file, "") == 0 || strcmp(* value, "") == 0)
    {
        printf("Error: missing argument\n");
        print_help();
    }
}
