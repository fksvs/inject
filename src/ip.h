#ifndef IP
#define IP

#include <sys/types.h>

void build_ip(char *buffer, size_t payload_size, unsigned char *src,
	      unsigned char *dst, unsigned char ttl, unsigned char service,
	      unsigned char protocol);
void inject_ip(int argc, char *argv[]);

#endif
