#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../lib/lib.h"

struct __attribute__((packed)) ICMP_HEADER{
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
	uint16_t identi;
	uint16_t seq;
};

uint16_t CALC_CHECKSUM(void *buf, const int start, const int end)
{
//calculate icmp checksum
	uint8_t *p = buf;
	int sum = 0;
	for(int i = start; i < end; i += 2){
		sum += (uint16_t)((p[i] << 8) | p[i + 1]);
	}
	while(sum >> 16){
		sum = (sum & 0xffff) + (sum >> 16);
	}

	return ~(uint16_t) sum;
}

int main(int argc, char *argv[])
{
	int soc;

	if(argc == 1){
		printf("Usage: %s ip_address\n", argv[0]);
		exit(1);
	}

	//create ip addr
	in_addr_t ip_addr = GetIPaddrFromString(argv[1]);

	printf("ping to ...");
	PrintIPaddr(ip_addr);
	printf("\n");
	//create socket
	soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if(soc < 0){
		printf("error at making socket.\n");
		exit(1);
	}
	/*
	int res;
	uint8_t test[] = {0x8, 0,0, 0x1,  0, 0xe, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68};
	res = CALC_CHECKSUM(&test, 0, sizeof(test));
	printf("checksum : %x\n", res);
	*/
	//create ICMP_HEADER
	struct ICMP_HEADER icmp;
	memset(&icmp, 0, sizeof(icmp));
	icmp.type = 8;
	icmp.checksum = CALC_CHECKSUM(&icmp, 0, sizeof(icmp));

	//send packet
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip_addr;
	int n = sendto(soc, &icmp, sizeof(icmp), 0,(struct sockaddr *)&addr, sizeof(addr));
	if(n < 0){
		printf("error at sendto\n");
		exit(1);
	}
	
	//recv msg
	int msg_len;
	uint8_t buf[256];
	socklen_t addr_len;
	msg_len = recvfrom(soc, buf, sizeof(buf), 0, (struct sockaddr *)&addr,  &addr_len);
	if(msg_len < 0){
		printf("error at recvfrom\n");
		exit(1);
	}
	

	printf("recv msg %d bytes\n", msg_len);

	printf("contents of recv_msg : ");
	for(int i = 0; i < msg_len; i++){
		printf("%x ", buf[i]);
	}
	printf("\n");

	struct ICMP_HEADER *recv_icmp = (struct ICMP_HEADER *)buf;
	printf("ICMP packet received from : ");
	PrintIPaddr(addr.sin_addr.s_addr);
	printf(" ICMP type = %d\n", recv_icmp->type);

	close(soc);

	return 0;
}
