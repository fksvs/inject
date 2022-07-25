#ifndef ETH
#define ETH

void set_eth(char *buffer, unsigned char *dst, unsigned char *src,
	     unsigned short proto, char *payload, size_t payload_size);
void inject_eth(int argc, char *argv[]);

#endif
