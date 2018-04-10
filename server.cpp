#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>
using namespace std;

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

void createServer(int port);
string receiveMessage(int client);

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
		string msg_out, msg_in;
		msg_out.reserve(1024);
		msg_in.reserve(1024);
		printf("server waiting\n");
		client_sockfd = accept(server_sockfd, (sockaddr *)&client_address, (unsigned int *)&client_len);
		
		
		msg_in = receiveMessage(client_sockfd);
		cout<<msg_in<<endl;

		/*read(client_sockfd, &msg_in[0], 20);
		cout.write(&msg_in[0], 100);	

		msg_out = "hola, soy el server al cual acabas de enviar mensaje, te informo que tu mensaje fue recibido";	
		write(client_sockfd, &msg_out[0], 10);*/

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

string receiveMessage(int client)
{
	string msg;
	int len;
	int unconverted_len;

	read(client, &unconverted_len, sizeof(unconverted_len));
	len = ntohl(unconverted_len);
	cout<<"len "<<len<<endl;
	cout<<"unconverted len "<<unconverted_len<<endl;
	msg.reserve(len);
	read(client, &msg[0], len);

	cout<<msg<<endl;
	return msg;
}