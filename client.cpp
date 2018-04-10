#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
using namespace std;

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

#define LOGIN 1
#define BROADCAST 2;

void createClient(int port);
void sendMessage(string msg);

int sockfd, len;
sockaddr_in address;

int main()
{
	string msg_out, msg_in;
	//msg_out.reserve(1024);
	msg_in.reserve(1024);

	msg_out = "abcdefghijklmnopqrstuvwyzabcdefghijklkjasñflk";

	createClient(9734);

	sendMessage(msg_out);

	//write(sockfd, &msg_out[0], msg_out.length());
	//read(sockfd, &msg_in[0], 1024);
	
	//cout<<"Recibio "<<msg_in<<endl;

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

void sendMessage(string msg)
{
	int len = msg.length()+1;
	int converted_len = htonl(len);

	cout<<"len "<<len<<endl;
	cout<<"converted len "<<converted_len<<endl;

	write(sockfd, &converted_len, sizeof(converted_len));
	write(sockfd, &msg[0], len); 	
}

