#include"../lib/lib.h"
#include<sys/socket.h>

int main(int argc, char *argv[])
{
//program port
	if(argc < 2){
		printf("Usage : %s port\n", argv[0]);
		exit(1);
	}

	//create socket
	int soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(soc < 0){
		printf("error at socket()\n");
		exit(1);
	}

	//bind
	int n;
	struct sockaddr_in server_addr;
	uint16_t port = StrToNum16(argv[1], NULL);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	n = bind(soc, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(n < 0){
		printf("error at bind()\n");
		exit(1);
	}

	printf("listening port : %d \n", port);


	char buf[2048];
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int received_size;
	//receive loop
	while(1){
		received_size = recvfrom(soc, (char *)buf, sizeof(buf), 0, 
			(struct sockaddr *) &client_addr, &client_addr_len);
		printf("received size : %d\n", received_size);
		printf("received message : ");
		PrintNStr((char *) buf, received_size);
		printf("\n");
	}

	close(soc);
	return 0;
}
