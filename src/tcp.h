#ifndef TCP
#define TCP

void set_tcp(char *buffer, char *payload, size_t payload_size,
	     unsigned short src, unsigned short dst, unsigned char flag,
	     unsigned int seq, unsigned int ack);
void inject_tcp(int argc, char *argv[]);

#endif
