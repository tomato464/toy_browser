#include"../lib/lib.h"
#include<string.h>

#define SIZE_BACKLOG 3
#define SIZE_REQUEST 1000
#define SIZE_RESPONSE 10000

bool tcp;
char *cmd_port;

uint16_t port;

bool ParseArgs(int argc, char *argv[])
{
	tcp = false;
	cmd_port = "8888";

	while(argc > 0){
		if(!strncmp(argv[0], "--tcp", 5) | !strncmp(argv[0], "-t", 2)){
			tcp = true;
			argc -= 1;
			argv += 1;
			continue;
		}
		if(!strncmp(argv[0], "--port", 6) | !strncmp(argv[0], "-p", 2)){
			cmd_port = argv[1];
			argc -= 2;
			argv += 2;
			continue;
		}

		return false;
	}
	
	return true;
}

void StatusLine(char *response, int status)
{
	switch(status){
	case 200 ://succeed
		strcpy(response, "HTTP/1.1 200 OK\n");
		break;

	case 404 :
		strcpy(response, "HTTP1.1 404 Not Found\n");
		break;

	default ://
		strcpy(response, "HTTP/1.1 500 Internal Server Error\n");
		break;
	}
}

void Headers(char *response)
{
	strcat(response, "Content-Type: text/html; charset=UTF-8\n");
}

void Crlf(char *response)
{
	strcat(response, "\n");
}

void Body(char *response, char *contents)
{
	strcat(response, contents);
}

void MakeResponse(char *response, int status, char *contents)
//200 => ok | 500 => error
// see contents of http response
{
	StatusLine(response, status);
	Headers(response);
	Crlf(response);
	Body(response, contents);
}

void Route(char *response, char *path)
{
	if(!strcmp(path, "/") || !strcmp(path, "/index.html")){
		//make contents
		char *contents = 
		"<html>\n"
        "  <body>\n"
        "    <h1>Hello World</h1>\n"
        "    <div>\n"
        "      <p>This is a sample paragraph.</p>\n"
        "      <ul>\n"
        "        <li>List 1</li>\n"
        "        <li>List 2</li>\n"
        "        <li>List 3</li>\n"
        "      </ul>\n"
        "    </div>\n"
        "  </body>\n"
        "</html>\n";

		MakeResponse(response, 200, contents);
		return;
	}

	if(!(strcmp(path, "/page1.html"))){
		char *contents =
	     "<html>\n"
         "  <body>\n"
         "    <h1>Example Page 1</h1>\n"
         "    <div>\n"
         "      <p>This is a sample paragraph.</p>\n"
         "      <ul>\n"
         "        <li>List 1</li>\n"
         "        <li>List 2</li>\n"
         "      </ul>\n"
         "      <ul>\n"
         "        <li>List 3</li>\n"
         "        <li>List 4</li>\n"
         "      </ul>\n"
         "    </div>\n"
         "  </body>\n"
         "</html>\n";

		MakeResponse(response, 200, contents);
		return;
	}

	if(!strcmp(path, "/page2.html")){
		char *contents =
         "<html>\n"
         "  <body>\n"
         "    <h1>Example Page 2</h1>\n"
         "    <div>\n"
         "      <p>This is the first paragraph.</p>\n"
         "    </div>\n"
         "    <ul>\n"
         "      <li>List 1</li>\n"
         "      <li>List 2</li>\n"
         "    </ul>\n"
         "    <div>\n"
         "      <p>This is the second paragraph.</p>\n"
         "    </div>\n"
         "  </body>\n"
         "</html>\n";
		MakeResponse(response, 200, contents);
		return;
	}

	char *contents = 
       "<html>\n"
       "  <body>\n"
       "    <h1>Error!</h1>\n"
       "    <p>Page is not found.</p>\n"
       " </body>\n"
       "</html>\n";
	MakeResponse(response, 404, contents);
}

void StartServer(void)
{
	int soc;
	//create socket
	if(tcp){
		soc = socket(AF_INET, SOCK_STREAM, 0);
	}else{
		soc = socket(AF_INET, SOCK_DGRAM, 0);
	}

	if(soc < 0){
		printf("error at socket()\n");
		exit(1);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	socklen_t addrlen = sizeof(server_addr);

	int n;
	n = bind(soc, (struct sockaddr *) &server_addr, addrlen);
	if(n < 0){
		printf("error at bind\n");
		exit(1);
	}

	if(tcp){
		n = listen(soc, SIZE_BACKLOG);
		if(n < 0){
			printf("error at listen\n");
			exit(1);
		}
	}
	//loop for request
	//once got request => read request for method path host
	//route path
	//

	while(1){
		//received request
		char request[SIZE_REQUEST];
		int received_size;
		int accepted_soc;
		if(tcp){
			//accept => read => sendto => close fd
			accepted_soc = accept(soc, (struct sockaddr *) &server_addr, &addrlen);
			if(accepted_soc < 0){
				printf("error at accept\n");
				exit(1);
			}
			received_size = read(accepted_soc, request, sizeof(request));
			if(received_size < 0){
				printf("error at read\n");
				exit(1);
			}
		}else{
			received_size = recvfrom(soc, request, sizeof(request), 0,
				 (struct sockaddr *) &server_addr, &addrlen);
			if(received_size < 0){
				printf("error at receivfrom\n");
				exit(1);
			}
		}

		printf("got request :\n");
		printf("%s\n", request);

		//routing and make responce
		char *method = strtok(request, " ");
		char *path = strtok(NULL, " ");
		char *response;
		response = (char *) malloc(SIZE_RESPONSE);

		if(!strcmp(method, "GET")){
			Route(response, path);
		}else{
			MakeResponse(response, 500, "only accepte GET method.");
		}		

		printf("resoponse :\n%s\n", response);
		int response_size;
		if(tcp){
			response_size = sendto(accepted_soc, response, strlen(response),
				0, (struct sockaddr *)&server_addr, addrlen);
		}else{
			response_size = sendto(soc, response, strlen(response),
				0, (struct sockaddr *)&server_addr, addrlen);
		}

		if(response_size < 0){
			printf("error at sendto\n");
			exit(1);
		}

		free(response);
		if(tcp){
			close(accepted_soc);
		}
		printf("response\n");
	}
	close(soc);
}

int main(int argc, char *argv[])
{
	if(!ParseArgs(argc - 1, argv + 1)){
		printf("Usage :\n");
		return 1;
	}
	port = StrToNum16(cmd_port, NULL);

	if(tcp){
		printf("communicate on tco/ip\nport : %d\n", port);	
	}else{
		printf("communicate on udp/ip\nport : %d\n", port);
	}

	StartServer();

	return 0;
}
