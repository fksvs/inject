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
#include "arp.h"
#include "network.h"
#include "type.h"
#include "helpers.h"
#include "output.h"
#include "usage.h"

static char buffer[BUFF_SIZE];
static eth_hdr eth;
static arp_hdr arp;
static struct sockaddr_ll device;
static int sockfd;

static char *iface;
static int count = 1;
static int verbose = 0;

void build_arp(arp_hdr *arp, unsigned char *source_mac,
	       unsigned char *source_ip, unsigned char *target_mac,
	       unsigned char *target_ip, unsigned short oper)
{
	arp->htype = htons(1);
	arp->ptype = htons(ETH_P_IP);
	arp->hlen = ETHER_ADDR_LEN;
	arp->plen = 4;
	arp->oper = htons(oper);

	memcpy(arp->src_mac, source_mac, ETHER_ADDR_LEN);
	if (!source_ip) {
		struct in_addr addr = { get_address() };
		inet_pton(AF_INET, (const char *)inet_ntoa(addr), &arp->src_ip);
	} else {
		inet_pton(AF_INET, (const char *)source_ip, &arp->src_ip);
	}

	memcpy(arp->dst_mac, target_mac, ETHER_ADDR_LEN);
	inet_pton(AF_INET, (const char *)target_ip, &arp->dst_ip);
}

static void validate_arp_packet()
{
	char zero[BUFF_SIZE];
	memset(zero, 0, BUFF_SIZE);

	if (!iface) {
		err_exit("network interface not specified.");
	}

	if (!memcmp(&arp.src_ip, zero, 4)) {
		struct in_addr addr = { get_address() };
		inet_pton(AF_INET, (const char *)inet_ntoa(addr), arp.src_ip);
	}

	if (!memcmp(&arp.dst_ip, zero, 4)) {
		err_exit("destination ip address not specified.");
	}

	if (!memcmp(&arp.src_mac, zero, ETHER_ADDR_LEN)) {
		struct ifreq ifr;
		memset(&ifr, 0, sizeof(struct ifreq));

		memcpy(ifr.ifr_name, iface, strlen(iface));
		if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
			err_msg("arp.c", "validate_arp_packet", __LINE__, errno);
		}

		memcpy(&arp.src_mac, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
	}

	if (!memcmp(&arp.dst_mac, zero, ETHER_ADDR_LEN)) {
		memset(&arp.dst_mac, 0xff, ETHER_ADDR_LEN);
	}
}

static void usage()
{
	general_usage();
	arp_usage();
	printf("\n");

	exit(EXIT_FAILURE);
}

static void parse_arp(int argc, char *argv[])
{
	int opt;
	unsigned short oper;
	unsigned char src_mac[8], dst_mac[6], *src_ip, *dst_ip;

	if (argc < 3) {
		usage();
	}

	arp.htype = htons(1);
	arp.ptype = htons(ETH_P_IP);
	arp.hlen = ETHER_ADDR_LEN;
	arp.plen = 4;

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
			break;
		case 'M':
			read_mac_address(optarg, src_mac);
			memcpy(arp.src_mac, src_mac, ETHER_ADDR_LEN);
			break;
		case 'K':
			read_mac_address(optarg, dst_mac);
			memcpy(arp.dst_mac, dst_mac, ETHER_ADDR_LEN);
			break;
		case 'S':
			src_ip = (unsigned char *)optarg;
			inet_pton(AF_INET, (const char *)src_ip, arp.src_ip);
			break;
		case 'D':
			dst_ip = (unsigned char *)optarg;
			inet_pton(AF_INET, (const char *)dst_ip, arp.dst_ip);
			break;
		case 'r':
			oper = atoi(optarg);
			arp.oper = htons(oper);
		case '?':
			break;
		}
	}
}

void inject_arp(int argc, char *argv[])
{
	memset(buffer, 0, BUFF_SIZE);
	memset(&eth, 0, sizeof(eth_hdr));
	memset(&arp, 0, sizeof(arp_hdr));
	memset(&device, 0, sizeof(struct sockaddr_ll));

	if ((sockfd = init_packet_socket()) == -1) {
		exit(EXIT_FAILURE);
	}

	parse_arp(argc, argv);
	validate_arp_packet();
	build_eth(&eth, arp.dst_mac, arp.src_mac, ETH_P_ARP);
	fill_device(&device, iface, arp.src_mac);

	memcpy(buffer, &eth, sizeof(eth_hdr));
	memcpy(buffer + sizeof(eth_hdr), &arp, sizeof(arp_hdr));

	int len = sizeof(eth_hdr) + sizeof(arp_hdr);
	send_packet(sockfd, buffer, len, &device, count);

	if (verbose) {
		print_eth(&eth);
		print_arp(&arp);
	}

	close_sock(sockfd);
	exit(EXIT_SUCCESS);
}
