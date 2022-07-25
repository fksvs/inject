#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <ifaddrs.h>

#include "error_func.h"
#include "get_addr.h"

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
