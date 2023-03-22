#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <linux/if_ether.h>

#include "sockf.h"
#include "error_func.h"

int init_socket()
{
	int sockfd, enable = 1;

	if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
		err_msg("sockf.c", "init_socket", __LINE__, errno);
	if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(int)) <
	    0)
		err_msg("sockf.c", "init_socket", __LINE__, errno);

	return sockfd;
}

int init_packet_socket()
{
	int sockfd;

	if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
		err_msg("sockf.c", "init_raw_socket", __LINE__, errno);

	return sockfd;
}

int bind_iface(int sockfd, char *iface)
{
	int len = strlen(iface);

	if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, iface, len) == -1) {
		err_msg("sockf.c", "bind_iface", __LINE__, errno);
		return -1;
	}

	return 0;
}

void close_sock(int sockfd)
{
	if (close(sockfd) == -1)
		err_msg("sockf.c", "close_sock", __LINE__, errno);
}
