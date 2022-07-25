#ifndef OUTPUT
#define OUTPUT

#include "type.h"

void print_eth(eth_hdr *eth);
void print_arp(arp_hdr *arph);
void print_ip(ip_hdr *iph);
void print_icmp(icmp_hdr *icmph);
void print_tcp(tcp_hdr *tcph);
void print_udp(udp_hdr *udph);

#endif
