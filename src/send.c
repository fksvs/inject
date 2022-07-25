#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/in.h>

#include "send.h"
#include "error_func.h"

int send_data(int sockfd, char *buffer, size_t len, struct sockaddr_in *dst)
{
	if (sendto(sockfd, buffer, len, 0, (struct sockaddr *)dst,
		   sizeof(*dst)) < 0) {
		err_msg("send.c", "send_data", __LINE__, errno);
		return -1;
	}

	return 0;
}

int send_packet_data(int sockfd, char *buffer, size_t len,
		     struct sockaddr_ll *device)
{
	if (sendto(sockfd, buffer, len, 0, (struct sockaddr *)device,
		   sizeof(*device)) < 0) {
		err_msg("send.c", "send_data", __LINE__, errno);
		return -1;
	}

	return 0;
}
