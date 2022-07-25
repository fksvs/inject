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
#include "error_func.h"
#include "output.h"
#include "type.h"
#include "eth.h"
#include "arp.h"
#include "usage.h"

static unsigned char src_mac[6], dst_mac[6];
static unsigned char *src_ip = NULL, *dst_ip = NULL;
static unsigned short oper;
static int count = 1, verbose = 0, src_mac_control = 0, dst_mac_control = 0;
static char *iface = NULL;

void set_arp(char *buffer, unsigned char *source_mac, unsigned char *source_ip,
	     unsigned char *target_mac, unsigned char *target_ip,
	     unsigned short oper)
{
	arp_hdr *arph = (arp_hdr *)(buffer + sizeof(eth_hdr));

	arph->htype = htons(1);
	arph->ptype = htons(ETH_P_IP);
	arph->hlen = 6;
	arph->plen = 4;
	arph->oper = htons(oper);
	memcpy(arph->src_mac, source_mac, 6);
	inet_pton(AF_INET, (const char *)source_ip, &arph->src_ip);
	memcpy(arph->dst_mac, target_mac, 6);
	inet_pton(AF_INET, (const char *)target_ip, &arph->dst_ip);
}

static void usage()
{
	general_usage();
	arp_usage();
	printf("\n");

	exit(EXIT_FAILURE);
}

static void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 3)
		usage();

	while ((opt = getopt(argc, argv, "i:c:vhM:K:S:D:r:")) != -1) {
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
		case 'S':
			src_ip = (unsigned char *)optarg;
			break;
		case 'D':
			dst_ip = (unsigned char *)optarg;
			break;
		case 'r':
			oper = atoi(optarg);
			break;
		case '?':
			break;
		}
	}
}

void inject_arp(int argc, char *argv[])
{
	char buffer[BUFF_SIZE];
	struct ifreq ifr;
	struct sockaddr_ll device;
	int sockfd, ind, len;

	parser(argc, argv);

	if (!src_ip)
		err_exit("source ip address not specified.");
	if (!dst_ip)
		err_exit("destination ip address not specified.");
	if (!iface)
		err_exit("network interface not specified.");

	memset(buffer, 0, BUFF_SIZE);
	memset(&ifr, 0, sizeof(struct ifreq));
	memset(&device, 0, sizeof(struct sockaddr_ll));

	if ((sockfd = init_packet_socket()) == -1)
		exit(EXIT_FAILURE);

	if (!src_mac_control) {
		memcpy(ifr.ifr_name, iface, strlen(iface));
		if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1)
			err_msg("arp.c", "inject_arp", __LINE__, errno);

		memcpy(src_mac, ifr.ifr_hwaddr.sa_data, 6);
	}

	if (!dst_mac_control)
		memset(dst_mac, 0xff, 6);

	if ((device.sll_ifindex = if_nametoindex(iface)) == 0)
		err_msg("arp.c", "inject_arp", __LINE__, errno);
	device.sll_family = AF_PACKET;
	memcpy(device.sll_addr, src_mac, 6);
	device.sll_halen = 6;

	set_eth(buffer, dst_mac, src_mac, ETH_P_ARP, NULL, 0);
	set_arp(buffer, src_mac, src_ip, dst_mac, dst_ip, oper);

	len = sizeof(eth_hdr) + sizeof(arp_hdr);
	for (ind = 0; ind < count; ind += 1)
		send_packet_data(sockfd, buffer, len, &device);

	if (verbose) {
		eth_hdr *eth = (eth_hdr *)buffer;
		arp_hdr *arph = (arp_hdr *)(buffer + sizeof(eth_hdr));
		print_eth(eth);
		print_arp(arph);
	}

	close_sock(sockfd);
	exit(EXIT_SUCCESS);
}
