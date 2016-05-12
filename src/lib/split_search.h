#ifndef split_search
#define split_search

void check_limit(int limit, pid_t general_master_id);
void split(pid_t general_master_id, char * found, int inizio, int fine, int depth, char * input, char * output, int limit, int total_length);

#endif
