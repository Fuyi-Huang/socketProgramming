//the compliation is successful using g++,
//	g++ -server_linux.cpp -o server_linux
//	./server_linux

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

#define PORT 8080
#define BUFFLEN 1024

int main(int argc, char* argv[])
{
	//create socket file descriptor
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0)
	{
		perror("socket failed");
		return 1;
	}
	
	struct sockaddr_in address;
	bzero((char*) &address, sizeof(address));
	int addrlen = sizeof(address);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	//forcefully attaching socket to port 8080
	if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if(listen(server_fd, 3) < 0)	//3 pending client connection at most
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	int new_socketfd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	if(new_socketfd < 0)
	{
		perror("accept failed");
		exit(EXIT_FAILURE);
	}
	char buffer[BUFFLEN];
	int valread = read(new_socketfd, buffer, BUFFLEN);
	if(valread < 0)
	{
		perror("error reading from socket");
		exit(EXIT_FAILURE);
	}
	printf("%s\n", buffer);
	char* pen = "i have a pen";
	valread = write(new_socketfd, pen, strlen(pen));
	if(valread < 0)
	{
		perror("error writing to socket");
		exit(EXIT_FAILURE);
	}
	printf("pen message sent\n");
	close(new_socketfd);
	close(server_fd);
	
	return 0;
}
