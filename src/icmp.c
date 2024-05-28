#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "network.h"
#include "output.h"
#include "type.h"
#include "ip.h"
#include "icmp.h"
#include "usage.h"
#include "helpers.h"

static unsigned char *src_addr = NULL, *dst_addr = NULL;
static unsigned char ttl, service = 0, icmp_type, icmp_code;
static int count = 1, verbose = 0;
static char *iface = NULL;

void build_icmp(char *buffer, unsigned char type, unsigned char code,
		unsigned short seq)
{
	icmp_hdr *icmph = (icmp_hdr *)(buffer + sizeof(ip_hdr));

	icmph->type = type;
	icmph->code = code;
	icmph->seq = seq;
	icmph->id = htons(getpid());
	icmph->check = 0;
	icmph->check = checksum((unsigned short *)icmph, sizeof(icmp_hdr));
}

static void validate_icmp()
{
	if (!dst_addr) {
		err_exit("destination address not specified.");
	}
}

static void usage()
{
	general_usage();
	ip_usage();
	icmp_usage();
	fprintf(stderr, "\n");

	exit(EXIT_FAILURE);
}

static void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 3) {
		usage();
	}

	while ((opt = getopt(argc, argv, "i:c:vhS:D:T:o:t:C:")) != -1) {
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
		case 'S':
			src_addr = (unsigned char *)optarg;
			break;
		case 'D':
			dst_addr = (unsigned char *)optarg;
			break;
		case 'T':
			ttl = atoi(optarg);
			break;
		case 'o':
			service = atoi(optarg);
			break;
		case 't':
			icmp_type = atoi(optarg);
			break;
		case 'C':
			icmp_code = atoi(optarg);
			break;
		case '?':
			break;
		}
	}
}

void inject_icmp(int argc, char *argv[])
{
	char buffer[BUFF_SIZE];
	struct sockaddr_in sock_dst;
	int sockfd;

	parser(argc, argv);

	memset(buffer, 0, BUFF_SIZE);
	memset(&sock_dst, 0, sizeof(struct sockaddr_in));

	if ((sockfd = init_socket()) == -1) {
		exit(EXIT_FAILURE);
	}

	validate_icmp();

	if (iface) {
		bind_iface(sockfd, iface);
	}

	sock_dst.sin_family = AF_INET;
	inet_pton(AF_INET, (const char *)dst_addr, &sock_dst.sin_addr.s_addr);

	build_ip(buffer, 0, src_addr, dst_addr, ttl, service, IPPROTO_ICMP);
	build_icmp(buffer, icmp_type, icmp_code, 0);

	ip_hdr *iph = (ip_hdr *)buffer;
	send_raw(sockfd, buffer, iph->length, &sock_dst, count);

	if (verbose) {
		ip_hdr *iph = (ip_hdr *)buffer;
		icmp_hdr *icmph = (icmp_hdr *)(buffer + sizeof(ip_hdr));
		print_ip(iph);
		print_icmp(icmph);
	}

	close_sock(sockfd);
	exit(EXIT_SUCCESS);
}
