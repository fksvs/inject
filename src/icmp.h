#ifndef ICMP
#define ICMP

void build_icmp(char *buffer, unsigned char type, unsigned char code,
		unsigned short seq);
void inject_icmp(int argc, char *argv[]);

#endif
