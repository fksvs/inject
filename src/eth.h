#ifndef ETH
#define ETH

#include "type.h"

void build_eth(eth_hdr *eth, unsigned char *dst_mac, unsigned char *src_mac,
	       unsigned short protocol, char *payload, size_t payload_size);
void inject_eth(int argc, char *argv[]);

#endif
