#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
using namespace std;

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

void createServer(int port);
char* receiveMessage(int client);

int server_sockfd, server_len;
sockaddr_in server_address;

int main()
{
	int client_sockfd;
	int server_len, client_len;
	sockaddr_in client_address;

	createServer(9734);

	while(1)
	{
		char *msg_in;

		printf("server waiting\n");
		client_sockfd = accept(server_sockfd, (sockaddr *)&client_address, (unsigned int *)&client_len);
		
		msg_in = receiveMessage(client_sockfd);
		cout<<"Llego mensaje: "<<msg_in<<endl;

		close(client_sockfd);
	}	
}

void createServer(int port)
{
	//creates socket
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//initialize socket address
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(port);	
	server_len = sizeof(server_address);

	bind(server_sockfd, (sockaddr *)&server_address, server_len);
	listen(server_sockfd, 5);
}

char* receiveMessage(int client)
{
	char* msg;
	int len;
	int unconverted_len;

	//read text len
	read(client, &unconverted_len, sizeof(unconverted_len));
	len = ntohl(unconverted_len);

	//read text
	read(client, msg, len);	

	return msg;
}