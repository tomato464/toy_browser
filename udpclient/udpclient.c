#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include"../lib/lib.h"


int main(int argc, char *argv[])
{
	if(argc < 4){
		printf("Usage: %s ip_address port message\n", argv[0]);
		exit(1);
	}

	//create socket
	int soc;
	soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(soc < 0){
		printf("error at making socket.\n");
		exit(1);
	}

	//create ip addr
	in_addr_t ip_addr = GetIPaddrFromString(argv[1]);

	//create port
	unsigned short  port = htons(StrToNum16(argv[2], NULL));

	//send packet
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip_addr;
	addr.sin_port = port;
	char *buf = argv[3];
	int n = sendto(soc, buf, strlen(buf), 0,(struct sockaddr *)&addr, sizeof(addr));

	printf("send size : %d \n", n);

	close(soc);

	return 0;
}
