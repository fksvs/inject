I N J E C T
======

Inject is a command-line packet crafting, injection and sniffing tool that uses RAW sockets. It supports various protocols with multiple options. It is useful for network debugging and testing network services.

Supported Protocols and Key Features
---------------------------------------------------
* ETHERNET (Source and destination MAC address, protocol and payload file)
* ARP (Source and destination MAC address, source and destination IP address, ARP operation)
* IP (Source and destination IP address, TTL, Type of Service)
* ICMP (Type and code)
* TCP (Source and destination port, TCP flag, payload file)
* UDP (Source and destination port, payload file)
* Packet sniffing (ETHERNET, ARP, IP, ICMP, TCP and UDP protocols)
* Listing network interfaces

Installation
------------
- Clone the repository from [GitHub][] or [GitLab][]: 

```console
git clone https://github.com/fksvs/inject
git clone https://gitlab.com/fksvs/inject
```

- Change directory to inject:

```console
cd inject
```

- Build the project:

```console
make
```

Usage Examples
--------------

### Ethernet

Inject can craft Ethernet packets with the following options:

- Source MAC address (`-K`)
- Destination MAC address (`-M`)
- Ethernet type (`-p`)
- Payload file (`-a`)

Ethernet packet with the payload `payload_file.txt` to the MAC address `00:11:22:33:44:55` using `wlp5s0` network interface:

```console
inject eth -i wlp5s0 -K 00:11:22:33:44:5 -a payload_file.txt
```

---

### ARP

Inject can craft ARP packets with the following options:

- Source MAC address (`-K`)
- Source IP address (`-S`)
- Destination MAC address (`-M`)
- Destination IP address (`-D`)
- ARP operation (request or reply) (`-r`)

ARP request from the MAC address `00:11:22:33:44:55` asking who has the IP address `192.168.0.1`:

```console
inject arp -i wlp5s0 -K 00:11:22:33:44:55 -S 192.168.1.50 -D 192.168.0.1 -r 1
```

---

### IP

Inject can craft IP packets with the following options:

- Source IP address (`-S`)
- Destination IP address (`-D`)
- TTL (`-T`)
- Type of service (`-0`)

IP packet from the IP address `192.168.1.40` to the IP address `192.168.1.1` with TTL of `48`:

```console
inject ip -S 192.168.1.40 -D 192.168.1.1 -T 48
```

---

### TCP

Inject can craft TCP packets with the following options:

- Source IP address (`-S`)
- Source port (`-s`)
- Destination IP address (`-D`)
- Destination port (`-d`)
- Flags (SYN, ACK, PSH, etc.) (`-f`)
- Payload file (`-a`)

TCP packet with the payload file `payload_file.txt` from the IP address `192.168.1.50` to the IP address `192.168.1.1` with the source port `4444`, the destination port `80`, and the `SYN` flag:

```console
inject tcp -S 192.168.1.50 -s 4444 -D 192.168.1.1 -d 80 -f syn -a payload_file.txt
```

---

### UDP

Inject can craft UDP packets with the following options:

- Source IP address (`-S`)
- Source port (`-s`)
- Destination IP address (`-D`)
- Destination port (`-d`)
- Payload file (`-a`)

UDP packet with the payload file `payload_file.txt` from the IP address `192.168.0.2` to the IP address `8.8.8.8` with the source port `4444` and the destination port `53`:

```console
inject udp -S 192.168.0.2 -s 4444 -D 8.8.8.8 -d 53 -a payload_file.txt
```

---

### ICMP

Inject can craft ICMP packets with the following options:

- Source IP address (`-S`)
- Destination IP address (`-D`)
- Type (`-t`)
- Code (`-C`)

ICMP packet with the IP address `192.168.0.2` to the IP address `8.8.8.8` with the ICMP type `8` (echo request) and code `0`:

```console
inject icmp -S 192.168.0.2 -D 8.8.8.8 -t 8 -C 0
```

---

### Sniff

Inject can sniff following protocols:

- Ethernet (`-e`)
- ARP (`-a`)
- IP (`-i`)
- ICMP (`-c`)
- TCP (`-t`)
- UDP (`-u`)

Sniffing only TCP and UDP packets:

```console
inject sniff -t -u
```

---

License
----------
This project is free software; you can redistribute it and/or modify it under the terms of the GPLv3 license. See [LICENSE][] for details.

[GitHub]: https://github.com/fksvs/inject
[GitLab]: https://gitlab.com/fksvs/inject
[LICENSE]: https://www.gnu.org/licenses/gpl-3.0.en.html
