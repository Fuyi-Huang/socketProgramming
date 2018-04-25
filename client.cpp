// the compilation is successful using g++: 
//      g++ client.cpp -lws2_32 -lmswsock -ladvapi32 -o client
//      ./client
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>
#include <cstring>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define BUFFLEN 1024
const char* PORT = "8080";

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(iResult != 0)
    {
        printf("fail to startup");
        return 1;
    }

    addrinfo* result = NULL;
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_socktype = SOCK_STREAM;
    iResult = getaddrinfo("localhost", PORT, &hints, &result);
    if(iResult != 0)
    {
        printf("fail to get addr info");
        WSACleanup();
        return 1;
    }

    //attempt to connect an address until one succeed
    struct addrinfo* ptr = NULL;
    SOCKET connSocket = INVALID_SOCKET;
    for(ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        std::cout << result->ai_family << " " << result->ai_socktype << " " << result->ai_protocol << std::endl;
        connSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(connSocket == INVALID_SOCKET)
        {
            printf("fail to connect address");
            WSACleanup();
            return 1;
        }

        //connect to server
        iResult = connect(connSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if(iResult == SOCKET_ERROR)
        {
            closesocket(connSocket);
            WSACleanup();
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if(connSocket == SOCKET_ERROR)
    {
        printf("fail to connect to server");
        WSACleanup();
        return 1;
    }
    //send content to server
    char sendBuff[BUFFLEN];
    std::cin >> sendBuff;
    iResult = send(connSocket, sendBuff, (int)sizeof(sendBuff), 0);
    if(iResult == SOCKET_ERROR)
    {
        printf("fail to send message");
        closesocket(connSocket);
        WSACleanup();
        return 1;
    }

    // shutdown the connection since no more data will be sent
    iResult = shutdown(connSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(connSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    char recvBuff[BUFFLEN];
    do {

        iResult = recv(connSocket, recvBuff, BUFFLEN, 0);
        if ( iResult > 0 )
            printf("%s", recvBuff);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );

    closesocket(connSocket);
    WSACleanup();

    system("pause");
    return 0;
}