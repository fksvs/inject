#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "error_func.h"
#include "eth.h"
#include "arp.h"
#include "ip.h"
#include "icmp.h"
#include "tcp.h"
#include "udp.h"
#include "get_addr.h"
#include "listen.h"
#include "usage.h"

void parser(int argc, char *argv[])
{
	int opt;

	if (argc < 2) {
		main_usage();
		exit(EXIT_FAILURE);
	}

	if (!strcmp(argv[1], "sniff"))
		sniff_network(argc, argv);
	if (!strcmp(argv[1], "eth"))
		inject_eth(argc, argv);
	if (!strcmp(argv[1], "arp"))
		inject_arp(argc, argv);
	if (!strcmp(argv[1], "ip"))
		inject_ip(argc, argv);
	if (!strcmp(argv[1], "icmp"))
		inject_icmp(argc, argv);
	if (!strcmp(argv[1], "tcp"))
		inject_tcp(argc, argv);
	if (!strcmp(argv[1], "udp"))
		inject_udp(argc, argv);

	while ((opt = getopt(argc, argv, "gh")) != -1) {
		switch (opt) {
		case 'g':
			list_interfaces();
			break;
		case 'h':
			main_usage();
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char *argv[])
{
	if (getuid())
		err_exit("permission denied.");
	parser(argc, argv);

	return 0;
}
