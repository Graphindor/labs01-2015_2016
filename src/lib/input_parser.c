#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

// MAXLEN of strings:
#define MAXLEN 100
typedef char string[MAXLEN];

#include "fifo_handler.h"
#include "file_manager.h"
#include "split_search.h"
#include "input_parser.h"

void print_help () {
  printf("\nUsage: \n" );
  printf("splitsearch -i <input file> -o <output file> -v <value> [-m <max limit>] \n\n");
  exit(0);
}

void print_error(char * message)
{
    printf("# ");
    printf("\x1B[31m");
    printf("%s\n", message);
    printf("\x1B[0m");
    exit(0);
}

void get_input(int *argc, char **argv, string * out_file, string * input_file, string * value, int * max)
{
    int i;

    for (i = 1; i < *argc; i++) {
        //printf("argv[%d] = %s\n", i, argv[i]);

        if(strcmp(argv[i], "-m")!=0 && strcmp(argv[i], "-i")!=0 && strcmp(argv[i], "-o")!=0 && strcmp(argv[i], "-v")!=0 && strcmp(argv[i], "-h")!=0) {
            print_error("Error: invalid arguments");
            exit(0);
        }

        if(strcmp(argv[i], "-i")==0) {
            if (strcmp(*input_file, "")==0) {
                i++;
                if (i>=*argc) {
                    print_error("Error: input file");
                    exit(0);
                }
                if (strncmp(argv[i], "-", 1)==0) {
                    print_error("Error: input file");
                    exit(0);
                }
                strcpy(*input_file,argv[i]);
            } else {
                print_error("Error: input file");
                exit(0);
            }
        }

        if(strcmp(argv[i], "-o")==0) {

            if (strcmp(*out_file, "")==0) {
                i++;
                if (i>=*argc) {
                    print_error("Error: output file");
                    exit(0);
                }
                if (strncmp(argv[i], "-", 1)==0) {
                    print_error("Error: output file");
                    exit(0);
                }
                strcpy(*out_file,argv[i]);
            } else {
                print_error("Error: output file");
                exit(0);
            }
        }

        if(strcmp(argv[i], "-v")==0) {
            if (strcmp(*value, "")==0) {
                i++;
                if (i>=*argc) {
                    print_error("Error: value");
                    exit(0);
                }
                strcpy(*value,argv[i]);
            } else {
                print_error("Error: value");
                exit(0);
            }
        }

        if(strcmp(argv[i], "-m")==0) {
            if (*max==-1) {
                i++;
                if (i>=*argc) {
                    print_error("Error: max result");
                    exit(0);
                }
                if (strncmp(argv[i], "-", 1)==0) {
                    print_error("Error: max result");
                    exit(0);
                }

                *max = atoi(argv[i]);

                if(*max==0) {
                  print_error("Error: invalid max result");
                  exit(0);
                }
            } else {
                print_error("Error: max result");
                exit(0);
            }
        }

        if(strcmp(argv[i], "-h")==0) {
          print_help();
          exit(0);
        }

    }

    if (strcmp(*input_file, "")==0 || strcmp(*out_file, "")==0 || strcmp(*value, "")==0) {
        print_error("Error: missing argument");
        print_help();
        exit(0);
    }

}
