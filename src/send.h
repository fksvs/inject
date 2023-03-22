#ifndef SEND
#define SEND

#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/in.h>

void send_raw(int sockfd, char *buffer, size_t len,
	      struct sockaddr_in *dest_addr, int count);
void send_packet(int sockfd, char *buffer, size_t len,
		 struct sockaddr_ll *dest_addr, int count);
#endif
