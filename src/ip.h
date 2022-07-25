#ifndef IP
#define IP

#include <sys/types.h>

void set_ip(char *buffer, size_t payload_size, unsigned char src[4],
	    unsigned char dst[4], unsigned char ttl, unsigned char service,
	    unsigned char protocol);
void inject_ip(int argc, char *argv[]);

#endif
