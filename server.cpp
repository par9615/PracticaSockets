#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <pthread.h>
using namespace std;

#define MAXCLIENT 10

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

void createServer(int port);
char* receiveMessage(int client);
void* serveClient(void* arg);
void sendMessage(char* msg, int client);

int server_sockfd, server_len, client_sockfd[MAXCLIENT], parameter[MAXCLIENT], active[MAXCLIENT];
sockaddr_in server_address;
pthread_t tid[MAXCLIENT];
char* username[MAXCLIENT];

int main()
{
	int server_len, client_len;
	sockaddr_in client_address;

	createServer(9734);
	printf("server waiting\n");

	while(1)
	{

		for(int i = 0; i < MAXCLIENT; i++)
		{
			client_sockfd[i] = accept(server_sockfd, (sockaddr *)&client_address, (unsigned int *)&client_len);

			parameter[i] = i;
			active[i] = 1;
			pthread_create(&tid[i], NULL, serveClient, (void*)&parameter[i]);

		}
		

		for(int i = 0; i < MAXCLIENT; i++)
			pthread_join(tid[i], NULL);

	}	
}

void* serveClient(void* arg)
{
	char *nickname, *msg_in;
	int n = *((int*)arg);

	nickname = receiveMessage(client_sockfd[n]);
	username[n] = nickname;

	while(1)
	{
		nickname = receiveMessage(client_sockfd[n]);
		msg_in = receiveMessage(client_sockfd[n]);

		if(strcmp(msg_in, "exit") == 0)
			break;	

		for(int i = 0; i < MAXCLIENT; i++)
			if(i != n)
			{
				sendMessage(nickname, client_sockfd[i]);	
				sendMessage(msg_in, client_sockfd[i]);		
			}
	}

	shutdown(client_sockfd[n], SHUT_RDWR);
	close(client_sockfd[n]);

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
	msg = (char*)malloc(len);

	//read text
	read(client, msg, len);	

	return msg;
}

void sendMessage(char* msg, int client)
{
	int len = strlen(msg) + 1;
	int converted_len = htonl(len);

	//write text length
	write(client, &converted_len, sizeof(converted_len));

	//write text
	write(client, msg, len); 	
}