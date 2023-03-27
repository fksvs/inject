#ifndef HELPERS
#define HELPERS

 #include <sys/socket.h>
 #include <linux/if_packet.h>
 #include <net/ethernet.h>

unsigned short checksum(unsigned short *ptr, int nbytes);

void err_msg(char *file_name, char *func, int line, int err);
void err_exit(char *msg);

void fill_device(struct sockaddr_ll *device, char *iface, unsigned char *mac);
unsigned int get_address();
int list_interfaces();
void read_mac_address(char *data, char *mac);

unsigned int rand_addr();
unsigned short rand_port();

#endif
