#ifndef SEND
#define SEND

#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/in.h>

int send_data(int sockfd, char *buffer, size_t len, struct sockaddr_in *dst);
int send_packet_data(int sockfd, char *buffer, size_t len,
		     struct sockaddr_ll *device);

#endif
