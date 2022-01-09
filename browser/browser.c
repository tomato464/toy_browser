#include"../lib/lib.h"
#include"render.h"
#include<string.h>
#include<stdio.h>

#define SIZE_URL 100
#define SIZE_SCHEME 10
#define SIZE_HOST 100
#define SIZE_PATH 100

#define SIZE_REQUEST 1000
#define SIZE_RESPONSE 10000

struct ParseUrl{
	char *url;
	char *scheme;
	char *host;
	uint16_t port;
	char *path;
};

bool tcp = false;

struct ParseUrl parseurl;

int ParseArgs(int argc, char *argv[])
//for test, we return status
//-1 => error
//0 => continue to browser
//1 => test parse url
//2 => test tokenize html
{
	int ret = 0;
	//should use strcpy
	strcpy(parseurl.url, "http://example.com/path");
	while(argc > 0){
		if(!strncmp(argv[0], "--url_test", 10)){
			argc -= 1;
			argv += 1;
			ret = 1;
			continue;
		}
		if(!strncmp(argv[0], "--url", 5) || strncmp(argv[0], "-u", 2)){
			strcpy(parseurl.url, argv[1]);
			argc -= 2;
			argv += 2;
			continue;	
		}
		if(!strncmp(argv[0], "--tcp", 5) || strncmp(argv[0], "-t", 2)){
			tcp = true;
			argc -= 1;
			argv += 1;
			continue;	
		}
		return -1;
	}
	return ret;
}

int ParseUrl()
// url = scheme :// host [':' port]  /path
{
	char *url = (char *) parseurl.url;

	if(strncmp(url, "http://", 7)){
		printf("only support http scheme\n");
		return -1;
	}

	url += 7;
	strcpy(parseurl.scheme, "http");

	int host_len = 0;
	while(*url != ':' && *url != '/' && *url){
		*(parseurl.host + host_len) = *url;
		host_len++;
		url++;
	}

	*(parseurl.host + host_len) = '\0';


	if(*url == ':'){
	//port
		char *tmp_port;
		tmp_port = (char *)malloc(10);
		url++;
		char *start;
		start = tmp_port;
		while(*url != '/' && *url){
			*tmp_port = *url;
			tmp_port++;
			url++;
		}
		*tmp_port = '\0';
		parseurl.port = StrToNum16(start, NULL);
		free(start);
	}else{
		parseurl.port = 80;
	}

	if(*url == '/'){
		int path_len = 0;
		while(*url){
			parseurl.path[path_len] = *url;
			path_len++;
			url++;
		}
	}else{
		strcpy(parseurl.path, "/");
	}

	if(!parseurl.host[0]){
		printf("error at host\n");
		return -1;
	}

	if(!parseurl.path[0]){
		printf("error at path\n");
		return -1;
	}

	return 0;
}

void PrintParseUrl()
{
	printf("Scheme : %s\n", parseurl.scheme);
	printf("Host : %s\n", parseurl.host);
	printf("Port : %d\n", parseurl.port);
	printf("Path : %s\n", parseurl.path);
}

void MakeRequest(char *request)
{
	strcpy(request, "GET ");
	strcat(request, parseurl.path);
	strcat(request, " HTTP/1.1\n");
}

void MakeHeader(char *request)
{
	strcat(request, "HOST: ");
	strcat(request, parseurl.host);
	strcat(request, "\n");
}

void Crlf(char *request)
{
	strcat(request, "\n");
}

void MakeBody(char *request)
{

}

void BuildRequest(char *request)
{
	MakeRequest(request);
	MakeHeader(request);
	Crlf(request);
	MakeBody(request);
}

void GetResponse(char *request, char *response)
{
	in_addr_t ip = GetIPaddrFromString(parseurl.host);

	uint16_t port = 8888;

	if(parseurl.port){
		port = parseurl.port;
	}

	int soc;

	if(tcp){
		soc = socket(AF_INET, SOCK_STREAM, 0);
	}else{
		soc = socket(AF_INET, SOCK_DGRAM, 0);
	}

	if(soc < 0){
		printf("error at sockets()\n");
		exit(1);
	}

	struct sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = ip;
	client_addr.sin_port = htons(port);
	socklen_t addrlen = sizeof(client_addr);

	int n;
	if(tcp){
		n = connect(soc, (struct sockaddr *) &client_addr, addrlen);
		if(n < 0){
			printf("error at connext()\n");
			exit(1);
		}
	}
	//send
	n = sendto(soc, request, strlen(request), 0, (struct sockaddr *) &client_addr, addrlen);
	if(n < 0){
		printf("error at sendto()\n");
		exit(1);
	}

	n = recvfrom(soc, response, SIZE_RESPONSE, 0, (struct sockaddr *) &client_addr, 
					&addrlen);
	if(n < 0){
		printf("error at recvfrom()\n");
		exit(0);
	}
	
	close(soc);
}

void GetHtmlFromResponse(char *response, char *html)
{
	while(*response){
		if(*response == '<'){
			break;
		}
		response++;
	}

	strcpy(html, response);

}

int main(int argc, char *argv[])
{
	int n;

	parseurl.url = (char *) malloc(SIZE_URL);
	parseurl.scheme = (char *) malloc(SIZE_SCHEME);
	parseurl.host = (char *) malloc(SIZE_HOST);
	parseurl.path = (char *) malloc(SIZE_PATH);

	n = ParseArgs(argc - 1, argv + 1);

	if(n < 0){
		printf("Usage : %s --url url\n		--url_test\n--tcp -t\n", argv[0]);
		exit(1);
	}

	if(n == 1){//test for parsing url
		n = ParseUrl();
		if(n < 0){
			printf("error at Parse Url\n");
			exit(1);
		}
		PrintParseUrl();
		return 0;
	}

	if(n == 3){//test for tokenizing http
		return 0;
	}

	char *request;
	char *response;

	ParseUrl();

	request = (char *) malloc(SIZE_REQUEST);
	response = (char *) malloc(SIZE_RESPONSE);

	BuildRequest(request);

	GetResponse(request, response);

	char *html;
	html = (char *) malloc(strlen(response));
	GetHtmlFromResponse(response, html);
	free(request);
	free(response);
	free(parseurl.url);
	free(parseurl.scheme);
	free(parseurl.host);
	free(parseurl.path);

	printf("%s\n", html);

	Render(html);

	return 0;
}
