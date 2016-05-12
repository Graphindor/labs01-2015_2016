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

void get_input(int *argc, char **argv, string * out_file, string * input_file, string * value, int * max)
{
    int i;

    for (i = 1; i < *argc; i++) {
        //printf("argv[%d] = %s\n", i, argv[i]);

        if(strcmp(argv[i], "-m")!=0 && strcmp(argv[i], "-i")!=0 && strcmp(argv[i], "-o")!=0 && strcmp(argv[i], "-v")!=0 && strcmp(argv[i], "-h")!=0) {
            printf("Error: invalid argument\n%s",argv[i]);
            exit(0);
        }

        if(strcmp(argv[i], "-i")==0) {
            if (strcmp(*input_file, "")==0) {
                i++;
                if (i>=*argc) {
                    printf("Error: input file\n");
                    exit(0);
                }
                if (strncmp(argv[i], "-", 1)==0) {
                    printf("Error: input file\n");
                    exit(0);
                }
                strcpy(*input_file,argv[i]);
            } else {
                printf("Error: input file\n");
                exit(0);
            }
        }

        if(strcmp(argv[i], "-o")==0) {

            if (strcmp(*out_file, "")==0) {
                i++;
                if (i>=*argc) {
                    printf("Error: output file\n");
                    exit(0);
                }
                if (strncmp(argv[i], "-", 1)==0) {
                    printf("Error: output file\n");
                    exit(0);
                }
                strcpy(*out_file,argv[i]);
            } else {
                printf("Error: output file\n");
                exit(0);
            }
        }

        if(strcmp(argv[i], "-v")==0) {
            if (strcmp(*value, "")==0) {
                i++;
                if (i>=*argc) {
                    printf("Error: value\n");
                    exit(0);
                }
                strcpy(*value,argv[i]);
            } else {
                printf("Error: value\n");
                exit(0);
            }
        }

        if(strcmp(argv[i], "-m")==0) {
            if (*max==-1) {
                i++;
                if (i>=*argc) {
                    printf("Error: max result\n");
                    exit(0);
                }
                if (strncmp(argv[i], "-", 1)==0) {
                    printf("Error: max result\n");
                    exit(0);
                }

                *max = atoi(argv[i]);

                if(*max==0) {
                  printf("Error: invalid max result\n");
                  exit(0);
                }
            } else {
                printf("Error: max result\n");
                exit(0);
            }
        }

        if(strcmp(argv[i], "-h")==0) {
          print_help();
          exit(0);
        }

    }

    if (strcmp(*input_file, "")==0 || strcmp(*out_file, "")==0 || strcmp(*value, "")==0) {
        printf("Error: missing argument\n");
        print_help();
        exit(0);
    }

}
