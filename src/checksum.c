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
