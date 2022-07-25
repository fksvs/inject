#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>

#include "sockf.h"
#include "get_addr.h"
#include "send.h"
#include "data.h"
#include "error_func.h"
#include "output.h"
#include "type.h"
#include "eth.h"
#include "usage.h"

static unsigned char src_mac[6], dst_mac[6];
static unsigned short protocol;
static int count = 1, verbose = 0, src_mac_control = 0, dst_mac_control = 0;
static char *file_name = NULL, *iface = NULL;

void set_eth(char *buffer, unsigned char *dst, unsigned char *src,
	     unsigned short proto, char *payload, size_t payload_size)
{
	eth_hdr *ethh = (eth_hdr *)buffer;
	char *ptr = (buffer + sizeof(eth_hdr));

	strncat(ptr, payload, payload_size);
	memcpy(ethh->dst, dst, 6);
	memcpy(ethh->src, src, 6);
	ethh->protocol = htons(proto);
}

static void usage()
{
	general_usage();
	eth_usage();
	printf("\n");

	exit(EXIT_FAILURE);
}

static void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 3)
		usage();

	while ((opt = getopt(argc, argv, "i:c:vhM:K:p:a:")) != -1) {
		switch (opt) {
		case 'i':
			iface = optarg;
			break;
		case 'c':
			count = atoi(optarg);
			break;
		case 'v':
			verbose = 1;
			break;
		case 'h':
			usage();
			break;
		case 'M':
			sscanf(optarg, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
			       &src_mac[0], &src_mac[1], &src_mac[2],
			       &src_mac[3], &src_mac[4], &src_mac[5]);
			src_mac_control = 1;
			break;
		case 'K':
			sscanf(optarg, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
			       &dst_mac[0], &dst_mac[1], &dst_mac[2],
			       &dst_mac[3], &dst_mac[4], &dst_mac[5]);
			dst_mac_control = 1;
			break;
		case 'p':
			protocol = atoi(optarg);
			break;
		case 'a':
			file_name = optarg;
			break;
		case '?':
			break;
		}
	}
}

void inject_eth(int argc, char *argv[])
{
	char buffer[BUFF_SIZE], *payload;
	struct ifreq ifr;
	struct sockaddr_ll device;
	int sockfd, ind, len;
	size_t payload_size = 0;

	parser(argc, argv);

	memset(buffer, 0, BUFF_SIZE);
	memset(&ifr, 0, sizeof(struct ifreq));
	memset(&device, 0, sizeof(struct sockaddr_ll));

	if ((sockfd = init_packet_socket()) == -1)
		exit(EXIT_FAILURE);

	if (!iface)
		err_exit("network interface not specified.");

	if (!src_mac_control) {
		memcpy(ifr.ifr_name, iface, strlen(iface));
		if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1)
			err_msg("eth.c", "inject_eth", __LINE__, errno);

		memcpy(src_mac, ifr.ifr_hwaddr.sa_data, 6);
	}

	if (!dst_mac_control)
		memset(dst_mac, 0xff, 6);

	if ((device.sll_ifindex = if_nametoindex(iface)) == 0)
		err_msg("eth.c", "inject_eth", __LINE__, errno);
	device.sll_family = AF_PACKET;
	memcpy(device.sll_addr, src_mac, 6);
	device.sll_halen = 6;

	if (file_name) {
		if ((payload = read_file(file_name)) == NULL)
			exit(EXIT_FAILURE);
		payload_size = strlen(payload);
	}

	set_eth(buffer, dst_mac, src_mac, protocol, payload, payload_size);

	len = sizeof(eth_hdr) + payload_size;
	for (ind = 0; ind < count; ind += 1)
		send_packet_data(sockfd, buffer, len, &device);

	if (verbose) {
		eth_hdr *eth = (eth_hdr *)buffer;
		print_eth(eth);
	}

	close_sock(sockfd);
	exit(EXIT_SUCCESS);
}
