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

void print_help ()
{
    printf("\nUsage: \n" );
    printf("splitsearch -i <input file> -o <output file> -v <value> [-m <max limit>] \n\n");
    exit(0);
}

void print_error(char * message)
{
    printf("# ");
    printf("\033[31m");
    printf("%s\n", message);
    printf("\033[0m");
    exit(0);
}

void print_message(char * message)
{
    printf("# ");
    printf("\033[32m");
    printf("%s\n", message);
    printf("\033[0m");
}

void print_alert(char * message)
{
    printf("# ");
    printf("\033[33m");
    printf("%s\n", message);
    printf("\033[0m");
}

void print_found(int index)
{
	printf("\033[1A");
	//printf("\033[2K");
	printf("\033[1A");
	//printf("\033[2K");
	printf("# Elemento trovato alla riga %d\n", index);
  printf("\033[2K");
	printf("#\n");
  printf("\033[2K");
	printf("#\n");
}

void print_progressbar(int index, int total_length, int progressbar_lenght)
{

  //printf("\033[1A");
  //printf("\033[2K");
  printf("\r");
  printf("\033[1A");
  printf("\033[2K");

  printf("# Processati %d su %d elementi\n", (index), total_length);
  printf("# ");

	int progressbar_value = 0;
	double percent = 0;
  int period=0;

	percent = index * 100 / total_length;
	progressbar_value = percent * progressbar_lenght / 100;
  period=total_length/120;
  if (period<1)
    period=1;

  char * waiter;

  int waiter_class=0;
  waiter_class=index%period;
  waiter_class=(((index-waiter_class)/period)+1)%4;

  if (waiter_class==0)
    waiter="\\";
  if (waiter_class==1)
    waiter="|";
  if (waiter_class==2)
    waiter="/";
  if (waiter_class==3)
    waiter="-";

  char progressbar [progressbar_value];
  strcpy(progressbar, "");

	int i;
	for (i = 0; i < progressbar_value; i++)
		strcat(progressbar, "=");

	printf("%s - %s - %d%%\r", waiter, progressbar, (int)percent);

}
