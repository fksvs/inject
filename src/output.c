#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "output.h"
#include "type.h"

void print_eth(eth_hdr *eth)
{
	int ind;

	printf("ethernet | ");

	for (ind = 0; ind < 5; ind += 1)
		printf("%.2x:", eth->src[ind]);
	printf("%.2x", eth->src[5]);

	printf(" --> ");

	for (ind = 0; ind < 5; ind += 1)
		printf("%.2x:", eth->dst[ind]);
	printf("%.2x ", eth->dst[5]);

	printf("%d\n", eth->protocol);
}

void print_arp(arp_hdr *arph)
{
	int ind;

	printf("arp      | ");
	for (ind = 0; ind < 5; ind += 1)
		printf("%.2x:", arph->src_mac[ind]);
	printf("%.2x", arph->src_mac[5]);
	printf(" --> ");
	for (ind = 0; ind < 5; ind += 1)
		printf("%.2x:", arph->dst_mac[ind]);
	printf("%.2x ", arph->dst_mac[5]);

	printf("%u.%u.%u.%u", arph->src_ip[0], arph->src_ip[1], arph->src_ip[2],
	       arph->src_ip[3]);
	printf(" --> ");
	printf("%u.%u.%u.%u", arph->dst_ip[0], arph->dst_ip[1], arph->dst_ip[2],
	       arph->dst_ip[3]);

	if (arph->oper == htons(1))
		printf(" request\n");
	else if (arph->oper == htons(2))
		printf(" reply\n");
	else
		printf(" %d\n", arph->oper);
}

void print_ip(ip_hdr *iph)
{
	printf("ip       | ");
	printf("%u.%u.%u.%u", iph->src[0], iph->src[1], iph->src[2],
	       iph->src[3]);
	printf(" --> ");
	printf("%u.%u.%u.%u ", iph->dst[0], iph->dst[1], iph->dst[2],
	       iph->dst[3]);
	printf("proto:%d ", iph->protocol);
	printf("tos:%d ", iph->service);
	printf("ttl:%d ", iph->ttl);
	printf("id:%d ", iph->ident);
	printf("check:%x ", iph->check);
	printf("len:%d\n", iph->length);
}

void print_icmp(icmp_hdr *icmph)
{
	printf("icmp     | ");
	printf("type:%d (", icmph->type);
	if (icmph->type == 0)
		printf("echo reply");
	if (icmph->type == 3)
		printf("destination unreachable");
	if (icmph->type == 4)
		printf("source quench");
	if (icmph->type == 5)
		printf("redirect");
	if (icmph->type == 8)
		printf("echo request");
	if (icmph->type == 9)
		printf("router advertisement");
	if (icmph->type == 10)
		printf("router selection");
	if (icmph->type == 11)
		printf("time exceeded");
	if (icmph->type == 12)
		printf("parameter problem");
	if (icmph->type == 13)
		printf("timestamp");
	if (icmph->type == 14)
		printf("timestamp reply");
	if (icmph->type == 15)
		printf("information request");
	if (icmph->type == 16)
		printf("information reply");
	if (icmph->type == 17)
		printf("address mask request");
	if (icmph->type == 18)
		printf("address mask reply");
	if (icmph->type == 30)
		printf("traceroute");
	printf(") ");
	printf("code:%d ", icmph->code);
	printf("id:%d ", icmph->id);
	printf("seq:%d ", icmph->seq);
	printf("check:%x\n", icmph->check);
}

void print_tcp(tcp_hdr *tcph)
{
	printf("tcp      | ");
	printf("%d --> %d ", htons(tcph->src), htons(tcph->dst));
	printf("seq:%d ", tcph->seq);
	printf("ack:%d ", tcph->ack);
	printf("flag:");
	if ((tcph->flag | 1) == tcph->flag)
		printf("fin,");
	if ((tcph->flag | 2) == tcph->flag)
		printf("syn,");
	if ((tcph->flag | 4) == tcph->flag)
		printf("rst,");
	if ((tcph->flag | 8) == tcph->flag)
		printf("psh,");
	if ((tcph->flag | 16) == tcph->flag)
		printf("ack,");
	if ((tcph->flag | 32) == tcph->flag)
		printf("urg,");
	printf(" check:%x\n", tcph->check);
}

void print_udp(udp_hdr *udph)
{
	printf("udp      | ");
	printf("%d --> %d ", htons(udph->src), htons(udph->dst));
	printf("check:%x\n", udph->check);
}
