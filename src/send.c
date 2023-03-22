#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

#include "send.h"
#include "error_func.h"

void send_raw(int sockfd, char *buffer, size_t len,
	      struct sockaddr_in *dest_addr, int count)
{
	socklen_t addrlen = sizeof(*dest_addr);
	int ind;

	for (ind = 0; ind < count; ind += 1) {
		if (sendto(sockfd, buffer, len, 0, (struct sockaddr *)dest_addr,
			   addrlen) < 0)
			err_msg("send.c", "send_packet", __LINE__, errno);
	}
}

void send_packet(int sockfd, char *buffer, size_t len,
		 struct sockaddr_ll *dest_addr, int count)
{
	socklen_t addrlen = sizeof(*dest_addr);
	int ind;

	for (ind = 0; ind < count; ind += 1) {
		if (sendto(sockfd, buffer, len, 0, (struct sockaddr *)dest_addr,
			   addrlen) < 0)
			err_msg("send.c", "send_packet", __LINE__, errno);
	}
}
