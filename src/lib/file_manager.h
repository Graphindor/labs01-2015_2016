#ifndef fifo_manager
#define fifo_manager

void addFound(char * output, int index);
char * get_value(char * input, int index, int show);
int create_offsets(char * input, int show);
void remove_offsets();

#endif
