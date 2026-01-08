#ifndef TYPE_H
#define TYPE_H

/* Buffer size for packet operations */
#define BUFF_SIZE 65535

/* Default TTL for IP packets */
#define DEFAULT_TTL 248

/* Protocol flags for sniffing */
#define ETH_FLAG  1
#define ARP_FLAG  2
#define ICMP_FLAG 4
#define IP_FLAG   8
#define TCP_FLAG  16
#define UDP_FLAG  32
#define ALL       63

/* Ethernet header structure */
typedef struct {
	unsigned char dst[6];
	unsigned char src[6];
	unsigned short protocol;
} eth_hdr;

/* ARP header structure */
typedef struct {
	unsigned short htype;
	unsigned short ptype;
	unsigned char hlen;
	unsigned char plen;
	unsigned short oper;
	unsigned char src_mac[6];
	unsigned char src_ip[4];
	unsigned char dst_mac[6];
	unsigned char dst_ip[4];
} arp_hdr;

/* IP header structure */
typedef struct {
	unsigned char ver_ihl;
	unsigned char service;
	unsigned short length;
	unsigned short ident;
	unsigned short frag;
	unsigned char ttl;
	unsigned char protocol;
	unsigned short check;
	unsigned char src[4];
	unsigned char dst[4];
} ip_hdr;

/* ICMP header structure */
typedef struct {
	unsigned char type;
	unsigned char code;
	unsigned short check;
	unsigned short id;
	unsigned short seq;
} icmp_hdr;

/* TCP header structure */
typedef struct {
	unsigned short src;
	unsigned short dst;
	unsigned int seq;
	unsigned int ack;
	unsigned char len_res;
	unsigned char flag;
	unsigned short win;
	unsigned short check;
	unsigned short urgp;
} tcp_hdr;

/* UDP header structure */
typedef struct {
	unsigned short src;
	unsigned short dst;
	unsigned short length;
	unsigned short check;
} udp_hdr;

/* Pseudo header for checksum calculation */
typedef struct {
	unsigned char src[4];
	unsigned char dst[4];
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short length;
} psd_hdr;

#endif /* TYPE_H */
