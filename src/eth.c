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

#include "eth.h"
#include "sockf.h"
#include "type.h"
#include "send.h"
#include "data.h"
#include "error_func.h"
#include "output.h"
#include "usage.h"

static char buffer[BUFF_SIZE];
static eth_hdr eth;
static struct sockaddr_ll device;
static int sockfd;
static int payload_size = 0;

static char *iface;
static int count = 1;
static int verbose = 0;

void build_eth(eth_hdr *eth, unsigned char *dst_mac, unsigned char *src_mac,
	       unsigned short protocol)
{
	memcpy(eth->dst, dst_mac, ETHER_ADDR_LEN);
	memcpy(eth->src, src_mac, ETHER_ADDR_LEN);
	eth->protocol = htons(protocol);
}

static void fill_device()
{
	if ((device.sll_ifindex = if_nametoindex(iface)) == 0)
		err_msg("eth.c", "fill_device", __LINE__, errno);

	device.sll_family = AF_PACKET;
	memcpy(device.sll_addr, &eth.src, ETHER_ADDR_LEN);
	device.sll_halen = ETHER_ADDR_LEN;
}

static int fill_payload(char *file_name)
{
	char *payload;

	if ((payload = read_file(file_name)) == NULL) {
		err_msg("eth.c", "fill_payload", __LINE__, errno);
		return -1;
	}
	payload_size = strlen(payload);
	strncat(buffer + sizeof(eth_hdr), payload, payload_size);

	return 0;
}

static void validate_eth_packet()
{
	char zero[BUFF_SIZE];
	memset(zero, 0, BUFF_SIZE);

	if (!iface)
		err_exit("network interface not specified.");

	if (!memcmp(&eth.src, zero, ETHER_ADDR_LEN)) {
		struct ifreq ifr;
		memset(&ifr, 0, sizeof(struct ifreq));

		memcpy(ifr.ifr_name, iface, strlen(iface));
		if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1)
			err_msg("eth.c", "validate_eth_packet", __LINE__,
				errno);

		memcpy(&eth.src, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
	}

	if (!memcmp(&eth.dst, zero, ETHER_ADDR_LEN))
		memset(&eth.dst, 0xff, ETHER_ADDR_LEN);
}

static void get_mac_address(char *data, char *mac)
{
	sscanf(data, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx:", &mac[0], &mac[1],
	       &mac[2], &mac[3], &mac[4], &mac[5]);
}

static void usage()
{
	general_usage();
	eth_usage();
	printf("\n");

	exit(EXIT_FAILURE);
}

static void parse_eth(int argc, char *argv[])
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
		case 'M':
			char src_mac[6];
			get_mac_address(optarg, src_mac);
			memcpy(eth.src, src_mac, ETHER_ADDR_LEN);
			break;
		case 'K':
			char dst_mac[6];
			get_mac_address(optarg, dst_mac);
			memcpy(eth.dst, dst_mac, ETHER_ADDR_LEN);
			break;
		case 'p':
			unsigned short protocol = atoi(optarg);
			eth.protocol = htons(protocol);
			break;
		case 'a':
			char *file_name = optarg;
			fill_payload(file_name);
			break;
		case '?':
			break;
		}
	}
}

void inject_eth(int argc, char *argv[])
{
	memset(buffer, 0, BUFF_SIZE);
	memset(&eth, 0, sizeof(eth_hdr));
	memset(&device, 0, sizeof(struct sockaddr_ll));

	if ((sockfd = init_packet_socket()) == -1)
		exit(EXIT_FAILURE);

	parse_eth(argc, argv);
	validate_eth_packet();
	fill_device();
	memcpy(buffer, &eth, sizeof(eth_hdr));

	int len = sizeof(eth_hdr) + payload_size;
	send_packet(sockfd, buffer, len, &device, count);

	if (verbose)
		print_eth(&eth);

	close_sock(sockfd);
	exit(EXIT_SUCCESS);
}
