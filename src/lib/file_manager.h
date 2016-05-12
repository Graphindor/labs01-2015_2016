#ifndef fifo_handler
#define fifo_handler

void addFound(char * output, int index);
char * get_value(char * input, int index, int show);
int create_offsets(char * input, int show);
void remove_offsets();
#endif
