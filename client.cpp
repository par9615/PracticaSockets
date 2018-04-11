#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
using namespace std;

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

void createClient(int port);
void sendMessage(char* msg);
char* receiveMessage(int client);
void* listener(void *arg);

int sockfd, len;
sockaddr_in address;
pthread_t tid;

int main()
{
	char nickname[1024], msg_out[1024];
	createClient(9734);

	cout<<"Ingresa tu nickname"<<endl;
	cin.getline(nickname, 1024);

	sendMessage(nickname);

	pthread_create(&tid, NULL, listener, NULL);	

	while(1)
	{
		cout<<"Escribe algo..."<<endl;
		cin.getline(msg_out, 1024);

			sendMessage(nickname);
			sendMessage(msg_out);

		if(strcmp(msg_out, "exit") == 0)
			break;	
	}

	close(sockfd);
	exit(0);
}

void *listener(void *arg)
{
	char *msg_in, *from;
	while(1)
	{
		from = receiveMessage(sockfd);	
		msg_in = receiveMessage(sockfd);


		cout<<from<<" dice:"<<endl;
		cout<<msg_in<<endl;
	}
}

void createClient(int port)
{
	int result;

	//creates socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//initialize socket address
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(port);

	len = sizeof(address);
	result = connect(sockfd, (sockaddr *)&address, len);

	if(result < 0)
	{
		cout<<"Error: connect failed"<<endl;
		exit(1);	
	}

}

void sendMessage(char* msg)
{
	int len = strlen(msg) + 1;
	int converted_len = htonl(len);

	//write text length
	write(sockfd, &converted_len, sizeof(converted_len));

	//write text
	write(sockfd, msg, len); 	
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

