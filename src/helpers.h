#ifndef HELPERS
#define HELPERS

unsigned int get_address();
int list_interfaces();

unsigned short checksum(unsigned short *ptr, int nbytes);

void err_msg(char *file_name, char *func, int line, int err);
void err_exit(char *msg);

unsigned int rand_addr();
unsigned short rand_port();

#endif
