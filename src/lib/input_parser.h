#ifndef input_parser
#define input_parser

// MAXLEN of strings:
#define MAXLEN 100
typedef char string[MAXLEN];

void get_input(int *argc, char **argv, string * out_file, string * input_file, string * value, int * max);
void print_help();


#endif
