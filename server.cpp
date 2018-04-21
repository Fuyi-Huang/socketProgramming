#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

// need to link with ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

const int BUFFLEN = 1024;
const char* PORT = "8080";

int main(int argc, char* argv[])
{
    int iResult;
    WSADATA wsData;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsData);
    if(iResult != 0)
    {
        printf("WSA startup failed");
        WSACleanup();
        return 1;
    }    
    //get address info
    struct addrinfo* result = NULL;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    iResult = getaddrinfo(NULL, PORT, &hints, &result);
    if(iResult != 0)
    {
        printf("fail to get address info");
        return 1;
    }
    //create listening socket
    SOCKET listen_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(listen_sock == INVALID_SOCKET)
    {
        printf("fail to create socket");
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    //bind socket
    iResult = bind(listen_sock, result->ai_addr, (int)result->ai_addrlen);
    if(iResult == SOCKET_ERROR)
    {
        printf("fail to bind socket");
        freeaddrinfo(result);
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);
    //listen the connection request, SOMMAXCONN is the maximal number of pending connection request
    iResult = listen(listen_sock, SOMAXCONN);
    if(iResult == SOCKET_ERROR)
    {
        printf("fail to listen");
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }
    //accept one connection request from the client
    SOCKET client_socket = accept(listen_sock, NULL, NULL);
    if(client_socket == INVALID_SOCKET)
    {
        printf("fail to accept client request");
        closesocket(listen_sock);
        WSACleanup();
        return 1;
    }

    //receive content from client
    char recvBuff[BUFFLEN];
    do{
        iResult = recv(client_socket, recvBuff, BUFFLEN, 0);
        if(iResult > 0)
        {
            printf("Bytes received: %d\n", iResult);
            printf(recvBuff);
            
            iResult = send(client_socket, "i have an pen!", sizeof("i have an pen!"), 0);
        }
        else if(iResult == 0)
            printf("closing connection with client");
        else
        {
            printf("fail to receive content from client");
            closesocket(client_socket);
            WSACleanup();
            return 1;
        }
    }while(iResult > 0);

    //close the connection with client
    iResult = shutdown(client_socket, SD_SEND);
    if(iResult == SOCKET_ERROR)
    {
        printf("fail to shutdown the connection with client");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }
    //close the socket
    closesocket(client_socket);
    closesocket(listen_sock);
    WSACleanup();

    system("pause");
    return 0;
}