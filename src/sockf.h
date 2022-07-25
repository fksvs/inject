#ifndef SOCKF
#define SOCKF

int init_socket();
int init_packet_socket();
int bind_iface(int sockfd, char *iface);
void close_sock(int sockfd);

#endif
