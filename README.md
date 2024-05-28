I N J E C T
=

Inject is a command line network tool for crafting, injecting, and sniffing a wide range of network protocols. It is designed for users who need a robust tool for detailed network protocol analysis and manipulation, whether for network troubleshooting, testing, or educational purposes.

## Table of Contents

1. [Supported Protocols and Key Features](#supported-protocols-and-key-features)
2. [Installation](#installation)
3. [Usage Examples](#usage-examples)
4. [License](#license)

Supported Protocols and Key Features
---
* **Multiple Protocol Support** : Inject handles Ethernet, ARP, IP, ICMP, TCP and UDP.
* **Network Packet Injecting** : Create and send custom network packets on specified network interfaces.
* **Payload Integration**: Load payload files into network packets.
* **Network Sniffing** : Capture network packets with customizable filtering options to focus on specific network protocols.

Installation
---
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

Ethernet packet with the payload `payload.txt` to the MAC address `00:11:22:33:44:55` using `wlp5s0` network interface:

```console
inject eth -i wlp5s0 -K 00:11:22:33:44:5 -a payload.txt
```
---
ARP request from the MAC address `00:11:22:33:44:55` asking who has the IP address `192.168.0.1`:
```console
inject arp -i wlp5s0 -K 00:11:22:33:44:55 -S 192.168.1.50 -D 192.168.0.1 -r 1
```
---
IP packet from the IP address `192.168.1.40` to the IP address `192.168.1.1` with TTL of `48`:
```console
inject ip -S 192.168.1.40 -D 192.168.1.1 -T 48
```
---
TCP packet with the payload file `payload.txt` from the IP address `192.168.1.50` to the IP address `192.168.1.1` with the source port `4444`, the destination port `80`, and the `SYN` flag:
```console
inject tcp -S 192.168.1.50 -s 4444 -D 192.168.1.1 -d 80 -f syn -a payload.txt
```
---
UDP packet with the payload file `payload.txt` from the IP address `192.168.0.2` to the IP address `8.8.8.8` with the source port `4444` and the destination port `53`:
```console
inject udp -S 192.168.0.2 -s 4444 -D 8.8.8.8 -d 53 -a payload.txt
```
---
ICMP packet with the IP address `192.168.0.2` to the IP address `8.8.8.8` with the ICMP type `8` (echo request) and code `0`:
```console
inject icmp -S 192.168.0.2 -D 8.8.8.8 -t 8 -C 0
```
---
Sniffing all supported network protocol packets:
```console
inject sniff
```
---
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
