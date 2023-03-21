#ifndef UDP
#define UDP

void build_udp(char *buffer, char *payload, size_t payload_size,
	       unsigned short src, unsigned short dst);
void inject_udp(int argc, char *argv[]);

#endif
