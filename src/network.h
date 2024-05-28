#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

int init_socket();
int init_packet_socket();
int bind_iface(int sockfd, char *iface);
void close_sock(int sockfd);
void send_raw(int sockfd, char *buffer, size_t len,
              struct sockaddr_in *dest_addr, int count);
void send_packet(int sockfd, char *buffer, size_t len,
                 struct sockaddr_ll *dest_addr, int count);


#endif
