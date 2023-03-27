#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>

#include "helpers.h"

unsigned short checksum(unsigned short *ptr, int nbytes)
{
        unsigned short oddbyte = 0;
        register long sum = 0;
        register short answer = 0;

        while (nbytes > 1) {
                sum += *ptr++;
                nbytes -= 2;
        }
        if (nbytes == 1) {
                *((unsigned char *)&oddbyte) = *(unsigned char *)ptr;
                sum += oddbyte;
        }
        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);
        answer = (unsigned short)~sum;

        return answer;
}

void err_msg(char *file_name, char *func, int line, int err)
{
        fprintf(stderr, "[ERROR] [%s/%s/%d] : %s\n", file_name, func, line,
                strerror(err));
}

void err_exit(char *msg)
{
        fprintf(stderr, "[ERROR] %s\n", msg);
        exit(EXIT_FAILURE);
}

void fill_device(struct sockaddr_ll *device, char *iface, unsigned char *mac)
{
        if ((device->sll_ifindex = if_nametoindex(iface)) == 0)
                err_msg("eth.c", "fill_device", __LINE__, errno);

        device->sll_family = AF_PACKET;
        memcpy(device->sll_addr, mac, ETHER_ADDR_LEN);
        device->sll_halen = ETHER_ADDR_LEN;
}

unsigned int get_address()
{
        struct ifaddrs *addr, *temp;
        struct sockaddr_in *ip_addr;
        unsigned int ip;

        if (getifaddrs(&addr) == -1) {
                err_msg("get_addr.c", "get_address", __LINE__, errno);
                return -1;
        }

        for (temp = addr; temp != NULL; temp = temp->ifa_next) {
                if (temp->ifa_addr == NULL)
                        continue;

                if (temp->ifa_addr->sa_family == AF_INET &&
                    strcmp(temp->ifa_name, "lo")) {
                        ip_addr = (struct sockaddr_in *)temp->ifa_addr;
                        ip = ip_addr->sin_addr.s_addr;

                        break;
                }
        }

        freeifaddrs(addr);

        return ip;
}

int list_interfaces()
{
        struct ifaddrs *addr, *temp;
        struct sockaddr_in *ip_addr;
        int ind = 0;

        if (getifaddrs(&addr) == -1) {
                err_msg("get_addr.c", "list_interfaces", __LINE__, errno);
                return -1;
        }

        for (temp = addr; temp != NULL; temp = temp->ifa_next) {
                if (temp->ifa_addr == NULL)
                        continue;

                ip_addr = (struct sockaddr_in *)temp->ifa_addr;

                if (temp->ifa_addr->sa_family == AF_INET)
                        printf("[%d] [%s] -> [%s]\n", ind, temp->ifa_name,
                               inet_ntoa(ip_addr->sin_addr));
                ind += 1;
        }

        freeifaddrs(addr);

        return 0;
}

void read_mac_address(char *data, char *mac)
{
        sscanf(data, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx:", &mac[0], &mac[1],
               &mac[2], &mac[3], &mac[4], &mac[5]);
}

unsigned int rand_addr()
{
        return rand();
}

unsigned short rand_port()
{
        return rand() % 65535;
}
