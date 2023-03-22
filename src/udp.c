#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sockf.h"
#include "send.h"
#include "data.h"
#include "error_func.h"
#include "output.h"
#include "type.h"
#include "ip.h"
#include "udp.h"
#include "random.h"
#include "checksum.h"
#include "usage.h"

static unsigned char *src_addr = NULL, *dst_addr = NULL;
static unsigned char ttl, service = 0;
static unsigned short src_port, dst_port;
static int count = 1, verbose = 0;
static char *file_name = NULL, *iface = NULL;

static unsigned short udp_check(ip_hdr *iph, udp_hdr *udph, char *payload,
				size_t payload_size)
{
	psd_hdr psh;
	char *psd;
	size_t psd_size;
	unsigned short check;

	memcpy(psh.src, iph->src, 4);
	memcpy(psh.dst, iph->dst, 4);

	psh.placeholder = 0;
	psh.protocol = IPPROTO_UDP;
	psh.length = htons(sizeof(udp_hdr) + payload_size);

	psd_size = sizeof(psd_hdr) + sizeof(udp_hdr) + payload_size;

	psd = malloc(psd_size);
	memset(psd, 0, psd_size);

	memcpy(psd, (char *)&psh, sizeof(psd_hdr));
	memcpy(psd + sizeof(psd_hdr), udph, sizeof(udp_hdr));
	memcpy(psd + sizeof(psd_hdr) + sizeof(udp_hdr), payload, payload_size);

	check = checksum((unsigned short *)psd, psd_size);
	free(psd);

	return check;
}

void build_udp(char *buffer, char *payload, size_t payload_size,
	       unsigned short src, unsigned short dst)
{
	ip_hdr *iph = (ip_hdr *)buffer;
	udp_hdr *udph = (udp_hdr *)(buffer + sizeof(ip_hdr));
	char *ptr = (buffer + sizeof(ip_hdr) + sizeof(udp_hdr));

	strncat(ptr, payload, payload_size);
	udph->src = (src) ? htons(src) : htons(rand_port());
	udph->dst = (dst) ? htons(dst) : htons(rand_port());
	udph->length = htons(sizeof(udp_hdr) + payload_size);
	udph->check = 0;
	udph->check = udp_check(iph, udph, payload, payload_size);
}

static void validate_udp()
{
	if (!dst_addr)
		err_exit("destination address not specified.");
}

static void usage()
{
	general_usage();
	ip_usage();
	udp_usage();
	printf("\n");

	exit(EXIT_FAILURE);
}

static void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 3)
		usage();

	while ((opt = getopt(argc, argv, "i:c:vhS:D:T:o:s:d:a:")) != -1) {
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
		case 's':
			src_port = atoi(optarg);
			break;
		case 'd':
			dst_port = atoi(optarg);
			break;
		case 'a':
			file_name = optarg;
			break;
		case '?':
			break;
		}
	}
}

void inject_udp(int argc, char *argv[])
{
	char buffer[BUFF_SIZE], *payload;
	struct sockaddr_in sock_dst;
	int sockfd;
	size_t payload_size = 0;

	parser(argc, argv);

	srand(time(NULL));
	memset(buffer, 0, BUFF_SIZE);
	memset(&sock_dst, 0, sizeof(struct sockaddr_in));

	if ((sockfd = init_socket()) == -1)
		exit(EXIT_FAILURE);

	validate_udp();

	if (iface)
		bind_iface(sockfd, iface);

	sock_dst.sin_family = AF_INET;
	inet_pton(AF_INET, (const char *)dst_addr, &sock_dst.sin_addr.s_addr);
	sock_dst.sin_port = dst_port;

	if (file_name) {
		if ((payload = read_file(file_name)) == NULL)
			exit(EXIT_FAILURE);
		payload_size = strlen(payload);
	}

	build_ip(buffer, payload_size, src_addr, dst_addr, ttl, service,
		 IPPROTO_UDP);
	build_udp(buffer, payload, payload_size, src_port, dst_port);

	ip_hdr *iph = (ip_hdr *)buffer;
	send_raw(sockfd, buffer, iph->length, &sock_dst, count);

	if (verbose) {
		ip_hdr *iph = (ip_hdr *)buffer;
		udp_hdr *udph = (udp_hdr *)(buffer + sizeof(ip_hdr));
		print_ip(iph);
		print_udp(udph);
	}

	if (file_name)
		free(payload);

	close_sock(sockfd);
	exit(EXIT_SUCCESS);
}
