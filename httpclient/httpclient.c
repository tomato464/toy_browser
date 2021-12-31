#include"../lib/lib.h"
#include<stdlib.h>
#include<string.h>

#define SIZE_REQUEST 1000
#define SIZE_RESPONSE 10000
char *host;
char *path;
char *cmd_ip;
char *cmd_port;
bool tcp;

bool ParseArgv(int argc, char *argv[])
{
	host = "";
	path = "/";
	cmd_ip = "127.0.0.1";
	cmd_port = "8888";
	tcp = false;
	while(argc > 0){
		if(!strncmp(argv[0], "--host", 6) || !strncmp(argv[0], "-h", 2)){
			host = argv[1];
			argv += 2;
			argc -= 2;
			continue;
		}
		if(!strncmp(argv[0], "--path", 6) || !strncmp(argv[0], "-p", 2)){
			path = argv[1];
			argv += 2;
			argc -= 2;
			continue;
		}
		if(!strncmp(argv[0], "--ip", 4) || !strncmp(argv[0], "-i", 2)){
			cmd_ip = argv[1];
			argv += 2;
			argc -= 2;
			continue;
		}
		if(!strncmp(argv[0], "--port", 6) || !strncmp(argv[0], "-P", 2)){
			cmd_port = argv[1];
			argv += 2;
			argc -= 2;
			continue;
		}
		if(!strncmp(argv[0], "--tcp", 5) || !strncmp(argv[0], "-t", 2)){
			tcp = true;
			argv += 1;
			argc -= 1;
			continue;
		}
		return false;
	}

	return true;
}

void MakeReqLine(char *request)
{
	strcpy(request, "GET ");
	strcat(request, path);
	strcat(request, " HTTP/1.1\n");
}

void MakeHeader(char *request)
{
	strcat(request, "Host: ");
	strcat(request, host);
	strcat(request, "\n");
}

void Crlf(char *request)
{
	strcat(request, "\n");
}

void MakeBody(char *request)
{
}

bool SendRequest(char *request)
{
	//convert char to number
	in_addr_t ip_addr = GetIPaddrFromString(cmd_ip);
	uint16_t port = StrToNum16(cmd_port, NULL);

	int soc;
	//create socket
	if(tcp){
		soc = socket(AF_INET, SOCK_STREAM, 0);
	}else{
		//udp
		soc = socket(AF_INET, SOCK_DGRAM, 0);
	}
	if(soc < 0){
		printf("error at socket()\n");
		return false;
	}
	struct sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = ip_addr;
	client_addr.sin_port = htons(port);

	// in tcp, we whould call connect
	int n;
	if(tcp){
			n = connect(soc, (struct sockaddr *) &client_addr, sizeof(client_addr));
			if(n < 0){
				printf("error at connect()\n");
				return false;
			}
	}

	//send scoekt
	printf("udp send to : ");
	PrintIPaddr(ip_addr);
	printf("\n");
	printf("port : %d\n", port);
	n = sendto(soc, request, strlen(request), 0, (struct sockaddr *) &client_addr, sizeof(client_addr));

	printf("send message %d bytes\n", n);
	printf("contens :\n%s\n", request);

	//recvive message
	char response[SIZE_RESPONSE];
	socklen_t addr_len;
	ssize_t recvlen = recvfrom(soc, response, sizeof(response), 0, (struct sockaddr *) &client_addr, &addr_len);
	if(recvlen < 0){
		printf("failed to recvive message\n");
		return false;
	}
	printf("got response %ld bytes:\n", recvlen);
	printf("%s\n", response);

	close(soc);
	return true;
}

int main(int argc, char *argv[])
{
	if(!ParseArgv(argc - 1, argv + 1)){
		printf("Usage : %s {options} : \n", argv[0]);
		printf("	--port,-P for port (default 8888)\n");
		printf("	--host,-h for host (default empty)\n");
		printf("	--ip,-i for ip address (default 127.0.0.1\n");
		printf("	--path,-p for path (default / )\n");
		return 1;
	}


	//create buf for request
	char *request;
	request = (char *)malloc(SIZE_REQUEST);

	//create request
	//start
	MakeReqLine(request);//only GET , path = /
	//header
	MakeHeader(request);//host example.cmd
	Crlf(request);
	//body (nothing)
	MakeBody(request);

	//send request
	bool n = SendRequest(request);
	if(!n){
		printf("error at send request\n");
		return 1;
	}
	free(request);
	return 0;
}
