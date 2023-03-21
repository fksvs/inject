I N J E C T
===========
inject is a command line packet crafter, injector and sniffer for testing network services.

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
--------------
- Clone the source code from [GitHub][] or [GitLab][] :

        git clone https://github.com/fksvs/inject
        git clone https://gitlab.com/fksvs/inject

- Move to the project directory :

        cd inejct/

- Compile the source code :

        make

Example Usages
-----------------------
* ETHERNET packet

        ./inject eth -i wlp5s0 -M AA:BB:CC:DD:EE:FF -K FF:EE:DD:CC:BB:AA

* ARP request

        ./inject arp -i wlp5s0 -S 192.168.1.50 -D 192.168.1.1 -r 1

* IP packet with TTL of 20

        ./inject ip -S 192.168.1.50 -D 192.168.1.1 -T 20

* ICMP ping

        ./inject icmp -S 192.168.1.50 -D 192.168.1.1 -t 8

* TCP packet with SYN-ACK flag

        ./inject tcp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 80 -f syn -f ack

* TCP packet with PSH flag and payload

        ./inject tcp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 4000 -f psh -a payload_file.txt

* UDP packet with payload

        ./inject udp -S 192.168.1.50 -D 192.168.1.1 -s 5000 -d 80 -a payload_file.txt

License
----------
This library is free software; you can redistribute it and/or modify it under the terms of the GPLv3 license.
See [LICENSE][] for details.

[GitHub]: https://github.com/fksvs/inject
[GitLab]: https://gitlab.com/fksvs/inject
[LICENSE]: https://www.gnu.org/licenses/gpl-3.0.en.html
