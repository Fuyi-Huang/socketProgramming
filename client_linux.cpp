//the compilation is successful using g++
//	g++ client_linux.cpp -o client_linux
//	./client_linux

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>

#define PORT 8080
#define BUFFLEN 1024

void error(const char* msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
	//creating a socket
	int clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd < 0)
		error("ERROR: opening socket");
	
	//connect to server socket
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons( PORT );
	if(connect(clientfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
		error("ERROR: connecting to server socket");
	printf("please enter the message: ");
	char buffer[BUFFLEN];
	bzero(buffer, BUFFLEN);
	fgets(buffer, BUFFLEN, stdin);
	int iResult = write(clientfd, buffer, strlen(buffer));
	if(iResult < 0)
		error("ERROR: writing to server socket");
	else
		printf("send message to server successfully");
	bzero(buffer, BUFFLEN);
	iResult = read(clientfd, buffer, BUFFLEN);
	if(iResult < 0)
		error("ERROR: reading from socket");
	printf("%s\n", buffer);

	return 0;
}
