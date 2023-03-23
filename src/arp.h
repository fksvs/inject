#ifndef ARP
#define ARP

#include "type.h"

void build_arp(arp_hdr *arp, unsigned char *source_mac,
	       unsigned char *source_ip, unsigned char *target_mac,
	       unsigned char *target_ip, unsigned short oper);
void inject_arp(int argc, char *argv[]);

#endif
