#ifndef fifo_handler
#define fifo_handler

void check_limit(int limit);
void split(pid_t general_master_id, char * found, int inizio, int fine, int depth, char * input, char * output, int limit, int show);
#endif