#include <stdio.h>

void main_usage()
{
	fprintf(stderr ,"\n usage : ./inject [protocol/mode] [options]\n\n protocols :\n\
\teth : eth packets\n\
\tarp : arp packets\n\
\tip : ip packets\n\
\ticmp : icmp packets\n\
\ttcp : tcp packets\n\
\tudp : udp packets\n\n\
 modes : \n\
\tsniff : sniff network\n\n\
 options :\n\
\t-g : list avaliable interfaces\n\
\t-h : this help message\n\n");
}

void general_usage()
{
	fprintf(stderr, "\n general options :\n\n\
\t-i [interface] : network interface\n\
\t-c [count] : number of packets to send\n\
\t-v : verbose\n\
\t-h : this help message\n");
}

void sniff_usage()
{
	fprintf(stderr, "\n sniff options :\n\n\
\t-e : ethernet\n\
\t-a : arp\n\
\t-i : ip\n\
\t-c : icmp\n\
\t-t : tcp\n\
\t-u : udp\n");
}

void eth_usage()
{
	fprintf(stderr, "\n ETH options :\n\n\
\t-M [mac address] : source mac address\n\
\t-K [mac address] : destination mac address\n\
\t-p [protocol] : protocol\n\
\t-a [file name] : payload file\n");
}

void arp_usage()
{
	fprintf(stderr, "\n ARP options :\n\n\
\t-M [mac address] : source mac address\n\
\t-K [mac address] : destination mac address\n\
\t-S [ip address] : source ip address\n\
\t-D [ip address] : destination ip address\n\
\t-r [operation] : ARP operation\n");
}

void ip_usage()
{
	fprintf(stderr, "\n IP options :\n\n\
\t-S [address] : source address\n\
\t-D [address] : destination address\n\
\t-T [ttl] : ttl\n\
\t-o [service] : type of service\n");
}

void icmp_usage()
{
	fprintf(stderr, "\n ICMP options :\n\n\
\t-t [type] : icmp type\n\
\t-C [code] : icmp code\n");
}

void tcp_usage()
{
	fprintf(stderr, "\n TCP options :\n\n\
\t-s [port] : source port\n\
\t-d [port] : destination port\n\
\t-f [flag] : tcp flag (syn, ack, psh, fin, rst, urg)\n\
\t-a [file] : payload file\n");
}

void udp_usage()
{
	fprintf(stderr, "\n UDP options :\n\n\
\t-s [port] : source port\n\
\t-d [port] : destination port\n\
\t-a [file] : payload file\n");
}
