#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
using namespace std;

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

void createClient(int port);
void sendMessage(char* msg);

int sockfd, len;
sockaddr_in address;

int main()
{
	char msg_out[1024], msg_in[1024];

	cout<<"Ingresa cadena"<<endl;
	cin>>msg_out;
	createClient(9734);

	sendMessage(msg_out);

	close(sockfd);
	exit(0);
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

