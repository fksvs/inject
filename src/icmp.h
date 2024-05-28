#ifndef ICMP_H
#define ICMP_H

void build_icmp(char *buffer, unsigned char type, unsigned char code,
		unsigned short seq);
void inject_icmp(int argc, char *argv[]);

#endif
