#ifndef ETH_H
#define ETH_H

#include "type.h"

void build_eth(eth_hdr *eth, unsigned char *dst_mac, unsigned char *src_mac,
	       unsigned short protocol);
void inject_eth(int argc, char *argv[]);

#endif
