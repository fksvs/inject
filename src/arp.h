#ifndef ARP
#define ARP

void build_arp(char *buffer, unsigned char *source_mac,
	       unsigned char *source_ip, unsigned char *target_mac,
	       unsigned char *target_ip, unsigned short oper);
void inject_arp(int argc, char *argv[]);

#endif
